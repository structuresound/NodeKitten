//
//  GLState.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/19/15.
//
//

#include "GLState.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Camera.h"

GLCullFaceMode GLState::_cullFace = GLCullFaceNone;
GLBlendMode GLState::_blendMode = GLBlendModeNone;
bool GLState::_usesDepth = false;
bool GLState::_lineSmooth = false;
bool GLState::_isInit = false;

void GLState::glInit(){
    // MUST BE CALLED WITH ACTIVE OPEN GL CONTEXT
    if (!_isInit) {
        _isInit = true;
        
        GetGLError();
        
        glEnable(GL_BLEND);
        
        GetGLError();
        
        setUsesDepth(true);
        
        glLineWidth(1.0f);
        
        GetGLError();
        
#if !(NK_USE_WEBGL || NK_USE_GLES)
        glEnable( GL_POLYGON_SMOOTH );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
        
#if !NK_USE_GL3
        glEnable(GL_MULTISAMPLE_ARB);
        glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
#endif
        
        float gran;
        glGetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &gran);
        nkLog("smooth line gran: %f",gran);
#endif
        
        GetGLError();
        
        Camera::glInit();
        Mesh::glInit();
        Shader::glInit();
    }
}

void GLState::setLineSmooth(bool smooth) {
#if !NK_USE_GLES
    if (smooth) {
        glEnable(GL_LINE_SMOOTH);
    }
    else {
        glDisable(GL_LINE_SMOOTH);
    }
    nkGetGLError();
#endif
}

void GLState::setCullFace(GLCullFaceMode cullFace) {
    
    if (_cullFace != cullFace) {
        
        switch (cullFace) {
                
            case GLCullFaceNone:
                glDisable(GL_CULL_FACE);
                break;
                
            case GLCullFaceFront:
                if (_cullFace < 1) {
                    glEnable(GL_CULL_FACE);
                }
                glCullFace(GL_FRONT);
                break;
                
            case GLCullFaceBack:
                if (_cullFace < 1) {
                    glEnable(GL_CULL_FACE);
                }
                glCullFace(GL_BACK);
                break;
                
            case GLCullFaceBoth:
                if (_cullFace < 1) {
                    glEnable(GL_CULL_FACE);
                }
                glCullFace(GL_FRONT_AND_BACK);
                break;
                
            default:
                break;
        }
        
        nkGetGLError();
        _cullFace = cullFace;
    }
    
}

void GLState::setBlendMode(GLBlendMode blendMode) {
    if (_blendMode != blendMode) {
        
        if (_blendMode == GLBlendModeNone) {
            glEnable(GL_BLEND);
        }
        
        switch (blendMode){
            case GLBlendModeNone:
                glDisable(GL_BLEND);
                break;
                
            case GLBlendModeAlpha:
                //glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
                //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
                break;
                
            case GLBlendModeFBO:
                //glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
                break;
                
            case GLBlendModeAdd:{
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            }
                
            case GLBlendModeMultiply:{
                glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
                break;
            }
                
            case GLBlendModeScreen:{
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
                break;
            }
                
            case GLBlendModeSubtract:{
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            }
                
            default:
                break;
        }
        
        nkGetGLError();
        _blendMode = blendMode;
    }
    
}

void GLState::setUsesDepth(bool usesDepth) {
    if (_usesDepth != usesDepth) {
        if (usesDepth) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
        }
        else {
            glDisable(GL_DEPTH_TEST);
        }
        
        nkGetGLError();
        _usesDepth = usesDepth;
    }
}

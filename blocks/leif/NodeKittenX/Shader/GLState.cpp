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
#include "Shader.h"

using namespace Shader;

Program* GLState::_activeShader = nullptr;

GLCullFaceMode GLState::_cullFace = GLCullFaceNone;
GLBlendMode GLState::_blendMode = GLBlendModeNone;

bool GLState::_usesDepth = false;
bool GLState::_lineSmooth = false;
bool GLState::_isInit  = false;

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
    Program::glInit();
  }
}

void GLState::GLSetLineSmooth() {
#if !NK_USE_GLES
  _lineSmooth ? glEnable(GL_LINE_SMOOTH) : glDisable(GL_LINE_SMOOTH);
  nkGetGLError();
#endif
}

void GLState::GLSetActiveLights() {
  _numActiveLights;
}

void GLState::GLSetActiveShader() {
  _activeShader ? glUseProgram(_activeShader->glName) : glUseProgram(0);
  nkGetGLError();
}

void GLState::GLSetCullFace() {
  switch (_cullFace) {
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
      nkAssert(0, "invalid GL cull face mode");
      break;
  }
  nkGetGLError();
}

void GLState::GLSetBlendMode(){
  if (_blendMode == GLBlendModeNone) {
    glEnable(GL_BLEND);
  }
  
  switch (_blendMode){
    case GLBlendModeNone:
      glDisable(GL_BLEND);
      break;
    case GLBlendModeAlpha:
      glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
      break;
    case GLBlendModeFBO:
      glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
      break;
    case GLBlendModeAdd:{
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      break;
    }
    case GLBlendModeMultiply:{
      glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
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
      nkAssert(0, "invalid GL blend mode");
  }
  
  nkGetGLError();
}
void GLState::GLSetDepth() {
    if (_usesDepth) {
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
    }
    else {
      glDisable(GL_DEPTH_TEST);
    }
    nkGetGLError();
}

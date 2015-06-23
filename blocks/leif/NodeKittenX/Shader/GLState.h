//
//  GLState.h
//  Pods
//
//  Created by Leif Shackelford on 6/19/15.
//
//

#ifndef __Pods__GLState__
#define __Pods__GLState__

#include "CommonTypes.h"

#define GetGLError()									\
{														\
GLenum err = glGetError();							\
while (err != GL_NO_ERROR) {						\
nkLog("GLError %s set in File:%s Line:%d\n",	\
GetGLErrorString(err),					\
__FILE__,								\
__LINE__);								\
err = glGetError();								\
}													\
}

#ifdef DEBUG
#if NK_LOG_GL
#define nkGetGLError() GetGLError()
#else
#define nkGetGLError()
#endif
#else
#define nkGetGLError()
#endif

#define glHasError()    (_glHasError(__PRETTY_FUNCTION__, __LINE__))
#ifdef DEBUG
# define glCheckAndClearErrorsIfDEBUG() glCheckAndClearErrors()
#else
# define glCheckAndClearErrorsIfDEBUG()
#endif

#define glCheckAndClearErrors() (_glCheckAndClearErrors(__PRETTY_FUNCTION__, __LINE__))


static inline const char * GetGLErrorString(GLenum error)
{
    const char *str;
    switch( error )
    {
        case GL_NO_ERROR:
            str = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            str = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            str = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            str = "GL_INVALID_OPERATION";
            break;
#if defined __gl_h_ || defined __gl3_h_
        case GL_OUT_OF_MEMORY:
            str = "GL_OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            str = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
#endif
#if defined __gl_h_
        case GL_STACK_OVERFLOW:
            str = "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            str = "GL_STACK_UNDERFLOW";
            break;
#if !TARGET_RASPBERRY_PI
        case GL_TABLE_TOO_LARGE:
            str = "GL_TABLE_TOO_LARGE";
            break;
#endif
#endif
        default:
            str = "(ERROR: Unknown Error Enum)";
            break;
    }
    return str;
}


static inline void _glCheckAndClearErrors(const char *function, int line)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        nkLog("%s, line %d: Error, OpenGL error: %s", function, line, GetGLErrorString(error));
    }
}

static inline bool _glHasError(const char *function, int line)
{
    GLenum error;
    bool errors = false;
    
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        errors = true;
        nkLog("%s, line %d: Error, OpenGL error: %s", function, line, GetGLErrorString(error));
    }
    
    return errors;
}

typedef enum GLBlendMode {
    GLBlendModeNone,
    GLBlendModeAlpha,
    GLBlendModeFBO,
    GLBlendModeAdd,
    GLBlendModeMultiply,
    GLBlendModeScreen,
    GLBlendModeSubtract
} GLBlendMode;

typedef enum GLCullFaceMode {
    GLCullFaceNone,
    GLCullFaceFront,
    GLCullFaceBack,
    GLCullFaceBoth
} GLCullFaceMode;


class GLState {
    static bool _isInit;
    static bool _lineSmooth;
    static bool _usesDepth;
    static GLBlendMode _blendMode;
    static GLCullFaceMode _cullFace;
    
public:
    
    static void setLineSmooth(bool smooth);
    static void setBlendMode(GLBlendMode blendMode);
    static void setCullFace(GLCullFaceMode cullFace);
    static void setUsesDepth(bool usesDepth);
    
    static void glInit();
};
#endif /* defined(__Pods__GLState__) */

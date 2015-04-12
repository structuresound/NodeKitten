//
//  NKFrameBuffer.h
//  NKNikeField
//
//  Created by Leif Shackelford on 5/7/14.
//  Copyright (c) 2014 Chroma Developer. All rights reserved.
//

#ifndef __NKFrameBuffer__
#define __NKFrameBuffer__

#include "NKTypes.h"


#define GetGLError()									\
{														\
GLenum err = glGetError();							\
while (err != GL_NO_ERROR) {						\
printf("GLError %s set in File:%s Line:%d\n",	\
GetGLErrorString(err),					\
__FILE__,								\
__LINE__);								\
err = glGetError();								\
}													\
}

#if NK_LOG_GL
#define nkGetGLError() GetGLError()
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
        printf("%s, line %d: Error, OpenGL error: %s", function, line, GetGLErrorString(error));
    }
}

static inline bool _glHasError(const char *function, int line)
{
    GLenum error;
    bool errors = false;
    
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        errors = true;
        printf("%s, line %d: Error, OpenGL error: %s", function, line, GetGLErrorString(error));
    }
    
    return errors;
}

class NKTexture;

class NKFrameBuffer
{
public:
    
    // INIT
    NKFrameBuffer(){} // DEFAULT FOR APPLE
    NKFrameBuffer(GLuint width, GLuint height, bool allocate = true);
    ~NKFrameBuffer();
    
    // IVARS
    
    GLuint glName {0};
    
    shared_ptr<NKTexture> renderTexture;
    shared_ptr<NKTexture> renderTexture2;
    shared_ptr<NKTexture> depthTexture;

    // METHODS
    
    
    void bind();
    
    void addSecondRenderTexture();
    void bindPing();
    void bindPong();
    
    void clear();
    
    
    void unbind();
    void unload();
    

    
    GLuint bindTexture(int glname);
    
    //NKImage *)imageAtRect:(CGRect)cropRect;
//    
//    - (NKByteColor*)colorAtPoint:(P2t)point;
//    - (void)pixelValuesInRect:(CGRect)cropRect buffer:(GLubyte *)pixelBuffer;
    
    GLuint renderBuffer;
    GLuint depthBuffer;
    I1t width {0};
    I1t height {0};
    
protected:

    
    void destroyFbo(int glname);
    void deleteFboAttachment(GLenum attachment);

    
};

#endif


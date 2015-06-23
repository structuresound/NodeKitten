//
//  FrameBuffer.h
//  NKNikeField
//
//  Created by Leif Shackelford on 5/7/14.
//  Copyright (c) 2014 Chroma Developer. All rights reserved.
//

#ifndef __FrameBuffer__
#define __FrameBuffer__

#include "CommonTypes.h"
#include "GLState.h"

class Texture;

class FrameBuffer
{

public:
    
    static vector<FrameBuffer*> stack;
    
    // INIT
    FrameBuffer(){} // DEFAULT FOR APPLE
    FrameBuffer(GLuint width, GLuint height, bool allocate = true);
    ~FrameBuffer();
    
    // IVARS
    
    GLuint glName {0};
    
    shared_ptr<Texture> renderTexture;
    shared_ptr<Texture> renderTexture2;
    shared_ptr<Texture> depthTexture;

    // METHODS
    
    
    void bind();
    
    void addSecondRenderTexture();
    void bindPing();
    void bindPong();
    
    void clear();

    void unbind();
    void unload();
    
    GLuint bindTexture(int glname);
    
    GLuint renderBuffer {0};
    GLuint depthBuffer {0};
    I1t width {0};
    I1t height {0};
    
    bool dirty {true};

#pragma mark - DEBUG
    
    static unsigned int createdObjects;
    static unsigned int destroyedObjects;
    static unsigned int liveObjects(){return createdObjects - destroyedObjects;};
    
protected:

    
    void destroyFbo(int glname);
    void deleteFboAttachment(GLenum attachment);

    
};

#endif


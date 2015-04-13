//
//  NKFrameBuffer.m
//  NKNikeField
//
//  Created by Leif Shackelford on 5/7/14.
//  Copyright (c) 2014 Chroma Developer. All rights reserved.
//
#include "NKFrameBuffer.h"
#include "NKTexture.h"

NKFrameBuffer::NKFrameBuffer(GLuint width_, GLuint height_, bool allocate) {
    
    width = width_;
    height = height_;
    
    if (!allocate) return;
    
#if NK_USE_GLES
    
    // 1 // Create the framebuffer and bind it.
    
    glGenFramebuffers(1, &glName);
    glBindFramebuffer(GL_FRAMEBUFFER, glName);
    
    // 2 // Create a color renderbuffer, allocate storage for it, and attach it to the framebuffer’s color attachment point.
    
    
    renderTexture = make_shared<NKTexture>(width, height);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->glName, 0);
    
    //        glGenRenderbuffers(1, &renderBuffer);
    //        glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    //
    //        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, width,height);
    //
    //        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    
    // 3 // Create a depth or depth/stencil renderbuffer, allocate storage for it, and attach it to the framebuffer’s depth attachment point.
    
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    
    
    
#else
    
    glGenFramebuffers(1, &glName);
    glBindFramebuffer(GL_FRAMEBUFFER, glName);
    
    // 1 color
    
    renderTexture = make_shared<NKTexture>(width, height);
    
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, width,height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->glName, 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("ERROR Creating color buffer\n");
        return;
    }
    
    // 2 depth
    
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    
    
    //        _depthTexture = [[NKTexture alloc]initWithWidth:width height:height];
    // glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture.glName, 0);
#endif
    
    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("ERROR Creating framebuffer \n\n");
        destroyFbo(glName);
    }

//glBindFramebuffer(GL_FRAMEBUFFER, 0);
printf("new framebuffer, col %d, dep %d \n\n", renderBuffer, depthBuffer);
printf("col tex is: %d size %d %d \n\n", renderTexture->glName, renderTexture->width, renderTexture->height);

}


void NKFrameBuffer::destroyFbo(int glname){
    if(0 == glname)
    {
        return;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, glname);
    
    GLint maxColorAttachments = 1;
    
    // OpenGL ES on iOS 4 has only 1 attachment.
    // There are many possible attachments on OpenGL
    // on MacOSX so we query how many below
#if !NK_USE_GLES
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
#endif
    
    GLint colorAttachment;
    
    // For every color buffer attached
    for(colorAttachment = 0; colorAttachment < maxColorAttachments; colorAttachment++)
    {
        // Delete the attachment
        deleteFboAttachment(GL_COLOR_ATTACHMENT0+colorAttachment);
    }
    
    // Delete any depth or stencil buffer attached
    deleteFboAttachment(GL_DEPTH_ATTACHMENT);
    
    deleteFboAttachment(GL_STENCIL_ATTACHMENT);
    
    glDeleteFramebuffers(1,(GLuint*)&glname);
}

void NKFrameBuffer::deleteFboAttachment(GLenum attachment)
{
    GLint param;
    GLuint objName;
    
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &param);
    
    if(GL_RENDERBUFFER == param)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &param);
        
        objName = ((GLuint*)(&param))[0];
        glDeleteRenderbuffers(1, &objName);
    }
    else if(GL_TEXTURE == param)
    {
        
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &param);
        
        objName = ((GLuint*)(&param))[0];
        glDeleteTextures(1, &objName);
    }
    
}

#pragma mark - Binding

void NKFrameBuffer::addSecondRenderTexture() {
    renderTexture = make_shared<NKTexture>(width,height);
}

void NKFrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, glName);
    nkGetGLError();
}

void NKFrameBuffer::bindPing() {
    glBindFramebuffer(GL_FRAMEBUFFER, glName);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->glName, 0);
    nkGetGLError();
}

void NKFrameBuffer::bindPong() {
    if (!renderTexture2) {
        renderTexture2 = make_shared<NKTexture>(width,height);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, glName);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->glName, 0);
    nkGetGLError();
}

void NKFrameBuffer::clear() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//void bind:(void(^)())drawingBlock
//{
//    [self bind];
//    drawingBlock();
//    [self unbind];
//}


void NKFrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NKFrameBuffer::unload() {
    
    unbind();
    
    if (renderBuffer) {
        glDeleteRenderbuffers(1, &renderBuffer);
    }
    if(depthBuffer)
    {
        glDeleteRenderbuffers(1, &depthBuffer);
        depthBuffer = 0;
    }
    if (glName) {
        glDeleteFramebuffers(1, &glName);
    }
    
}

NKFrameBuffer::~NKFrameBuffer() {
    unload();
}

// port
//- (NKByteColor*)colorAtPoint:(P2t)point {
//    
//    //printf("read pixels at %d, %d\n", (int)point.x, (int)point.y);
//    
//    NKByteColor *hit = [[NKByteColor alloc]init];
//    
//    [self bind];
//    
//    glReadPixels((int)point.x, (int)point.y,
//                 1, 1,
//                 GL_RGBA, GL_UNSIGNED_BYTE, hit.bytes);
//    
//    [self unbind];
//    
//    [hit log];
//    
//    return hit;
//    
//}

void pixelValuesInRect(V4t cropRect, GLubyte *pixelBuffer)
{
    GLint width = cropRect.size.width;
    GLint height = cropRect.size.height;
    glReadPixels(cropRect.origin.x, cropRect.origin.y,
                 width, height,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
}

//port
//- (NKImage *)imageAtRect:(CGRect)cropRect
//{
//    GLint width = cropRect.size.width;
//    GLint height = cropRect.size.height;
//    
//    NSInteger myDataLength = width * height * 4;
//    
//    // allocate array and read pixels into it.
//    GLubyte *buffer = (GLubyte *) malloc(myDataLength);
//    
//    [self pixelValuesInRect:cropRect buffer:buffer];
//    
//    return [NKImage imageWithBuffer:buffer ofSize:cropRect.size];
//}

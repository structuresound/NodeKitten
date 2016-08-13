//
//  FrameBuffer.m
//  NKNikeField
//
//  Created by Leif Shackelford on 5/7/14.
//  Copyright (c) 2014 Chroma Developer. All rights reserved.
//
#include "FrameBuffer.h"
#include "Texture.h"

std::vector<FrameBuffer*> FrameBuffer::stack;


FrameBuffer::FrameBuffer(V2t size_, bool shouldAllocate) {
  size.set(size_);
  if (shouldAllocate) allocate();
}

void FrameBuffer::afterResize() {
  if (_up){
    down();
    allocate();
  }
}

void FrameBuffer::allocate() {
  
#if NK_USE_GLES
  
  // 1 // Create the framebuffer and bind it.
  
  glGenFramebuffers(1, &glName);
  assert(glName);
  glBindFramebuffer(GL_FRAMEBUFFER, glName);
  
  // 2 // Create a color renderbuffer, allocate storage for it, and attach it to the framebuffer’s color attachment point.
  
  renderTexture = std::make_shared<Texture>(size.get().width, size.get().height);
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
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size.get().width, size.get().height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
  
#else
  
  glGenFramebuffers(1, &glName);
  glBindFramebuffer(GL_FRAMEBUFFER, glName);
  
  // 1 color
  
  renderTexture = std::make_shared<Texture>(size.get().width, size.get().height);
  
  glGenRenderbuffers(1, &renderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, (int)size.get().width,(int)size.get().height);
  
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->glName, 0);
  
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    nkLog("ERROR Creating color buffer");
    return;
  }
  
  // 2 depth
  
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size.get().width, size.get().height);
  
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
  
  
  //        _depthTexture = [[Texture alloc]initWithWidth:width height:height];
  // glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture.glName, 0);
#endif
  
  // Always check that our framebuffer is ok
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    nkLog("ERROR Creating framebuffer \n\n");
    destroyFbo(glName);
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //nkLog("<FBO>\nnew framebuffer, ", renderBuffer, depthBuffer);
  nkLog("gen fb, tex: %d, col %d, dep %d, size %d %d \n", renderTexture->glName, renderBuffer, depthBuffer, renderTexture->width, renderTexture->height);
  
  _up = true;
  _dirty = true;
  
}


void FrameBuffer::destroyFbo(int glname){
  if (_up){
    down();
  }
  
//  if(0 == glname)
//  {
//    return;
//  }
//  
//  glBindFramebuffer(GL_FRAMEBUFFER, glname);
//  
//  GLint maxColorAttachments = 1;
//  
//  // OpenGL ES on iOS 4 has only 1 attachment.
//  // There are many possible attachments on OpenGL
//  // on MacOSX so we query how many below
//#if !NK_USE_GLES
//  glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
//#endif
//  
//  GLint colorAttachment;
//  
//  // For every color buffer attached
//  for(colorAttachment = 0; colorAttachment < maxColorAttachments; colorAttachment++)
//  {
//    // Delete the attachment
//    deleteFboAttachment(GL_COLOR_ATTACHMENT0+colorAttachment);
//  }
//  
//  // Delete any depth or stencil buffer attached
//  deleteFboAttachment(GL_DEPTH_ATTACHMENT);
//  
//  deleteFboAttachment(GL_STENCIL_ATTACHMENT);
//  
//  glDeleteFramebuffers(1,(GLuint*)&glname);
}

void FrameBuffer::deleteFboAttachment(GLenum attachment)
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

void FrameBuffer::addSecondRenderTexture() {
  renderTexture = std::make_shared<Texture>(size.get().width,size.get().height);
}

void FrameBuffer::bind()
{
  if (stack.size()) {
    if (stack.back() == this) {
      nkLog("warning, framebuffer already bound, duplicate bind() calls ?");
      return;
    }
  }
  stack.push_back(this);
  bindPing();
}

void FrameBuffer::unbind()
{
  if (stack.size()) {
    stack.pop_back();
    if (stack.size()){
      stack.back()->bindPing();
    }
    else {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  }
  else {
    nkLog("framebuffer stack underflow, duplicate unbind() calls ?");
  }
}


void FrameBuffer::bindPing() {
  glBindFramebuffer(GL_FRAMEBUFFER, glName);
  if (renderTexture && renderTexture->glName){
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->glName, 0);
  }
  nkGetGLError();
}

void FrameBuffer::bindPong() {
  if (!renderTexture2) {
    renderTexture2 = std::make_shared<Texture>(size.get().width,size.get().height);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, glName);
  if (renderTexture2->glName){
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture2->glName, 0);
  }
  nkGetGLError();
}

void FrameBuffer::clear(Color color) {
  //glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(color.r, color.g, color.b, color.a);
}


void FrameBuffer::down() {
  
  if (stack.size() && stack.back() == this){
    unbind();
  }
  
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
  
  _up = false;
  
}

FrameBuffer::~FrameBuffer() {
  if (_up){
    down();
  }
}

// port
//- (UB4t*)colorAtPoint:(P2t)point {
//
//    //printf("read pixels at %d, %d\n", (int)point.x, (int)point.y);
//
//    UB4t *hit = [[UB4t alloc]init];
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

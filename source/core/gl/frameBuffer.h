//
//  FrameBuffer.h
//  NKNikeField
//
//  Created by Leif Shackelford on 5/7/14.
//  Copyright (c) 2014 Chroma Developer. All rights reserved.
//

#pragma once

#ifndef __FrameBuffer__
#define __FrameBuffer__

#include "../type.h"
#include "state.h"
#include "../generic/generic.h"

class Texture;

class FrameBuffer : public Sizeable<V2t>, public CountedClass<FrameBuffer>
{

  bool _dirty {true};
   bool _up {false};
public:

  static std::vector<FrameBuffer*> stack;

  // INIT
  FrameBuffer(){}; // DEFAULT FOR APPLE
  FrameBuffer(V2 size, bool shouldAllocate = true);

  ~FrameBuffer();

  // IVARS

  GLuint glName {0};

  std::shared_ptr<Texture> renderTexture;
  std::shared_ptr<Texture> renderTexture2;
  std::shared_ptr<Texture> depthTexture;

  // METHODS
  void afterResize() override;
  void allocate();

  void bind();

  void addSecondRenderTexture();
  void bindPing();
  void bindPong();

  void clear(Color = CLEAR);

  void unbind();
  bool up(){_up = true;};
  void down();

  GLuint bindTexture(int glname);

  GLuint renderBuffer {0};
  GLuint depthBuffer {0};

  const bool dirty() const {return _dirty;}
  void setDirty(bool dirty = true) {_dirty = dirty;}

protected:

  void destroyFbo(int glname);
  void deleteFboAttachment(GLenum attachment);
};

#endif

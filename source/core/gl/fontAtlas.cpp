//
// Created by Leif Shackelford on 4/25/15.
//

#include "fontAtlas.h"

void FontAtlas::bind() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(target, atlas->id);
}

void FontAtlas::upload() {
  // --------------------------------------------------- texture_atlas_upload
  // ---
  assert(atlas);
  assert(atlas->data);

  if (!atlas->id) {
    glGenTextures(1, &atlas->id);
  }

  glBindTexture(GL_TEXTURE_2D, atlas->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if (atlas->depth == 4) {
#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas->width, atlas->height, 0,
                 GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, atlas->data);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas->width, atlas->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, atlas->data);
#endif
  } else if (atlas->depth == 3) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlas->width, atlas->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, atlas->data);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height, 0,
                 GL_RED, GL_UNSIGNED_BYTE, atlas->data);
  }
}

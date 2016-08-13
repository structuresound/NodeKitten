//
// Created by Leif Shackelford on 4/25/15.
//

#include "fontAtlas.h"

void FontAtlas::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(target, atlas->id );
}

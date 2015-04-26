//
// Created by Leif Shackelford on 4/25/15.
//

#ifndef NODEKITTENX_NKFONTATLAS_H
#define NODEKITTENX_NKFONTATLAS_H

#include "freetype-gl.h"
#include "texture-atlas.h"
#include "texture-font.h"

#include "NKTexture.h"

class NKFontAtlas : public NKTexture {

public:
    NKFontAtlas(I1t width, I1t height) {
        fontTexture = texture_atlas_new(width, height, 8);
    }
    ~NKFontAtlas(){
        if (fontTexture){
            texture_atlas_delete(fontTexture);
        }
    }

    virtual void bind();

    texture_atlas_t *fontTexture;
};


#endif //NODEKITTENX_NKFONTATLAS_H

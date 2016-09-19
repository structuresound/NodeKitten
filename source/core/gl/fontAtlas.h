//
// Created by Leif Shackelford on 4/25/15.
//

#ifndef NODEKITTENX_NKFONTATLAS_H
#define NODEKITTENX_NKFONTATLAS_H

#include "texture.h"
#include "vertexBuffer.h"
#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>

class FontAtlas: public Texture {

public:

        texture_font_t *font = nullptr;
        texture_atlas_t *atlas = nullptr;

        FontAtlas(std::string fontName, U1t fontSize = 48, I1t width = 512, I1t height = 512){
                atlas = texture_atlas_new( width, height, 1);
                std::string filename = File::pathForFileNamed(fontName);

                nkLog("load font: %s", filename);
                font = texture_font_new_from_file( atlas, fontSize, filename.c_str() );

                const char * cache = " !\"#$%&'()*+,-./0123456789:;<=>?"
                  "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                  "`abcdefghijklmnopqrstuvwxyz{|}~";

                texture_font_load_glyphs( font, cache );

                GetGLError();

                bytes = 1;
                internal_format = format = GL_RED;
                texelType = GL_UNSIGNED_BYTE;
                size = width * height * bytes;
                target = GL_TEXTURE_2D;
                texelArray = atlas->data;

                allocate(NK_TEXTURE_CLAMP, 0, 0, 0);
                nkLog("gen atlas texture : %d", glName);

                atlas->id = glName;
                GetGLError();
        }

        ~FontAtlas(){
                if (atlas) {
                        texture_atlas_delete(atlas);
                        if (texelArray) {
                                texelArray = nullptr; // so texture doesn't double free the pointer
                        }
                }
                if (font) {
                        texture_font_delete(font);
                }
        }
};


#endif //NODEKITTENX_NKFONTATLAS_H

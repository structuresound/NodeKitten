#pragma once

#include "texture.h"
#include "vertexBuffer.h"
#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>
#include <freetype-gl/utf8-utils.h>

class FontAtlas : public Texture {

public:

    texture_font_t *font = nullptr;
    texture_atlas_t *atlas = nullptr;

    FontAtlas(std::string fontName, U1t fontSize = 48, I1t width = 512, I1t height = 512) {
        atlas = texture_atlas_new(width, height, 1);

        const char *cache = " !\"#$%&'()*+,-./0123456789:;<=>?"
                "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                "`abcdefghijklmnopqrstuvwxyz{|}~";

        size_t minsize = fontSize, maxsize = fontSize;
        size_t count = maxsize - minsize;
        size_t i, missed = 0;

        auto filename = File::pathForFileNamed(fontName);
        nkLog("load font: %s", filename);

        font = texture_font_new_from_file(atlas, fontSize, filename.c_str());
        missed += texture_font_load_glyphs(font, cache);
//        for (i = minsize; i < maxsize; ++i) {
//            texture_font_t *font = texture_font_new_from_file(atlas, i, filename.c_str());
//            missed += texture_font_load_glyphs(font, cache);
//            texture_font_delete(font);
//        }

        nkLog("Matched font               : %s\n", filename);
        nkLog("Number of fonts            : %ld\n", count);
        nkLog("Number of glyphs per font  : %ld\n", utf8_strlen(cache));
        nkLog("Number of missed glyphs    : %ld\n", missed);
        nkLog("Total number of glyphs     : %ld/%ld\n",
              utf8_strlen(cache) * count - missed, utf8_strlen(cache) * count);
        nkLog("Texture size               : %ldx%ld\n", atlas->width, atlas->height);
        nkLog("Texture occupancy          : %.2f%%\n",
              100.0 * atlas->used / (float) (atlas->width * atlas->height));

        GetGLError();

//        bytes = 1;
//        internal_format = format = GL_RED;
//        texelType = GL_UNSIGNED_BYTE;
//        size = width * height * bytes;
//        target = GL_TEXTURE_2D;
//        texelArray = atlas->data;
//
//        genGLTexture();
//        glTexImage2D(target, 0, internal_format, width, height, 0, format, texelType, texelArray);
//        atlas->id = glName;
        glName = upload();

        nkLog("gen atlas texture : %d", glName);
        GetGLError();
    }

    ~FontAtlas() {
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

    int upload() {
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas->width, atlas->height,
                         0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, atlas->data);
#else
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, atlas->width, atlas->height,
                      0, GL_RGBA, GL_UNSIGNED_BYTE, atlas->data );
#endif
        } else if (atlas->depth == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlas->width, atlas->height,
                         0, GL_RGB, GL_UNSIGNED_BYTE, atlas->data);
        } else {
#if defined(GL_ES_VERSION_2_0) || defined(GL_ES_VERSION_3_0)
            glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, atlas->width, atlas->height,
                      0, GL_LUMINANCE, GL_UNSIGNED_BYTE, atlas->data );
#else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->width, atlas->height,
                         0, GL_RED, GL_UNSIGNED_BYTE, atlas->data);
#endif
        }

        return atlas->id;
    }

};
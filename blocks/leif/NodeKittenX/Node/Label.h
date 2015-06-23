//
//  Label.h
//  Pods
//
//  Created by Leif Shackelford on 5/2/15.
//
//

#ifndef __Label__
#define __Label__

#include "Mesh.h"
#include "FontAtlas.h"

typedef enum NKTextAlignment {
    NKTextAlignmentCenter,
    NKTextAlignmentLeft,
    NKTextAlignmentRight,
    NKTextAlignmentTop,
    NKTextAlignmentBottom
};

class Label : public Mesh
{
    wstring _text;
    shared_ptr<FontAtlas> _fontAtlas;
    NKTextAlignment _textAlignment = NKTextAlignmentCenter;
    double _fontSize = 12;
    double _loadedSize = 64;
    double t_scale = (_fontSize / _loadedSize);
    
    string _fontName;
public:
    
    Label(string fontName, double fontSize = 12, Color color = Color(1.0), V3t size = V3(1.0));
    ~Label(){};
    
    void setText(string text);
    void setText(wstring text);
    V2t sizeForText();
    void genVertices();
    void genTexture();
    
    wstring text(){return _text;};
    
    void setTextAlignment(NKTextAlignment alignment);
    void setFontSize(double size);
    void setTexture(shared_ptr<Texture> fontAtlas) override;
    void chooseShader() override;
};

#endif /* defined(__Pods__Label__) */

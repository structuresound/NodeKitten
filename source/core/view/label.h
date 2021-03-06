//
//  Label.h
//  Pods
//
//  Created by Leif Shackelford on 5/2/15.
//
//

#ifndef __Label__
#define __Label__

#include "../node/mesh.h"
#include "../gl/fontAtlas.h"

typedef enum NKTextAlignment {
  NKTextAlignmentCenter,
  NKTextAlignmentLeft,
  NKTextAlignmentRight,
  NKTextAlignmentTop,
  NKTextAlignmentBottom
};

class Label : public Mesh
{
  std::string _text;
  std::shared_ptr<FontAtlas> _fontAtlas;
  NKTextAlignment _textAlignment = NKTextAlignmentLeft;
  double _fontSize = 12;
  double _loadedSize = 64;
  double t_scale = (_fontSize / _loadedSize);

  std::string _fontName;
public:

  static std::shared_ptr<Label> labelWithFontNamed(std::string fontName = "Roboto.ttf", double fontSize = 12, Color color = WHITE);

  Label(std::string fontName, double fontSize = 12, Color color = Color(1.0));

  void setText(std::string text);
  V2t sizeForText();
  void genVertices();
  void genTexture();

  std::string text(){return _text;};

  void setTextAlignment(NKTextAlignment alignment);
  void setFontSize(double size);

  // Overrides
  //virtual void afterTransform() override;
  void setTexture(std::shared_ptr<Texture> fontAtlas) override;
  void chooseShader() override;
};

#endif /* defined(__Pods__Label__) */

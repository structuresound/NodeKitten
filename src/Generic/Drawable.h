#pragma once

#include "GLState.h"

template <class Derived, class ColorType>
class Drawable {
  ColorType _color;
  F1t _alpha {1.0};
  F1t _colorBlendFactor {1.0};
  
  GLBlendMode _blendMode {GLBlendModeAlpha};
  GLCullFaceMode _cullFace {GLCullFaceNone};
  
  bool _usesDepth {true};
  bool _forceOrthographic {false};
  bool _ignoresLights {false};
  
  bool _cullable {true};
  bool _paused {false};
  bool _hidden {false};
  
public:
  
  bool hidden() const {return _hidden;}
  bool isVisible() const {return !_hidden && _color.visible();};
  
  void setCullable(bool cullable){_cullable = cullable;};
  const bool cullable() const {return _cullable;}
  
  bool isOpaque() const {
    return !(_alpha < 1.0 || _color.a < 1.0);
  }
  
  const F1t colorBlendFactor() const {return _colorBlendFactor;}
  void setColorBlendFactor(F1t colorBlendFactor) {_colorBlendFactor = colorBlendFactor;}
  
  const ColorType color() const {
    return _color;
  }
  virtual void setColor(ColorType color){
    _color = color;
  }
  
  virtual F1t alpha() const {return _alpha;}
  void setAlpha(F1t alpha){_alpha = alpha;}
  
  ColorType glColor() const {
    if (_colorBlendFactor == 1.0f) {
      return _color.fade(_alpha);
    }
    else {
      return _color.blend(_colorBlendFactor).fade(_alpha);
    }
  }
  
#pragma mark - GL States
  const bool ignoresLights() const {return _ignoresLights;}
  void setIgnoresLights(bool ignoresLights = true) {_ignoresLights = ignoresLights;}
  
  const bool forceOrthographic() const {return _forceOrthographic;}
  void setForceOrthographic(bool forceOrthographic = true) {_forceOrthographic = forceOrthographic; if (_forceOrthographic) setCullable(false);
  }
  
  const GLBlendMode blendMode() const {return _blendMode;}
  const GLCullFaceMode cullFace() const {return _cullFace;}
  const bool usesDepth() const {return _usesDepth;}
  
  virtual void setBlendMode(GLBlendMode blendMode) {_blendMode = blendMode;}
  virtual void setCullFace(GLCullFaceMode cullFace) {_cullFace = cullFace;}
  virtual void setUsesDepth(bool usesDepth) {_usesDepth = usesDepth;}
  
private:
  Derived& self() { return *static_cast<Derived*>(this); }
  const Derived& self() const { return *static_cast<const Derived*>(this); }
};
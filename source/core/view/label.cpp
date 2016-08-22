#include "label.h"
#include "../gl/shaderProgram.h"
#include "scene.h"
#include "../node/camera.h"
#include "../platform/platform.h"

using namespace std;
using namespace Shader;

shared_ptr<Label> Label::labelWithFontNamed(string fontName, double fontSize, Color color){
  return make_shared<Label>(fontName, fontSize, color);
}

Label::Label(string fontName, double fontSize, Color color) :
Mesh(nullptr, color, V3{0})
{
  _fontSize = fontSize;
  t_scale = (_fontSize * Platform::Screen::scale()) / _loadedSize;
  _fontName = fontName;
  _drawMode = GL_TRIANGLES;
  setCullFace(GLCullFaceBack);
  setForceOrthographic();
  size.afterSet([this](bool changed){
    if (changed){
      genVertices();
    }
  });
  auto exist = Texture::fontCache[fontName];
  if (!exist){
    Texture::fontCache[fontName] = make_shared<FontAtlas>(fontName, _loadedSize, 2048, 2048);
  }
  setTexture(Texture::fontCache[fontName]);

  modelViewMatrix.setPullFunction([this]{
    M16t v = forceOrthographic() ? M16MakeLookAt({0,0,1}, {0,0,0}, {0,1,0}) : scene()->camera->viewMatrix.get();
    return M16Multiply(v, globalTransform.get());
  });
}

void Label::setTexture(shared_ptr<Texture> fontAtlas){
  _fontAtlas = static_pointer_cast<FontAtlas>(fontAtlas);
  Mesh::setTexture(_fontAtlas);
}

void Label::chooseShader() {
  if (!shader) {
    auto modules = vector<ShaderModule> {ShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, 0),ShaderModule::freetypeModule()};
    shader = Program::shaderNamed("freeTypeShader", modules, 0);
    //       _shader = Shader::shaderNamed("uColorLightShader",NKS_COLOR_MODE_UNIFORM,0,1);
  }
}

V2t Label::sizeForText() {

  V2t pen;
  V4t bbox;

  for(size_t i=0; i<_text.length(); ++i)
  {
    texture_glyph_t *glyph = texture_font_get_glyph( _fontAtlas->font, _text[i] );

    if( glyph != NULL )
    {
      float kerning = 0.0f;

      if( i > 0)
      {
        kerning = texture_glyph_get_kerning( glyph, _text[i-1] );
      }

      pen.x += kerning;

      float x0  = ( pen.x + glyph->offset_x );
      float y0  = ( pen.y + glyph->offset_y );
      float x1  = x0 + (glyph->width);
      float y1  = y0 - (glyph->height);

      pen.x += glyph->advance_x;

      if  (x0 < bbox.x)                bbox.x = x0;
      if  (y1 < bbox.y)                bbox.y = y1;
      if ((x1 - bbox.x) > bbox.width)  bbox.width  = x1-bbox.x;
      if ((y0 - bbox.y) > bbox.height) bbox.height = y0-bbox.y;
    }
  }

  return bbox.size;
}

//void Label::afterTransform() {
//  Mesh::afterTransform();
//}

void Label::genVertices() {

  auto tsize = sizeForText();

  //nkLog("fontSize: %1.2f - text scale : %1.5f", _fontSize, t_scale);

  if (!_vertexBuffer) {
    _vertexBuffer = make_shared<VertexBuffer>();
  }

  _vertexBuffer->clear();

  bool clear_sub_pixel = false;

  V2t pen = {0,0};

  switch (_textAlignment) {
    case NKTextAlignmentLeft:
      pen.x = size.get().width * -0.5f;
      break;

    case NKTextAlignmentCenter:
      pen.x = tsize.width * -0.5f;
      break;

    case NKTextAlignmentRight:
      pen.x = size.get().width - tsize.width;
      break;
  }

  for(size_t i=0; i<_text.length(); ++i)
  {
    texture_glyph_t *glyph = texture_font_get_glyph(_fontAtlas->font, _text[i] );
    if( glyph != NULL )
    {
      float kerning = 0.0f;
      if( i > 0)
      {
        kerning = texture_glyph_get_kerning( glyph, _text[i-1] );
      }
      pen.x += kerning;
      float x0  = ( pen.x + glyph->offset_x ) * t_scale;
      float y0  = ( pen.y + glyph->offset_y ) * t_scale;
      float x1  = x0 + (glyph->width * t_scale);
      float y1  = y0 - (glyph->height * t_scale);
      float s0 = glyph->s0;
      float t0 = glyph->t0;
      float s1 = glyph->s1;
      float t1 = glyph->t1;

      if (clear_sub_pixel){
        x0 = int (x0);
        x1 = int (x1);
        y0 = int (y0);
        y1 = int (y1);
      }

      U1t start = (U1t)_vertexBuffer->_vertices.size();

      _vertexBuffer->_vertices.push_back({ x0,y0,0});
      _vertexBuffer->_vertices.push_back({ x0,y1,0});
      _vertexBuffer->_vertices.push_back({ x1,y1,0});
      _vertexBuffer->_vertices.push_back({ x1,y0,0});

      _vertexBuffer->_texCoords.push_back({s0,t0,0});
      _vertexBuffer->_texCoords.push_back({s0,t1,0});
      _vertexBuffer->_texCoords.push_back({s1,t1,0});
      _vertexBuffer->_texCoords.push_back({s1,t0,0});

      _vertexBuffer->_normals.push_back({ 0,0,1});
      _vertexBuffer->_normals.push_back({ 0,0,1});
      _vertexBuffer->_normals.push_back({ 0,0,1});
      _vertexBuffer->_normals.push_back({ 0,0,1});

      _vertexBuffer->_colors.push_back(glColor());
      _vertexBuffer->_colors.push_back(glColor());
      _vertexBuffer->_colors.push_back(glColor());
      _vertexBuffer->_colors.push_back(glColor());

      vector<U1t> indices;

      _vertexBuffer->_indices.push_back(start+0);
      _vertexBuffer->_indices.push_back(start+1);
      _vertexBuffer->_indices.push_back(start+2);
      _vertexBuffer->_indices.push_back(start+0);
      _vertexBuffer->_indices.push_back(start+2);
      _vertexBuffer->_indices.push_back(start+3);

      pen.x += glyph->advance_x;
    }
  }

  if (_vertexBuffer->size() > 0){
    _vertexBuffer->setupVAOInterlaceAndBufferVBO();
  }
}

// --------------------------------------------------------------- add_text ---
void Label::setText(string text)
{
  if (String::wstring2string(_text) == text) return;
  auto wtext = String::string2wstring(text);
  _text = wtext;
  genVertices();
}

void Label::setText(wstring text)
{
  if (_text == text) return;
  _text = text;
  genVertices();
}

void Label::setFontSize(double size){
  if (_fontSize == size) return;
  _fontSize = size;
  t_scale = (_fontSize * Platform::Screen::scale()) / _loadedSize;
}

void Label::setTextAlignment(NKTextAlignment alignment){
  if (_textAlignment == alignment) return;
  _textAlignment = alignment;
  genVertices();
}

void Label::genTexture() {
  setTexture(make_shared<FontAtlas>(_fontName, _loadedSize, 2048, 2048));
}

#pragma once

#include "shaderModule.h"

// ATTRIBUTES

//GLint uniforms[NUM_UNIFORMS];

class Node;
class Texture;

#define NUM_ATTRIBUTES 7

namespace Shader {

  using std::vector;
  using std::string;
  using std::shared_ptr;
  using std::make_shared;
  using std::map;

  class Program : public std::enable_shared_from_this<Program> {

      static vector<bool> vertexAttributeState;

      U1t _numTextures{0};
      U1t _batchSize{0};
      U1t _activeLights{0};

      NKS_COLOR_MODE _colorMode{NKS_COLOR_MODE_NONE};

      vector<string> extensions;

      vector<ShaderVariable> _attributes;
      vector<ShaderVariable> _uniforms;
      vector<ShaderVariable> _varyings;

      vector<string> _vertMain;
      vector<string> _fragMain;

      vector<ShaderVariable> _vertVars;
      vector<ShaderVariable> _fragVars;

      string _vertShaderPath;
      string _fragShaderPath;

  public:
      // GLOBALS
      static void glInit();

      static shared_ptr <Program> fill;
      static shared_ptr <Program> textureFill;

      static void newUIDColorForNode(Node *node);

      static Node *nodeForColor(UB4t color);

      static map <string, shared_ptr<Program>> programCache;
      static map<UB4t, Node *> nodeMap;

      static shared_ptr <Program> shaderNamed(string name);

      static shared_ptr <Program> shaderNamed(string name, vector <ShaderModule> modules, int batchSize = 0);

      static shared_ptr <Program> shaderNamed(string name, NKS_COLOR_MODE colorMode, U1t numTextures = 0, int numLights = 0,
                                             int batchSize = 0);

      Program();

      Program(string name, vector <ShaderModule> modules, int batchSize);

      Program(string name_, NKS_COLOR_MODE colorMode, U1t numTex, int numLights, int batchSize);

      ~Program();

      vector <ShaderModule> modules;

      string vertexSource();

      string fragmentSource();

      string name;
      GLuint glName{0};

      U1t batchSize{0};
      U1t numPasses{0};
      U1t currentPass{0};

      void setActiveLights(U1t activeLights) { _activeLights = activeLights; };

      vector <string> glslExtensions;

      bool hasAttributeNamed(NKS_ENUM name);

      ShaderVariable &attributeNamed(NKS_ENUM name);

      bool hasUniformNamed(NKS_ENUM name);

      ShaderVariable &uniformNamed(NKS_ENUM name);

      bool hasVaryingNamed(NKS_ENUM name);

      ShaderVariable &varyingNamed(NKS_ENUM name);

      ShaderVariable &vertVarNamed(NKS_ENUM name);

      ShaderVariable &fragVarNamed(NKS_ENUM name);

      bool load();

      void unload();

      bool use();

      bool operator==(const Program &other);

      void setVertexAttributeState(U1t attribute, bool state) {

          if (vertexAttributeState[attribute] != state) {
              if (state) { // ENABLE
                  glEnableVertexAttribArray(attribute);
                  //glBindAttribLocation(glName, attribute, nks(attribute).c_str());
                  nkLog("(*) %d : %s", (int) attribute, nks((NKS_ENUM) attribute));
              }

              else { // DISABLE
                  glDisableVertexAttribArray(attribute);
                  //nkLog("( ) %d : %s", (int)attribute, nks((NKS_ENUM)attribute));
              }
              vertexAttributeState[attribute] = state;
          }

      }

  private:

      bool compileShader(GLuint *shader, GLenum type, string &shaderSource);

      void calculateCommonVertexVaryings();

      vector <string> uniformNames();

      void writeShaderStrings();

      bool linkProgram(GLuint prog);

      bool validateProgram(GLuint prog);
  };


}
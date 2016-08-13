//
//  ShaderVariable.h
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#ifndef __ShaderTools__
#define __ShaderTools__

#pragma mark - DICTIONARY KEYS

#include "glState.h"

#define NKS_STRING_CONST static std::string const

#if TARGET_OS_IPHONE
NKS_STRING_CONST NKS_GLSL_VERSION = "";// \n";
#else
NKS_STRING_CONST NKS_GLSL_VERSION = "";// \n";
#endif

#ifdef NK_USE_ARB_EXT
#define glUniform1f glUniform1fARB
#define glUniform1fv glUniform1fvARB
#define glUniform2fv glUniform2fvARB
#define glUniform3fv glUniform3fvARB
#define glUniform4fv glUniform4fvARB
#define glUniformMatrix3fv glUniformMatrix3fvARB
#define glUniformMatrix4fv glUniformMatrix4fvARB
#endif


#if TARGET_EMSCRIPTEN
  // NOT SUPPORTED
#elif TARGET_RASPBERRY_PI
  // NOT SUPPORTED
#elif TARGET_OS_IPHONE
# define nkDrawArraysInstanced glDrawArraysInstancedEXT
#else
# ifndef glDrawArraysInstanced
#   define nkDrawArraysInstanced glDrawArraysInstancedARB
# else
#   define nkDrawArraysInstanced glDrawArraysInstancedARB
# endif
#endif

#if NK_USE_GLES
#define MAX_LIGHTS 2
#else
#define MAX_LIGHTS 4
#endif
// SHADER DICT KEYS

#pragma mark - SHADER MACROS

#define STRINGIZE(x) #x
#define STRINGIZE2(x) STRINGIZE(x)
#define SHADER_STRING(text) STRINGIZE2(text)

#define nksa(a,b) ShaderVariable(NKS_VARIABLE_ATTRIBUTE, NKS_PRECISION_NONE, a, b)
#define nksu(a,b,c) ShaderVariable(NKS_VARIABLE_UNIFORM,a,b,c)
#define nksua(a,b,c,d) ShaderVariable(NKS_VARIABLE_UNIFORM,a,b,c,d)
#define nksv(a,b,c) ShaderVariable(NKS_VARIABLE_VARYING,a,b,c)
#define nksva(a,b,c,d) ShaderVariable(NKS_VARIABLE_VARYING,a,b,c,d)
#define nksi(a,b,c) [ShaderVariable variableWith:NKS_VARIABLE_INLINE precision:a type:b name:c]

#define nks(a) nksString(a)
#define nksf String::format
#define nksGreater(a,b,c,d) [NSString nksIf:a greaterThan:b trueString:c falseString:d]
#define nksEquals(a,b) [NSString nksEquals:a r:b]

#if TARGET_OSX
#define NKS_TYPE_SAMPLER_CORE_VIDEO NKS_TYPE_SAMPLER_2D_RECT
#else
#define NKS_TYPE_SAMPLER_CORE_VIDEO NKS_TYPE_SAMPLER_2D
#endif

#define nksl(a) shaderLineWithArray(a)

#pragma mark - SHADER ENUMS

namespace Shader {

  using std::vector;
  using std::string;
  using std::shared_ptr;
  using std::make_shared;
  using std::map;

  NKS_STRING_CONST NKS_EXTENSIONS = "extensions";
  NKS_STRING_CONST NKS_UNIFORMS = "uniforms";
  NKS_STRING_CONST NKS_ATTRIBUTES = "attributes";
  NKS_STRING_CONST NKS_VARYINGS = "varyings";
  NKS_STRING_CONST NKS_BATCH_SIZE = "batchSize";

  NKS_STRING_CONST NKS_PROGRAMS = "programs";

  NKS_STRING_CONST NKS_VERT_MODULES = "vertexModules";
  NKS_STRING_CONST NKS_FRAG_MODULES = "fragmentModules";

  NKS_STRING_CONST NKS_VERT_MAIN = "vertMain";
  NKS_STRING_CONST NKS_FRAG_MAIN = "fragMain";

  typedef enum NKS_ENUM {
    // VERTEX ATTRIBUTE POSITIONS ** THESE NEED TO STAY 0-4
      NKS_V4_POSITION,
    NKS_V3_NORMAL,
    NKS_V2_TEXCOORD,
    NKS_V4_COLOR,
    NKS_V3_TANGENT,
    NKS_V3_BINORMAL,
    NKS_F1_BONE_WEIGHT,

    // GL_EXTENSIONS
      NKS_EXT_GPU_SHADER,
    NKS_EXT_DRAW_INSTANCED,

    // GL_SECTIONS
      NKS_VERTEX_SHADER,
    NKS_GEOMETRY_SHADER,
    NKS_FRAGMENT_SHADER,

    // GL VAR TYPES
      NKS_VARIABLE_ATTRIBUTE,
    NKS_VARIABLE_UNIFORM,
    NKS_VARIABLE_VARYING,
    NKS_VARIABLE_INLINE,

    // GL PRECISION
      NKS_PRECISION_NONE,
    NKS_PRECISION_LOW,
    NKS_PRECISION_MEDIUM,
    NKS_PRECISION_HIGH,

    // GL VECTOR TYPES
      NKS_NULL,
    NKS_TYPE_F1,
    NKS_TYPE_V2,
    NKS_TYPE_V3,
    NKS_TYPE_V4,
    NKS_TYPE_M9,
    NKS_TYPE_M16,
    NKS_TYPE_UB4,
    NKS_TYPE_INT,
    NKS_TYPE_BOOL,
    NKS_TYPE_SAMPLER_2D,
    NKS_TYPE_SAMPLER_2D_RECT,

    NKS_STRUCT_LIGHT,
    NKS_STRUCT_MATERIAL,
    // NK VAR NAMES
    // M16
      NKS_M16_MVP,
    NKS_M16_MV,
    NKS_M16_P,

    // M9
      NKS_M9_NORMAL,

    // F1
      NKS_F1_BATCH_SCALE,

    // V2
      NKS_V2_BATCH_POSITION,

    // V3
      NKS_V3_BATCH_POSITION,
    NKS_V3_EYE_DIRECTION,
    // V4
      NKS_V4_FINAL_COLOR,

    // SAMPLER
      NKS_S2D_TEXTURE,
    NKS_TEXTURE_RECT_SCALE,
    NKS_SIZE,

    // LIGHT
      NKS_LIGHT,
    NKS_I1_NUM_LIGHTS,
    NKS_V3_LIGHT_DIRECTION,
    NKS_V3_LIGHT_HALF_VECTOR,
    NKS_F1_ATTENUATION,
    NKS_F1_SPECULAR,
    NKS_F1_DIFFUSE,

    // GL
      NKS_V3_GL_POSITION,
    NKS_V4_GL_FRAG_COLOR,
    NKS_UINT_GL_INSTANCE_ID,
    NKS_F1_GL_LINEWIDTH,
    NKS_F1_GL_POINTSIZE,

    // INT
      NKS_INT_NUM_TEXTURES,
    NKS_F1_INSTANCE,
    NKS_INT_CURRENT_PASS,
    NKS_INT_NUM_PASSES,

    // BOOL
      NKS_INT_USE_UNIFORM_COLOR,

    // DEFAULT SHADERS
      NKS_PASSTHROUGH_TEXTURE_SHADER,
    NKS_PASSTHROUGH_UNIFORM_SHADER,
    // MODULES

    NKS_FALSE_COLOR_DARK_COLOR,
    NKS_FALSE_COLOR_LIGHT_COLOR,
    NKS_FALSE_COLOR_INTENSITY,

    // TIME
      NKS_CURRENT_TIME,

    // FUNCTIONS

    NKS_TYPE_VOID = NKS_NULL

  } NKS_ENUM;


  typedef enum NKS_COLOR_MODE {
    NKS_COLOR_MODE_NONE,
    NKS_COLOR_MODE_UNIFORM,
    NKS_COLOR_MODE_VERTEX
  } NKS_COLOR_MODE;

  class ShaderVariable;

// TEXTURE

  std::string textureFragmentFunction(std::map<std::string, std::string> shaderDict);

// UTILS

  std::string shaderStringWithDirective(std::string name, std::string directive);

  std::string nksString(NKS_ENUM string);

  std::string shaderStringWithArray(std::vector<std::string> array);

  std::string shaderLineWithArray(std::vector<std::string> array);

  std::string operatorString(std::vector<ShaderVariable> variables, std::string op);

  typedef struct LightProperties {
    V3t position;
    V3t color;

    V3t halfVector;
    V3t coneDirection;

    F1t spotCosCutoff;
    F1t spotExponent;
    F1t ambient;
    F1t linearAttenuation;
    F1t quadraticAttenuation;

    bool isLocal;
    bool isSpot;
  } LightProperties;

  struct NKMaterialProperties {
    V3t emission;
    V3t ambient;
    V3t diffuse;
    V3t specular;
    float shininess;
  };

#pragma mark - VARIABLE OBJECTS

  union _NKS_SCALAR {
    M16t M16;
    struct {
      M9t M9;
      V3t V3;
      V4t V4;
    };
    struct {
      F1t theFloat;
      I1t I1[4];
      U1t U1[44];
    };
  };

  typedef union _NKS_SCALAR NKS_SCALAR;

#pragma mark - NKSHADER FUNCTION

  class ShaderFunction {

  public:

    std::string name;

    NKS_ENUM inputType;
    NKS_ENUM returnType;

    std::string constants;
    std::string function;

    std::string functionString();

  };

#pragma mark - NKSHADER VARIABLE

  class Program;

  class ShaderVariable {

  public:

    NKS_ENUM type;
    NKS_ENUM precision;
    NKS_ENUM variable;
    NKS_ENUM name;

    GLuint arraySize{0};
    GLuint glLocation{0};

    Program *program{nullptr};

    ShaderVariable() { }

    ShaderVariable(NKS_ENUM variable_, NKS_ENUM precision_, NKS_ENUM type_, NKS_ENUM name_, GLuint arraySize_ = 0) {
      variable = variable_;
      precision = precision_;
      type = type_;
      name = name_;
      arraySize = arraySize_;
    };

    bool operator==(const ShaderVariable &other) const {
      return other.name == name;
    }

    std::string nameString();

    std::string description();

    std::string declarationStringForSection(NKS_ENUM section);

    // BINDING UNIFORMS

    void bindI1(int data);

    template<typename T>
    void bindU1(T data) {
      glUniform1i(glLocation, static_cast<GLint>(data));
      nkGetGLError();
    }

    void bindF1(F1t data);

    void bindV2(V2t data);

    void bindV3(V3t data);

    void bindV4(V4t data);

    void bindM9(M9t data);

    void bindM16(M16t data);

    void bindF1Array(F1t *data, int count);

    void bindV2Array(V2t *data, int count);

    void bindV3Array(V3t *data, int count);

    void bindV4Array(V4t *data, int count);

    void bindM9Array(M9t *data, int count);

    void bindM16Array(M16t *data, int count);

    void bindLightProperties(LightProperties &data, int count);

    // GENERIC UNION FOR OTHER TYPES
    void pushValue(NKS_SCALAR value);

  };

}

#endif


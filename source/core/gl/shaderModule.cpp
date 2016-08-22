//
//  NKFragModule.m
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 7/12/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#include "shaderModule.h"
#include "shaderTools.h"
#include "shaderProgram.h"
#include "../util/util.h"

namespace Shader {
  ShaderModule ShaderModule::vertexPositionModuleDefault() {
      ShaderModule module;
      module.name = "vertex position module";
      return module;
  }

  ShaderModule ShaderModule::batchSpritePositionModule(int batchSize = 0) {
      ShaderModule module;
      module.name = "batch position module";
      module.uniforms.push_back(nksua(NKS_PRECISION_HIGH, NKS_TYPE_V2, NKS_V2_BATCH_POSITION, batchSize));
      module.uniforms.push_back(nksua(NKS_PRECISION_HIGH, NKS_TYPE_F1, NKS_F1_BATCH_SCALE, batchSize));
      return module;
  }

  ShaderModule ShaderModule::colorModule(NKS_COLOR_MODE colorMode, int batchSize = 0) {

      ShaderModule module;

      module.name = "vertex color module";

      module.colorMode = colorMode;

      if (colorMode == NKS_COLOR_MODE_NONE) {
          return module;
      }

      module.varyings.push_back(nksv(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_V4_COLOR));

      if (colorMode == NKS_COLOR_MODE_UNIFORM) {
          module.uniforms.push_back(nksua(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_V4_COLOR, batchSize));
      }

      if (colorMode == NKS_COLOR_MODE_UNIFORM) {
          if (batchSize) {
#if (NK_USE_GLES && !NK_USE_WEBGL)
            module.vertexMain.push_back("v_color = u_color[gl_InstanceIDEXT];");
#else
              module.vertexMain.push_back("v_color = u_color[gl_InstanceID];");
#endif
          }
          else {
              module.vertexMain.push_back("v_color = u_color;");
          }
      }
      else if (colorMode == NKS_COLOR_MODE_VERTEX) {

          module.vertexMain.push_back("v_color = a_color;");
      }

      ShaderFunction colorFunction;

      colorFunction.name = "nkColorFunc";
      colorFunction.inputType = NKS_TYPE_V4;
      colorFunction.returnType = NKS_TYPE_V4;

      colorFunction.function = SHADER_STRING
      (
        return inputColor * v_color;
      );

      module.fragFunctions.push_back(colorFunction);

      return module;
  }

//+(ShaderModule*) materialModule:(int)numTex {
//
//    ShaderModule* module = [[ShaderModule alloc] init];
//
//    module.name = @"texture module";
//
//    module.types = SHADER_STRING
//    (
//     struct MaterialProperties {
//         vec3 emission;
//         vec3 ambient;
//         vec3 diffuse;
//         vec3 specular;
//         float shininess;
//     }
//     );
//
//    return module;
//}

  ShaderModule ShaderModule::textureModule(int numTex) {

      ShaderModule module;

      module.name = "texture module";
      module.numTexures = numTex;

      if (numTex == -1) { // quick vid fix
          module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_SAMPLER_CORE_VIDEO, NKS_S2D_TEXTURE));
          module.uniforms.push_back(nksu(NKS_PRECISION_MEDIUM, NKS_TYPE_V2, NKS_TEXTURE_RECT_SCALE));
          module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_V2, NKS_V2_TEXCOORD));

          if (module.uniformNamed(NKS_S2D_TEXTURE).type == NKS_TYPE_SAMPLER_2D) {
              module.vertexMain.push_back("v_texCoord0 = vec2(a_texCoord0.x, 1. - a_texCoord0.y);");
          }
          else {
              module.vertexMain.push_back("v_texCoord0 = vec2(a_texCoord0.x, 1. - a_texCoord0.y) * u_textureScale;");
          }

      }

      else if (numTex) {
          module.uniforms.push_back(nksu(NKS_PRECISION_MEDIUM, NKS_TYPE_SAMPLER_2D, NKS_S2D_TEXTURE));
          module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_V2, NKS_V2_TEXCOORD));

          module.vertexMain.push_back("v_texCoord0 = a_texCoord0;");
      }

      ShaderFunction texFunction;

      texFunction.name = "nkTexFunc";
      texFunction.inputType = NKS_TYPE_V4;
      texFunction.returnType = NKS_TYPE_V4;

      if (module.hasUniformNamed(NKS_S2D_TEXTURE)) {
#if NK_USE_GL3
        texFunction.function = "return inputColor * texture(u_texture,v_texCoord0);";
#else
          if (module.uniformNamed(NKS_S2D_TEXTURE).type == NKS_TYPE_SAMPLER_2D_RECT) {
              texFunction.function = "vec4 textureColor = texture2DRect(u_texture,v_texCoord0); if (textureColor.a < .1) discard; return textureColor * inputColor;";
          }
          else {
              texFunction.function = "vec4 textureColor = texture2D(u_texture,v_texCoord0); if (textureColor.a < .1) discard; return textureColor * inputColor;";
          }
#endif
      }

      module.fragFunctions.push_back(texFunction);

      return module;

  }

  ShaderModule ShaderModule::lightModule(bool highQuality, int batchSize) {

      ShaderModule module;

#if NK_USE_GLES
    module.types =
    "struct LightProperties {\
    mediump vec3 position;\
    lowp vec3 color;\
    lowp vec3 halfVector;\
    lowp vec3 coneDirection;\
    float spotCosCutoff;\
    float spotExponent;\
    float ambient;\
    float linearAttenuation;\
    float quadraticAttenuation;\
    bool isLocal;\
    bool isSpot;\
    };";
#else
      module.types =
        "struct LightProperties {\
    vec3 position;\
    vec3 color;\
    vec3 halfVector;\
    vec3 coneDirection;\
    float spotCosCutoff;\
    float spotExponent;\
    float ambient;\
    float linearAttenuation;\
    float quadraticAttenuation;\
    bool isLocal;\
    bool isSpot;\
    };";
#endif

      module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_INT, NKS_I1_NUM_LIGHTS));
      module.uniforms.push_back(nksu(NKS_PRECISION_NONE, NKS_STRUCT_LIGHT, NKS_LIGHT));

      module.uniforms.push_back(nksua(NKS_PRECISION_HIGH, NKS_TYPE_M16, NKS_M16_MV, batchSize));
      module.uniforms.push_back(nksua(NKS_PRECISION_MEDIUM, NKS_TYPE_M9, NKS_M9_NORMAL, batchSize));

      module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_V3, NKS_V3_NORMAL));
      module.varyings.push_back(nksv(NKS_PRECISION_HIGH, NKS_TYPE_V4, NKS_V4_POSITION));

      if (highQuality) {
          module.varyings.push_back(nksv(NKS_PRECISION_HIGH, NKS_TYPE_V3, NKS_V3_LIGHT_HALF_VECTOR));
          module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_F1, NKS_F1_ATTENUATION));
          module.varyings.push_back(nksv(NKS_PRECISION_HIGH, NKS_TYPE_V3, NKS_V3_LIGHT_DIRECTION));
      }
      else {
          module.varyings.push_back(nksva(NKS_PRECISION_HIGH, NKS_TYPE_F1, NKS_F1_SPECULAR, MAX_LIGHTS));
          module.varyings.push_back(nksva(NKS_PRECISION_HIGH, NKS_TYPE_F1, NKS_F1_DIFFUSE, MAX_LIGHTS));
          module.varyings.push_back(nksva(NKS_PRECISION_MEDIUM, NKS_TYPE_F1, NKS_F1_ATTENUATION, MAX_LIGHTS));
      }


      ShaderFunction lightFunction;

      lightFunction.name = "nkLightFunc";
      lightFunction.inputType = NKS_TYPE_V4;
      lightFunction.returnType = NKS_TYPE_V4;

      if (highQuality) {

          module.name = "HQ LIGHT PROGRAM";

          lightFunction.function = SHADER_STRING
          (
          // HQ LIGHT MODULE

            vec3 scatteredLight = vec3(0.0);
            vec3 reflectedLight = vec3(0.0);

            float Strength = 2.0;
            float Shininess = 16.0;

            if (u_numLights > 0) {
                for (int i = 0; i < MAX_LIGHTS; i++) {
                    if (i < u_numLights) {
                        vec3 halfVector;

                        vec3 lightDirection = u_light[i].position - v_position.xyz;

                        float attenuation = 1.0;

                        if (u_light[i].isLocal) { //

                            float lightDistance = length(lightDirection);

                            lightDirection = lightDirection / lightDistance; // = normalize( lightDir );

                            attenuation = 1.0 / (1.0 + (u_light[i].linearAttenuation * lightDistance) +
                                                 (u_light[i].quadraticAttenuation * lightDistance * lightDistance));

                            if (u_light[i].isSpot) {
                                float spotCos = dot(lightDirection, -u_light[i].coneDirection);
                                if (spotCos < u_light[i].spotCosCutoff) attenuation = 0.0;
                                else attenuation *= pow(spotCos, u_light[i].spotExponent);
                            }

                            halfVector = normalize(lightDirection - v_position.xyz); // +eye
                        }
                        else {
                            halfVector = u_light[i].halfVector;
                        }

                        //Intensity of the diffuse light. Saturate to keep within the 0-1 range.
                        float nDotL = dot(v_normal, lightDirection);
                        float diffuse = clamp(nDotL, 0.0, 1.0);

                        //vec3 reflectDir = reflect(-lightDirection, v_normal);

                        float nDotH = dot(v_normal, halfVector);
                        float specular = pow(clamp(nDotH, 0.0, 1.0), Shininess) * Strength;

                        //float specular = specular * (ambient + diffuse)

                        // Accumulate all the u_lights’ effects
                        scatteredLight += u_light[i].color * (attenuation * diffuse + u_light[i].ambient);
                        reflectedLight += u_light[i].color * (attenuation * specular * diffuse);
                    }
                }
                return vec4(scatteredLight + reflectedLight, 1.0) * inputColor;
            }
            return inputColor;
          );

      }

          // LOW QUALITY
      else {

          module.name = "LQ LIGHT PROGRAM";

          module.vertexMain.push_back(SHADER_STRING
                                      (
                                        float Strength = 2.;
                                        float Shininess = 16.;

                                        if (u_numLights > 0) {
                                            for (int i = 0; i < MAX_LIGHTS; i++) {

                                                if (i < u_numLights) {

                                                    vec3 halfVector;
                                                    vec3 lightDirection = u_light[i].position - v_position.xyz;

                                                    if (u_light[i].isLocal) { //

                                                        float lightDistance = length(lightDirection);

                                                        lightDirection = lightDirection / lightDistance;

                                                        v_attenuation[i] = 1.0 / (1.0 + (u_light[i].linearAttenuation *
                                                                                         lightDistance) +
                                                                                  (u_light[i].quadraticAttenuation *
                                                                                   lightDistance * lightDistance));

                                                        if (u_light[i].isSpot) {
                                                            float spotCos = dot(lightDirection,
                                                                                -u_light[i].coneDirection);
                                                            if (spotCos < u_light[i].spotCosCutoff)
                                                                v_attenuation[i] = 0.0;
                                                            else
                                                                v_attenuation[i] *= pow(spotCos,
                                                                                        u_light[i].spotExponent);
                                                        }

                                                        halfVector = normalize(lightDirection - v_position.xyz);
                                                    }
                                                    else {
                                                        v_attenuation[i] = 1.0;
                                                        halfVector = u_light[i].halfVector;
                                                    }

                                                    //Intensity of the diffuse light. Saturate to keep within the 0-1 range.
                                                    float nDotL = dot(v_normal, lightDirection);
                                                    v_diffuse[i] = clamp(nDotL, 0.0, 1.0);

                                                    float nDotH = dot(v_normal, halfVector);
                                                    v_specular[i] = pow(clamp(nDotH, 0.0, 1.0), Shininess) * Strength;
                                                }
                                            }
                                        }
                                      ));

          lightFunction.function = SHADER_STRING
          (
          // LQ LIGHT PROGRAM

            vec3 scatteredLight = vec3(0.0);
            vec3 reflectedLight = vec3(0.0);

            if (u_numLights > 0) {
                for (int i = 0; i < MAX_LIGHTS; i++) {
                    if (i < u_numLights) {
                        // Accumulate all the u_lights’ effects
                        scatteredLight += u_light[i].color * (v_attenuation[i] * v_diffuse[i] + u_light[i].ambient);
                        reflectedLight += u_light[i].color * (v_attenuation[i] * v_specular[i] * v_diffuse[i]);
                    }
                }
                return vec4(scatteredLight + reflectedLight, 1.0) * inputColor;
            }
            else {
                return inputColor;
            }

          );
      }

      module.fragFunctions = {lightFunction};

      return module;

  }

  ShaderModule ShaderModule::freetypeModule() {

      ShaderModule module;

#if NK_USE_GLES
    module.extensions.push_back("#extension GL_OES_standard_derivatives : enable");
#endif
      module.name = "FreeType-GL";

      module.uniforms.push_back(nksu(NKS_PRECISION_MEDIUM, NKS_TYPE_SAMPLER_2D, NKS_S2D_TEXTURE));
      module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_V2, NKS_V2_TEXCOORD));

      module.vertexMain.push_back("v_texCoord0 = a_texCoord0; \n");

      ShaderFunction texFunction;

      texFunction.name = "textureMultiplyAlpha";

      texFunction.inputType = NKS_TYPE_V4;
      texFunction.returnType = NKS_TYPE_V4;

#if NK_USE_GL3
    texFunction.function = "return inputColor * texture(u_texture,v_texCoord0);";
#else
      if (module.uniformNamed(NKS_S2D_TEXTURE).type == NKS_TYPE_SAMPLER_2D_RECT) {
          texFunction.function = "vec4 textureColor = texture2DRect(u_texture,v_texCoord0); if (textureColor.a < .1) discard; return textureColor * inputColor;";
      }
      else {
          texFunction.function = SHADER_STRING
          (
            float tcolor = texture2D(u_texture, v_texCoord0.xy).r;
            if (tcolor < .1) discard;
            float dist = tcolor;
            float width = fwidth(dist);
            float alpha = smoothstep(0.5 - width, 0.5 + width, dist);
            return vec4(inputColor.rgb, inputColor.a * alpha * tcolor);
          );
      }
#endif

      module.fragFunctions.push_back(texFunction);

      return module;

  }

#pragma mark - POST PROCESS MODULES

  ShaderModule ShaderModule::falseColorModule(F1t intensity, Color darkColor, Color lightColor) {

      ShaderModule module;

      module.name = "FALSE COLOR";

      module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_DARK_COLOR));
      module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_LIGHT_COLOR));
      module.uniforms.push_back(nksu(NKS_PRECISION_MEDIUM, NKS_TYPE_F1, NKS_FALSE_COLOR_INTENSITY));


      ShaderFunction falseColor;

      falseColor.name = "nkFalseColor";
      falseColor.inputType = NKS_TYPE_V4;
      falseColor.returnType = NKS_TYPE_V4;

      falseColor.constants = SHADER_STRING
      (
        const vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);
      );

      falseColor.function = SHADER_STRING
      (
        if (inputColor.a < .1) discard;
        float luminance = dot(inputColor.rgb, luminanceWeighting);
        return mix(inputColor,
                   vec4(mix(u_falseColor_darkColor.rgb, u_falseColor_lightColor.rgb, luminance), inputColor.a),
                   u_falseColor_intensity);
      );

      module.fragFunctions = {falseColor};

      module.uniformUpdateBlock = [intensity, darkColor, lightColor](ShaderModule &module) {
        module.uniformNamed(NKS_FALSE_COLOR_DARK_COLOR).bindV4(darkColor);
        module.uniformNamed(NKS_FALSE_COLOR_LIGHT_COLOR).bindV4(lightColor);
        module.uniformNamed(NKS_FALSE_COLOR_INTENSITY).bindF1(intensity);
      };

      return module;

  }


  ShaderModule ShaderModule::swirl(F1t intensity, V2t size) {

      ShaderModule module;

      module.name = "SWIRL";

      module.uniforms.push_back(nksu(NKS_PRECISION_HIGH, NKS_TYPE_F1, NKS_CURRENT_TIME));
      module.uniforms.push_back(nksu(NKS_PRECISION_HIGH, NKS_TYPE_V2, NKS_SIZE));
//    module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_DARK_COLOR));
//    module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_LIGHT_COLOR));
//    module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_F1, NKS_FALSE_COLOR_INTENSITY));

      module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_V2, NKS_V2_TEXCOORD));

      module.vertexMain.push_back("v_texCoord0 = a_texCoord0;");

      ShaderFunction swirl;

      swirl.name = "swirl";
      swirl.inputType = NKS_TYPE_V4;
      swirl.returnType = NKS_TYPE_V4;

      swirl.function = SHADER_STRING
      (
      //if (inputColor.a < .1) discard;

      //float time = u_time * .5;
        vec2 sp = v_texCoord0; // / u_size.xy; //gl_FragCoord.xy / u_size.xy; // v_texCoord0 ?
        vec2 p = sp * u_size - 20.0;
        vec2 i = p;
        float c = 1.0;
        float inten = .05;

        for (int n = 0; n < MAX_LIGHTS; n++) {
            float t = u_time * (1.0 - (3.5 / float(n + 1)));
            i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
            c += 1.0 / length(vec2(p.x / (sin(i.x + t) / inten), p.y / (cos(i.y + t) / inten)));
        }

        c /= float(MAX_LIGHTS);
        c = 1.55 - sqrt(c);
        vec3 colour = vec3(pow(abs(c), 15.0));

        return inputColor += vec4(clamp(colour + vec3(0.0, 0.17, 0.3), 0.0, 1.0), 0.0);

      );

      module.fragFunctions = {swirl};

      float time = Time::getCurrentTime();

      module.uniformUpdateBlock = [time, size](ShaderModule &module) {
        module.uniformNamed(NKS_CURRENT_TIME).bindF1((Time::getCurrentTime() - time));
        module.uniformNamed(NKS_SIZE).bindV2(size);
      };

      return module;

  }

  bool ShaderModule::hasUniformNamed(NKS_ENUM name) {
      for (auto &v : uniforms) {
          if (v.name == name) return true;
      }
      return false;
  }

  ShaderVariable &ShaderModule::uniformNamed(NKS_ENUM name) {
      for (auto &v : uniforms) {
          if (v.name == name) return v;
      }
      return uniforms[0]; // this will blow up if empty
  }

  bool ShaderModule::hasVaryingNamed(NKS_ENUM name) {
      for (auto &v : uniforms) {
          if (v.name == name) return true;
      }
      return false;
  }

  ShaderVariable &ShaderModule::varyingNamed(NKS_ENUM name) {
      for (auto &v : varyings) {
          if (v.name == name) return v;
      }
      return varyings[0]; // this will blow up if empty
  }

}

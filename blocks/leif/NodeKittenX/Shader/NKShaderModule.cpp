//
//  NKFragModule.m
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 7/12/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#include "NKShaderModule.h"
#include "NKShaderTools.h"
#include "NKShaderProgram.h"
#include "NKTime.h"

NKShaderModule NKShaderModule::colorModule(NKS_COLOR_MODE colorMode, int batchSize){
    
    NKShaderModule module;
    
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
    
    NKShaderFunction colorFunction;
    
    colorFunction.name = "nkColorFunc";
    colorFunction.inputType = NKS_TYPE_V4;
    colorFunction.returnType = NKS_TYPE_V4;
    
    colorFunction.function = SHADER_STRING
    (
     return inputColor * v_color;
     );
    
    module.fragFunctions.push_back(colorFunction);
    
    // module.outputColor = nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_V4, NKS_V4_COLOR);
    
    return module;
}

//+(NKShaderModule*) materialModule:(int)numTex {
//
//    NKShaderModule* module = [[NKShaderModule alloc] init];
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



NKShaderModule NKShaderModule::textureModule(int numTex) {
    
    NKShaderModule module;
    
    module.name = "texture module";
    module.numTexures = numTex;
    
    if (numTex == -1){ // quick vid fix
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
    
    NKShaderFunction texFunction;
    
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

NKShaderModule NKShaderModule::lightModule(bool highQuality, int batchSize){
    
    NKShaderModule module;
    
#if NK_USE_GLES
    module.types =
    "struct LightProperties {\
    \n\
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
    \n\
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
    
    //    precision mediump float;
    //
    //    varying vec3 normalInterp;
    //    varying vec3 vertPos;
    //
    //    uniform int mode;
    //
    //    const vec3 lightPos = vec3(1.0,1.0,1.0);
    //    const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
    //    const vec3 specColor = vec3(1.0, 1.0, 1.0);
    //
    //    void main() {
    //
    //        vec3 normal = normalize(normalInterp);
    //        vec3 lightDir = normalize(lightPos - vertPos);
    //
    //        float lambertian = max(dot(lightDir,normal), 0.0);
    //        float specular = 0.0;
    //
    //        if(lambertian > 0.0) {
    //
    //            vec3 reflectDir = reflect(-lightDir, normal);
    //            vec3 viewDir = normalize(-vertPos);
    //
    //            float specAngle = max(dot(reflectDir, viewDir), 0.0);
    //            specular = pow(specAngle, 4.0);
    //
    //            // the exponent controls the shininess (try mode 2)
    //            if(mode == 2)  specular = pow(specAngle, 16.0);
    //
    //            // according to the rendering equation we would need to multiply
    //            // with the the "lambertian", but this has little visual effect
    //            if(mode == 3) specular *= lambertian;
    //
    //            // switch to mode 4 to turn off the specular component
    //            if(mode == 4) specular *= 0.0;
    //
    //        }
    //
    //        gl_FragColor = vec4( lambertian*diffuseColor +
    //                            specular*specColor, 1.0);
    //    }
    
    
    module.uniforms.push_back(nksu(NKS_PRECISION_NONE, NKS_TYPE_INT, NKS_I1_NUM_LIGHTS));
    module.uniforms.push_back(nksu(NKS_PRECISION_NONE, NKS_STRUCT_LIGHT, NKS_LIGHT));
    
    module.uniforms.push_back(nksua(NKS_PRECISION_HIGH, NKS_TYPE_M16, NKS_M16_MV, batchSize));
    module.uniforms.push_back(nksua(NKS_PRECISION_MEDIUM, NKS_TYPE_M9, NKS_M9_NORMAL, batchSize));
    
    module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_V3, NKS_V3_NORMAL));
    module.varyings.push_back(nksv(NKS_PRECISION_HIGH, NKS_TYPE_V3, NKS_V3_EYE_DIRECTION));
    
    module.varyings.push_back(nksv(NKS_PRECISION_HIGH, NKS_TYPE_V3, NKS_V3_LIGHT_HALF_VECTOR));
    module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_F1, NKS_F1_ATTENUATION));
    
    module.varyings.push_back(nksv(NKS_PRECISION_HIGH, NKS_TYPE_V4, NKS_V4_POSITION));
    module.varyings.push_back(nksv(NKS_PRECISION_HIGH, NKS_TYPE_V3, NKS_V3_LIGHT_DIRECTION));
    
    //module.outputColor = nksi(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_V4_LIGHT_COLOR);
    
    NKShaderFunction lightFunction;
    
    lightFunction.name = "nkLightFunc";
    lightFunction.inputType = NKS_TYPE_V4;
    lightFunction.returnType = NKS_TYPE_V4;
    
    if (highQuality) {
        
        module.name = "HQ LIGHT PROGRAM";

        module.vertexMain.push_back("v_eyeDirection = -v_position.xyz;");
        
        lightFunction.function = SHADER_STRING
        (
         // HQ LIGHT MODULE
         
         vec3 scatteredLight = vec3(0.0);
         vec3 reflectedLight = vec3(0.0);
         
         float Strength = 2.0;
         float Shininess = 16.0;
         
         if (u_numLights > 0){
             for (int i = 0; i < 4; i++){
                 if (i < u_numLights) {
                     vec3 halfVector;
                     
                     vec3 lightDirection = u_light[i].position - v_position.xyz;
                     
                     float attenuation = 1.0;
                     
                     if (u_light[i].isLocal) { //
                         
                         float lightDistance = length(lightDirection);
                         
                         lightDirection = lightDirection / lightDistance; // = normalize( lightDir );
                         
                         attenuation = 1.0 / (1.0 + (u_light[i].linearAttenuation * lightDistance) + (u_light[i].quadraticAttenuation * lightDistance * lightDistance));
                         
                         if (u_light[i].isSpot) {
                             float spotCos = dot(lightDirection,-u_light[i].coneDirection);
                             if (spotCos < u_light[i].spotCosCutoff) attenuation = 0.0;
                             else attenuation *= pow(spotCos,u_light[i].spotExponent);
                         }
                         
                         halfVector = normalize(lightDirection + v_eyeDirection);
                     }
                     else {
                         halfVector = u_light[i].halfVector;
                     }
                     
                     //Intensity of the diffuse light. Saturate to keep within the 0-1 range.
                     float nDotL = dot(v_normal, lightDirection);
                     float diffuse = clamp(nDotL, 0.0, 1.0);
                     
                     //vec3 reflectDir = reflect(-lightDirection, v_normal);
                     
                     float nDotH = dot(v_normal, halfVector);
                     float specular = pow (clamp(nDotH, 0.0, 1.0), Shininess) * Strength;
                     
                     //float specular = specular * (ambient + diffuse)
                     
                     // Accumulate all the u_lights’ effects
                     scatteredLight += u_light[i].color * (attenuation * diffuse + u_light[i].ambient);
                     reflectedLight += u_light[i].color * (attenuation * specular * diffuse + u_light[i].ambient);
                 }
             }
             return vec4(scatteredLight + reflectedLight, 1.0) * inputColor;
         }
         else {
             return inputColor;
         }
         );
        
    }
    
    // LOW QUALITY
    else {
        
        module.name = "LQ LIGHT PROGRAM";
        
        module.vertexMain.push_back("v_eyeDirection = -v_position.xyz;");
        module.vertexMain.push_back(SHADER_STRING
                                    (
                                     if (u_light.isLocal) {
                                         
                                         v_lightDirection =  u_light.position - v_position.xyz;
                                         float lightDistance = length(v_lightDirection);
                                         v_lightDirection = v_lightDirection / lightDistance; // = normalize( lightDir );
                                         v_attenuation = 1.0 / (1.0 + (u_light.linearAttenuation * lightDistance) + (u_light.quadraticAttenuation * lightDistance * lightDistance));
                                         if (u_light.isSpot) {
                                             float spotCos = dot(v_lightDirection,-u_light.coneDirection);
                                             if (spotCos < u_light.spotCosCutoff) v_attenuation = 0.0;
                                             else v_attenuation *= pow(spotCos,u_light.spotExponent);
                                         }
                                         v_halfVector = normalize(v_lightDirection + v_eyeDirection);
                                         
                                     }
                                     else {
                                         v_halfVector = u_light.halfVector;
                                     }
                                     ));
        
        lightFunction.function = SHADER_STRING
        (
         // LQ LIGHT PROGRAM
         
         vec3 scatteredLight = vec3(0.0);
         vec3 reflectedLight = vec3(0.0);
         
         float Strength = 2.;
         float Shininess = 16.;
         
         if (u_numLights > 0){
             for (int i = 0; i < 4; i++){
                 if (i < u_numLights) {
                     //Intensity of the diffuse light. Saturate to keep within the 0-1 range.
                     float nDotL = dot(v_normal, v_lightDirection);
                     float diffuse = clamp(nDotL, 0.0, 1.0);
                     
                     //vec3 reflectDir = reflect(-lightDirection, v_normal);
                     
                     float nDotH = dot(v_normal, v_halfVector);
                     float specular = pow (clamp(nDotH, 0.0, 1.0), Shininess) * Strength;
                     
                     //float specular = specular * (ambient + diffuse)
                     
                     // Accumulate all the u_lights’ effects
                     scatteredLight += u_light.color * (v_attenuation * diffuse + u_light.ambient);
                     reflectedLight += u_light.color * (v_attenuation * specular * diffuse + u_light.ambient);
                     
                 }
             }
             //lightColor = vec4(min(reflectedLight,vec3(1.0)), 1.0);
             return inputColor * vec4(scatteredLight+reflectedLight, 1.0);
         }
         else {
             return inputColor;
         }
         
         );
    }
    
    module.fragFunctions = {lightFunction};
    
    return module;
    
}

#pragma mark - POST PROCESS MODULES

NKShaderModule NKShaderModule::falseColorModule(F1t intensity, NKColor darkColor, NKColor lightColor) {
    
    NKShaderModule module;
    
    module.name = "FALSE COLOR";
    
    module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_DARK_COLOR));
    module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_LIGHT_COLOR));
    module.uniforms.push_back(nksu(NKS_PRECISION_MEDIUM, NKS_TYPE_F1, NKS_FALSE_COLOR_INTENSITY));
    
    
    NKShaderFunction falseColor;
    
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
     return mix(inputColor,vec4( mix(u_falseColor_darkColor.rgb, u_falseColor_lightColor.rgb, luminance), inputColor.a),u_falseColor_intensity);
     );
    
    module.fragFunctions = {falseColor};
    
    module.uniformUpdateBlock = [intensity, darkColor,lightColor](NKShaderModule& module) {
        module.uniformNamed(NKS_FALSE_COLOR_DARK_COLOR).bindV4(darkColor.data);
        module.uniformNamed(NKS_FALSE_COLOR_LIGHT_COLOR).bindV4(lightColor.data);
        module.uniformNamed(NKS_FALSE_COLOR_INTENSITY).bindF1(intensity);
    };
    
    return module;
    
}


NKShaderModule NKShaderModule::swirl(F1t intensity, V2t size) {
    
    NKShaderModule module;
    
    module.name = "SWIRL";
    
    module.uniforms.push_back(nksu(NKS_PRECISION_HIGH, NKS_TYPE_F1, NKS_CURRENT_TIME));
    module.uniforms.push_back(nksu(NKS_PRECISION_HIGH, NKS_TYPE_V2, NKS_SIZE));
//    module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_DARK_COLOR));
//    module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_LIGHT_COLOR));
//    module.uniforms.push_back(nksu(NKS_PRECISION_LOW, NKS_TYPE_F1, NKS_FALSE_COLOR_INTENSITY));
    
    module.varyings.push_back(nksv(NKS_PRECISION_MEDIUM, NKS_TYPE_V2, NKS_V2_TEXCOORD));
    
    module.vertexMain.push_back("v_texCoord0 = a_texCoord0;");
    
    NKShaderFunction swirl;
    
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
     
     for (int n = 0; n < 5; n++)
     {
         float t = u_time * (1.0 - (3.5 / float(n+1)));
         i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
         c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
     }
     
     c /= float(5.0);
     c = 1.55-sqrt(c);
     vec3 colour = vec3(pow(abs(c), 15.0));
     
     return inputColor += vec4(clamp(colour + vec3(0.0, 0.17, 0.3), 0.0, 1.0), 0.0);
     
     );
    
    module.fragFunctions = {swirl};
    
    float time = Time::getCurrentTime();
    
    module.uniformUpdateBlock = [time, size](NKShaderModule& module) {
        module.uniformNamed(NKS_CURRENT_TIME).bindF1((Time::getCurrentTime()-time));
        module.uniformNamed(NKS_SIZE).bindV2(size);
    };
    
    return module;
    
}

#warning DO THESE

//

//
//+(NKShaderModule*) noiseModule:(V2t)time {
//
//    NKShaderModule* module = [[NKShaderModule alloc] init];
//
//    module.name = @"NAME";
//
//    [module.uniforms addObject:nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_DARK_COLOR)];
//
//    NKShaderFunction *function = [[NKShaderFunction alloc]init];
//
//    function.name = @"functionName";
//    function.inputType = NKS_TYPE_V4;
//    function.returnType = NKS_TYPE_V4;
//
//    function.constants = SHADER_STRING
//    (
//
//     float timeValX;
//     float timeValY;
//
//     //generate a random value from four points
//     vec4 rand(vec2 A,vec2 B,vec2 C,vec2 D){
//
//         vec2 s=vec2(12.9898,78.233);
//         vec4 tmp=vec4(dot(A,s),dot(B,s),dot(C,s),dot(D,s));
//
//         return fract(sin(tmp) * 43758.5453)* 2.0 - 1.0;
//     }
//
//     //this is similar to a perlin noise function
//     float noise(vec2 coord,float d){
//
//         vec2 C[4];
//
//         float d1 = 1.0/d;
//
//         C[0]=floor(coord*d)*d1;
//
//         C[1]=C[0]+vec2(d1,0.0);
//
//         C[2]=C[0]+vec2(d1,d1);
//
//         C[3]=C[0]+vec2(0.0,d1);
//
//
//         vec2 p=fract(coord*d);
//
//         vec2 q=1.0-p;
//
//         vec4 w=vec4(q.x*q.y,p.x*q.y,p.x*p.y,q.x*p.y);
//
//         return dot(vec4(rand(C[0],C[1],C[2],C[3])),w);
//     }
//
//    );
//
//    NSString *vertFunction = SHADER_STRING
//    (
//
//     //generate some noise values based on vertex position and the time value which comes in from our OF app
//     float noiseAmntX = noise( vec2(-timeValX + pos.x / 1000.0, 100.0), 20.0 );
//     float noiseAmntY = noise( vec2(timeValY + pos.y / 1000.0, pos.x / 2000.0), 20.0 );
//
//     //generate noise for our blue pixel value
//     float noiseB = noise( vec2(timeValY * 0.25, pos.y / 2000.0), 20.0 );
//
//     //lets also figure out the distance between the mouse and the vertex and apply a repelling force away from the mouse
//     vec2 d = vec2(pos.x, pos.y) - mouse;
//     float len =  sqrt(d.x*d.x + d.y*d.y);
//     if( len < 300.0 && len > 0.0  ){
//
//         //lets get the distance into 0-1 ranges
//         float pct = len / 300.0;
//
//         //this turns our linear 0-1 value into a curved 0-1 value
//         pct *= pct;
//
//         //flip it so the closer we are the greater the repulsion
//         pct = 1.0 - pct;
//
//         //normalize our repulsion vector
//         d /= len;
//
//         //apply the repulsion to our position
//         pos.x += d.x * pct * 90.0;
//         pos.y += d.y * pct * 90.0;
//     }
//
//     //modify our position with the smooth noise
//     pos.x += noiseAmntX * 20.0;
//     pos.y += noiseAmntY * 10.0;
//
//     //finally set the pos to be that actual position rendered
//     gl_Position = pos;
//
//
//
//     );
//
//    function.function = SHADER_STRING
//    (
//     return inputColor;
//    );
//
//    module.fragFunctions = @[function];
//
//    module.uniformUpdateBlock = newUniformUpdateBlock {
//    };
//
//    return module;
//
//
//
//}
//
//+(NKShaderModule*) boxBlurModule:(F1t)size intNumPasses:(F1t)passes {
//
//    NKShaderModule* module = [[NKShaderModule alloc] init];
//
//    module.name = @"NAME";
//
//    [module.uniforms addObject:nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_DARK_COLOR)];
//
//    NKShaderFunction *function = [[NKShaderFunction alloc]init];
//
//    function.name = @"functionName";
//    function.inputType = NKS_TYPE_V4;
//    function.returnType = NKS_TYPE_V4;
//
//    function.constants = SHADER_STRING
//    (
//    );
//
//    function.function = SHADER_STRING
//    (
//    );
//
//    module.fragFunctions = @[function];
//
//    module.uniformUpdateBlock = newUniformUpdateBlock {
//    };
//
//    return module;
//
//}
//
//+(NKShaderModule*) bezierModule:(F1t)size {
//
//    NKShaderModule* module = [[NKShaderModule alloc] init];
//
//    module.name = @"NAME";
//
//    [module.uniforms addObject:nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_DARK_COLOR)];
//
//    NKShaderFunction *function = [[NKShaderFunction alloc]init];
//
//    function.name = @"functionName";
//    function.inputType = NKS_TYPE_V4;
//    function.returnType = NKS_TYPE_V4;
//
//    function.constants = SHADER_STRING
//    (
//    );
//
//    function.function = SHADER_STRING
//    (
//     // Gradients
//     vec2 px = ddx(v_position);
//     vec2 py = ddy(v_position);
//     // Chain rule
//     float fx = (2*p.x)*px.x - px.y;
//     float fy = (2*p.x)*py.x - py.y;
//     // Signed distance
//     float sd = (p.x*p.x - p.y)/sqrt(fx*fx + fy*fy);
//     // Linear alpha
//     float alpha = 0.5 - sd;
//     if (alpha > 1)       // Inside
//     color.a = 1;
//     else if (alpha < 0)  // Outside
//     clip(-1);
//     else
//     // Near boundary
//     color.a = alpha;
//     return color;
//    );
//
//    module.fragFunctions = @[function];
//
//    module.uniformUpdateBlock = newUniformUpdateBlock {
//    };
//
//    return module;
//
//}
//
//
//+(NKShaderModule*) templateModule:(F1t)size {
//
//    NKShaderModule* module = [[NKShaderModule alloc] init];
//
//    module.name = @"NAME";
//
//    [module.uniforms addObject:nksu(NKS_PRECISION_LOW, NKS_TYPE_V4, NKS_FALSE_COLOR_DARK_COLOR)];
//
//    NKShaderFunction *function = [[NKShaderFunction alloc]init];
//
//    function.name = @"functionName";
//    function.inputType = NKS_TYPE_V4;
//    function.returnType = NKS_TYPE_V4;
//
//    function.constants = SHADER_STRING
//    (
//    );
//
//    function.function = SHADER_STRING
//    (
//    );
//
//    module.fragFunctions = @[function];
//
//    module.uniformUpdateBlock = newUniformUpdateBlock {
//    };
//
//    return module;
//
//}


bool NKShaderModule::hasUniformNamed(NKS_ENUM name){
    for (auto& v : uniforms){
        if (v.name == name) return true;
    }
    return false;
}

NKShaderVariable& NKShaderModule::uniformNamed(NKS_ENUM name) {
    for (auto& v : uniforms){
        if (v.name == name) return v;
    }
    return uniforms[0]; // this will blow up if empty
}

bool NKShaderModule::hasVaryingNamed(NKS_ENUM name){
    for (auto& v : uniforms){
        if (v.name == name) return true;
    }
    return false;
}

NKShaderVariable& NKShaderModule::varyingNamed(NKS_ENUM name) {
    for (auto& v : varyings){
        if (v.name == name) return v;
    }
    return varyings[0]; // this will blow up if empty
}

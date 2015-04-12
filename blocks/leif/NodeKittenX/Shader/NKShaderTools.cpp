//
//  NKShaderVariable.m
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#include "NKShaderTools.h"
#include "NKAppleDeviceTools.h"
#include "NKStringUtil.h"
#include "NKFrameBuffer.h"

#define nksf String::format

using namespace std;

std::string nksString(NKS_ENUM name) {
    
    switch (name) {
            
            // ATTRIBUTES
        case NKS_V4_POSITION:
            return "position";
            
        case NKS_V2_TEXCOORD:
            return "texCoord0";
            
        case NKS_V3_NORMAL:
            return "normal";
            
        case NKS_V4_COLOR:
            return "color";
            
        case NKS_V3_TANGENT:
            return "tangent";
            
        case NKS_V3_BINORMAL:
            return "binormal";
            
        case NKS_F1_BONE_WEIGHT:
            return "boneWeight";
            
        // GL VAR TYPES
            
        case NKS_VARIABLE_ATTRIBUTE:
            return "attribute";
        case NKS_VARIABLE_UNIFORM:
            return "uniform";
        case NKS_VARIABLE_VARYING:
            return "varying";
        case NKS_VARIABLE_INLINE:
            return "";
            
            // GL PRECISION
            
        case NKS_PRECISION_LOW:
            return "lowp";
        case NKS_PRECISION_MEDIUM:
            return "mediump";
        case NKS_PRECISION_HIGH:
            return "highp";
        case NKS_PRECISION_NONE:
            return "";
            
            // GL VECTOR TYPES
        case NKS_TYPE_VOID:
            return "void";
        case NKS_TYPE_F1:
            return "float";
        case NKS_TYPE_M9:
            return "mat3";
        case NKS_TYPE_M16:
            return "mat4";
        case NKS_TYPE_V2:
            return "vec2";
        case NKS_TYPE_V3:
            return "vec3";
        case NKS_TYPE_V4:
            return "vec4";
        case NKS_TYPE_INT:
            return "int";
        case NKS_TYPE_BOOL:
            return "bool";
        case NKS_TYPE_SAMPLER_2D:
            return "sampler2D";
        case NKS_TYPE_SAMPLER_2D_RECT:
            return "sampler2DRect";
            
        case NKS_STRUCT_LIGHT:
            return "LightProperties";
        case NKS_STRUCT_MATERIAL:
            return "MaterialProperties";
            // NK VAR NAMES
            // ATTRIBUTES
            
        case NKS_V3_EYE_DIRECTION:
            return "eyeDirection";
        // UNIFORMS
            
        case NKS_M16_MVP:
            return "modelViewProjectionMatrix";
        case NKS_M16_MV:
            return "modelViewMatrix";
        case NKS_M16_P:
            return "projectionMatrix";
            
        case NKS_M9_NORMAL:
            return "normalMatrix";
        case NKS_INT_USE_UNIFORM_COLOR:
            return "useUniformColor";
        case NKS_INT_NUM_TEXTURES:
            return "numTextures";
        case NKS_F1_INSTANCE:
            return "instance";
        case NKS_INT_NUM_PASSES:
            return "numPasses";
        case NKS_INT_CURRENT_PASS:
            return "currentPass";
        
        case NKS_LIGHT:
            return "light[4]";
        case NKS_I1_NUM_LIGHTS:
            return "numLights";
        case NKS_V3_LIGHT_DIRECTION:
            return "lightDirection";
        case NKS_V3_LIGHT_HALF_VECTOR:
            return "halfVector";
        case NKS_F1_ATTENUATION:
            return "attenuation";
            
        case NKS_S2D_TEXTURE:
            return "texture";
        case NKS_TEXTURE_RECT_SCALE:
            return "textureScale";
        case NKS_SIZE:
            return "size";
            // GL BUILT IN
        case NKS_V3_GL_POSITION:
            return "gl_position";
        case NKS_V4_GL_FRAG_COLOR:
#if NK_USE_GL3
            return "FragColor";
#else
            return "gl_FragColor";
#endif
        case NKS_F1_GL_LINEWIDTH:
            return "gl_Linewidth";
            
        case NKS_UINT_GL_INSTANCE_ID:
#if (NK_USE_GLES && !NK_USE_WEBGL)
            return "gl_InstanceIDEXT";
#else
            return "gl_InstanceID";
#endif
            
            // EXTENSIONS
        case NKS_EXT_DRAW_INSTANCED:
#if TARGET_EMSCRIPTEN
            return "#extension GL_ANGLE_instanced_arrays : enable";
            //return "#extension ANGLE_instanced_arrays : enable";
#elif NK_USE_GLES
            //return @"";
            return "#extension GL_EXT_draw_instanced : enable";
#else
            return "#extension GL_ARB_draw_instanced : enable";
#endif
            
        case NKS_EXT_GPU_SHADER:
#if NK_USE_GLES
            return "";
            //return @"#extension GL_OES_element_index_uint : enable";
#else
            return "#extension GL_EXT_gpu_shader4 : enable";
#endif
            
        // DEFAULT SHADERS
        case NKS_PASSTHROUGH_TEXTURE_SHADER:
            return "passThroughTextureShader";
        case NKS_PASSTHROUGH_UNIFORM_SHADER:
            return "passThroughColorShader";
        // MODULES
            
        case NKS_FALSE_COLOR_DARK_COLOR:
            return "falseColor_darkColor";
        case NKS_FALSE_COLOR_LIGHT_COLOR:
            return "falseColor_lightColor";
        case NKS_FALSE_COLOR_INTENSITY:
            return "falseColor_intensity";
            
        case NKS_CURRENT_TIME:
            return "time";
            
        default: return "((NKS_STRING_ENUM_ERROR))";
    }
    
}

string shaderStringFromFile(string name){
    
#if (TARGET_IOS || TARGET_OSX)
    string path = NKApple::pathForResource(name, "nks.txt");
#else
    string path = "error";
#endif
    string file;
    return String::getfile(path, file);
    return file;
}

string shaderStringWithDirective(string name, string directive){
    string file = shaderStringFromFile(name);
    vector<string> parts = String::split(file, directive, false, 0);
    return parts[1];
}

//string shaderStringWithArray(vector<string>& array){
//    
//    for (string& o : array) {
//        if ([o isKindOfClass:[NKShaderVariable class]]) {
//            [ms appendString:[(NKShaderVariable*)o nameString]];
//        }
//        else if ([o isKindOfClass:[NSNumber class]]){
//            [ms appendFormat:@"%@",[(NSNumber*)o stringValue]];
//        }
//        else  if ([o isKindOfClass:[NSString class]]) {
//            [ms appendString:(NSString*)o];
//            if ([o characterAtIndex:o.length-1] == ';' || [o characterAtIndex:o.length-1] == '{' || [o characterAtIndex:o.length-1] == '}') {
//                [ms appendString:@"\n"];
//            }
//        }
//    }
//    
//    return ms;
//}
//
//string shaderLineWithArray(NSArray *array) {
//    NSString *ms = shaderStringWithArray(array);
//
//    return [ms stringByAppendingString:@";\n"];
//
//}
//
//string operatorString(NSArray* variables, NSString *operator) {
//    NSMutableString *mult = [[nksf:@"%@", [variables[0] nameString]] mutableCopy];
//    for (int i = 1; i < variables.count; i++) {
//        [mult appendString:[nksf:@" %@ %@", operator, [variables[i] nameString]]];
//    }
//    return mult;
//}


string NKShaderFunction::functionString() {

    string functionString = String::format("%s %s (%s inputColor){ \n" ,nks(returnType).c_str(),name.c_str(),nks(inputType).c_str());
    
    functionString += function;
    
    functionString += "}\n";
    
    return functionString;
}

#pragma mark - NKShaderVariable


string NKShaderVariable::nameString() {
    switch (variable) {
        case NKS_VARIABLE_ATTRIBUTE:
            return nksf("a_%s",nks(name).c_str());
            break;
            
        case NKS_VARIABLE_UNIFORM:
            return nksf("u_%s",nks(name).c_str());
            break;
            
        case NKS_VARIABLE_VARYING:
            return nksf("v_%s",nks(name).c_str());
            break;
            
        case NKS_VARIABLE_INLINE:
            return nks(name);
            break;
            
        default:
            return "NKS_ERROR";
    }

}


string NKShaderVariable::description() {
    return nameString();
}

string NKShaderVariable::declarationStringForSection(NKS_ENUM section) {
    
    string dec;
    
#if NK_USE_GL3
    
    if (_variable == NKS_VARIABLE_VARYING) {
        
        switch (section) {
            case NKS_VERTEX_SHADER:
                dec = [@"out" mutableCopy];
                break;
                
            case NKS_FRAGMENT_SHADER:
                dec = [@"in" mutableCopy];
                break;
                
            default:
                break;
        }
    }
    else if (_variable == NKS_VARIABLE_ATTRIBUTE) {
        dec = [[NSString stringWithFormat:@"layout (location = %d) in",_name] mutableCopy];
    }
    else {
        dec = [nks(_variable) mutableCopy];
    }
    
#else
    dec = nks(variable);
#endif
    
#if NK_USE_GLES
    dec += " " + nks(precision);
#endif
    dec += " " + nks(type);
    dec += " " + nameString();
    if (arraySize) {
        dec += String::format("[%d]",arraySize);
    }
    dec += ";";
    return dec;
}

#pragma mark - BINDING UNIFORMS


void NKShaderVariable::bindI1(I1t data) {
    glUniform1i(glLocation, data);
    nkGetGLError();
}

void NKShaderVariable::bindF1(F1t data) {
    glUniform1f(glLocation, data);
    nkGetGLError();
}

void NKShaderVariable::bindV2(V2t data) {
#ifdef NK_USE_ARB_EXT
    glUniform2fvARB(glLocation, 1, data.v);
#else
    glUniform2fv(glLocation, 1, data.v);
#endif
    nkGetGLError();
}

void NKShaderVariable::bindV3(V3t data) {
#ifdef NK_USE_ARB_EXT
    glUniform3fvARB(glLocation, 1, data.v);
#else
    glUniform3fv(glLocation, 1, data.v);
#endif
    nkGetGLError();
}

void NKShaderVariable::bindV4(V4t data) {
#ifdef NK_USE_ARB_EXT
    glUniform4fvARB(glLocation, 1, data.v);
#else
    glUniform4fv(glLocation, 1, data.v);
#endif
    nkGetGLError();
}

void NKShaderVariable::bindM9(M9t data) {
    glUniformMatrix3fv(glLocation, 1, GL_FALSE, data.m);
    nkGetGLError();
}

void NKShaderVariable::bindM16(M16t data) {
    glUniformMatrix4fv(glLocation, 1, GL_FALSE, data.m);
    nkGetGLError();
}

void NKShaderVariable::bindV4Array(V4t* data, int count) {
#ifdef NK_USE_ARB_EXT
    glUniform4fvARB(glLocation, count, data->v); glUniform4fv(glLocation, count, data->v);
#else
    glUniform4fv(glLocation, count, data->v);
#endif
    nkGetGLError();
}

void NKShaderVariable::bindM9Array(M9t *data, int count){
    glUniformMatrix3fv(glLocation, count, GL_FALSE, data->m);
    nkGetGLError();
}

void NKShaderVariable::bindM16Array(M16t *data, int count){
    glUniformMatrix4fv(glLocation, count, GL_FALSE, data->m);
    nkGetGLError();
}

void NKShaderVariable::bindLightProperties(NKLightProperties& data, int index){
        int numUniforms = 11;
        int base = glLocation + (index*numUniforms);
        glUniform3fv(base+0, 1, data.position.v);
        glUniform3fv(base+1, 1, data.color.v);
        glUniform3fv(base+2, 1, data.halfVector.v);
        glUniform3fv(base+3, 1, data.coneDirection.v);
        
        glUniform1fv(base+4, 1, &data.spotCosCutoff);
        glUniform1fv(base+5, 1, &data.spotExponent);
        glUniform1fv(base+6, 1, &data.ambient);
        glUniform1fv(base+7, 1, &data.linearAttenuation);
        glUniform1fv(base+8, 1, &data.quadraticAttenuation);
        
        glUniform1i(base+9, data.isLocal);
        glUniform1i(base+10, data.isSpot);
}


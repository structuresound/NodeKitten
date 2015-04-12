//
//  NOCShaderProgram.h
//  Nature of Code
//
//  Created by William Lindmeier on 2/2/13.
//  Copyright (c) 2013 wdlindmeier. All rights reserved.
//

#ifndef __NKShaderProgram__
#define __NKShaderProgram__


#include "NKShaderModule.h"
// ATTRIBUTES

//GLint uniforms[NUM_UNIFORMS];

class NKTexture;

#define NUM_ATTRIBUTES 7

class NKShaderProgram : public enable_shared_from_this<NKShaderProgram> {
    
    static vector<bool> vertexAttributeState;
    
    U1t _numTextures {0};
    U1t _batchSize {0};
    NKS_COLOR_MODE _colorMode {NKS_COLOR_MODE_NONE};
    
    vector<string> extensions;
    
    vector<NKShaderVariable> _attributes;
    vector<NKShaderVariable> _uniforms;
    vector<NKShaderVariable> _varyings;
    
    vector<string> _vertMain;
    vector<string> _fragMain;
    
    vector<NKShaderVariable> _vertVars;
    vector<NKShaderVariable> _fragVars;
    
    string _vertShaderPath;
    string _fragShaderPath;
    
public:
    
    static shared_ptr<NKShaderProgram> shaderNamed(string name);
    static shared_ptr<NKShaderProgram> shaderNamed(string name, vector<NKShaderModule> modules, int batchSize = 0);
    static shared_ptr<NKShaderProgram> shaderNamed(string name, NKS_COLOR_MODE colorMode, U1t numTextures, int numLights, int batchSize = 0);
    
    NKShaderProgram();
    NKShaderProgram(string name, vector<NKShaderModule> modules, int batchSize);
    NKShaderProgram(string name_, NKS_COLOR_MODE colorMode, U1t numTex, int numLights, int batchSize);
    
    ~NKShaderProgram();
    
    vector<NKShaderModule> modules;
    
    string vertexSource();
    string fragmentSource();
    
    string name;
    GLuint glName {0};
    
    U1t batchSize {0};
    U1t numPasses {0};
    U1t currentPass {0};
    
    vector<string> glslExtensions;

    bool hasAttributeNamed(NKS_ENUM name);
    NKShaderVariable& attributeNamed(NKS_ENUM name);
    
    bool hasUniformNamed(NKS_ENUM name);
    NKShaderVariable& uniformNamed(NKS_ENUM name);
    
    bool hasVaryingNamed(NKS_ENUM name);
    NKShaderVariable& varyingNamed(NKS_ENUM name);
    
    NKShaderVariable& vertVarNamed(NKS_ENUM name);
    NKShaderVariable& fragVarNamed(NKS_ENUM name);
    
    bool load();
    void unload();
    void use();
    
    bool operator==(const NKShaderProgram& other);
    
    void setVertexAttributeState(U1t attribute, bool state){
        
        if (vertexAttributeState[attribute] != state){
            if (state){ // ENABLE
                glEnableVertexAttribArray(attribute);
                //glBindAttribLocation(glName, attribute, nks(attribute).c_str());
                nkLog("(*) %d : %s", (int)attribute, nks((NKS_ENUM)attribute));
            }
            
            else { // DISABLE
                glDisableVertexAttribArray(attribute);
                //nkLog("( ) %d : %s", (int)attribute, nks((NKS_ENUM)attribute));
            }
            vertexAttributeState[attribute] = state;
        }
        
    }

private:
    
    bool compileShader(GLuint *shader, GLenum type, string& shaderSource);
    void calculateCommonVertexVaryings();
    vector<string> uniformNames();
    void writeShaderStrings();
    bool linkProgram(GLuint prog);
    bool validateProgram(GLuint prog);
};

#endif

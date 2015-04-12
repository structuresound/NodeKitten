//
//  NKFragModule.h
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 7/12/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#ifndef __NKShaderModule__
#define __NKShaderModule__

#include "NKShaderTools.h"

class NKShaderModule;

typedef function<void(NKShaderModule& module)> UniformUpdateBlock;

class NKShaderModule : public enable_shared_from_this<NKShaderModule> {
    
public:
    
    string name;
    
    vector<NKShaderVariable> uniforms;
    vector<NKShaderVariable> varyings;
    
    string types;
    
    vector<string> vertexMain;
    vector<string> fragmentMain;
    
    vector<NKShaderFunction> vertFunctions;
    vector<NKShaderFunction> fragFunctions;
    
    void updateUniforms(){
        if (uniformUpdateBlock){
            uniformUpdateBlock(*this);
        }
    }
    
    UniformUpdateBlock uniformUpdateBlock;
    
    NKS_COLOR_MODE colorMode {NKS_COLOR_MODE_NONE};
    U1t numTexures {0};
    
    bool hasUniformNamed(NKS_ENUM name);
    NKShaderVariable& uniformNamed(NKS_ENUM name);
    bool hasVaryingNamed(NKS_ENUM name);
    NKShaderVariable& varyingNamed(NKS_ENUM name);
    
    // DEFAULT MODULES
    
    static NKShaderModule materialModule(int numTex);
    static NKShaderModule colorModule(NKS_COLOR_MODE colorMode, int batchSize);
    static NKShaderModule textureModule(int numTex);
    static NKShaderModule lightModule(bool highQuality, int batchSize);
    
    // POST PROCESS (FRAGMENT ONLY) MODULES
    
    static NKShaderModule falseColorModule(F1t intensity, NKColor darkColor, NKColor lightColor);
    static NKShaderModule swirl(F1t intensity, V2t size);
    
};

#endif
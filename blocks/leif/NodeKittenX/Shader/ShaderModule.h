//
//  NKFragModule.h
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 7/12/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#ifndef __ShaderModule__
#define __ShaderModule__

#include "ShaderTools.h"

class ShaderModule;

typedef function<void(ShaderModule& module)> UniformUpdateBlock;

class ShaderModule : public enable_shared_from_this<ShaderModule> {
    
public:
    
    string name;
    
    vector<ShaderVariable> uniforms;
    vector<ShaderVariable> varyings;
    
    string types;
    
    vector<string> vertexMain;
    vector<string> fragmentMain;
    
    vector<ShaderFunction> vertFunctions;
    vector<ShaderFunction> fragFunctions;
    
    void updateUniforms(){
        if (uniformUpdateBlock){
            uniformUpdateBlock(*this);
        }
    }
    
    UniformUpdateBlock uniformUpdateBlock;
    
    NKS_COLOR_MODE colorMode {NKS_COLOR_MODE_NONE};
    U1t numTexures {0};
    
    bool hasUniformNamed(NKS_ENUM name);
    ShaderVariable& uniformNamed(NKS_ENUM name);
    bool hasVaryingNamed(NKS_ENUM name);
    ShaderVariable& varyingNamed(NKS_ENUM name);
    
    // VERTEX MODULES
    
    static ShaderModule vertexPositionModuleDefault();
    static ShaderModule batchSpritePositionModule(int batchSize);
    
    // DEFAULT MODULES
    
    static ShaderModule materialModule(int numTex);
    static ShaderModule colorModule(NKS_COLOR_MODE colorMode, int batchSize);
    static ShaderModule textureModule(int numTex);
    static ShaderModule lightModule(bool highQuality, int batchSize);
    static ShaderModule freetypeModule();
    
    // POST PROCESS (FRAGMENT ONLY) MODULES
    
    static ShaderModule falseColorModule(F1t intensity, Color darkColor, Color lightColor);
    static ShaderModule swirl(F1t intensity, V2t size);
    
};

#endif
//
//  NKLightNode.h
//  EMA Stage
//
//  Created by Leif Shackelford on 5/26/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#ifndef __NKLightNode_h_
#define __NKLightNode_h_

#include "NKMeshNode.h"
#include "NKShaderProgram.h"

class NKLightNode : public NKMeshNode
{
    
public:
    
    static shared_ptr<NKLightNode> lightNode();
    static shared_ptr<NKLightNode> lightNode(NKColor color);
    static shared_ptr<NKLightNode> lightNode(NKLightProperties properties);
    
    NKLightNode(NKLightProperties properties);
    
    NKLightProperties properties;
    
    void draw() override;
    void chooseShader() override;
    void removeFromParent() override;
    void setColor(NKColor color) override;
    void setScene(NKSceneNode *scene) override;
    void setDirty(bool dirty) override;
    
};

#endif
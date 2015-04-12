//
//  NKBatchNode.h
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#ifndef __NKBatchNode_h_
#define __NKBatchNode_h_

#include "NKMeshNode.h"

class NKShaderProgram;

class NKBatchNode : public NKMeshNode
{
    GLuint spritesToDraw;
    
public:
    
    using NKMeshNode::NKMeshNode;
    
    static shared_ptr<NKBatchNode> meshNodeWithObj(string objName, V3t size = V3MakeF(1.0), bool normalize = true, bool anchor = false);
    static shared_ptr<NKBatchNode> meshNodeWithPrimitive(NKPrimitive primitive, shared_ptr<NKTexture> texture, NKColor color, V3t size);
    static shared_ptr<NKBatchNode> meshNodeWithVbo(shared_ptr<NKVertexBuffer> buffer, GLenum drawMode, shared_ptr<NKTexture> texture, NKColor color, V3t size);
    
    M16Stack mvpStack;
    M16Stack mvStack;
    M9Stack normalStack;
    V4Stack childColors;
    
    shared_ptr<NKShaderProgram> hitShader;

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    void setTexture(shared_ptr<NKTexture> texture) override;
#endif

    void chooseShader() override;
    void customDraw() override;
    void drawChildren() override;
    void drawWithHitShader();
    
    void drawGeometry(U1t start, U1t spritesInBatch, bool useColor);
};

#endif


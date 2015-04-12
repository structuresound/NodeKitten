//
//  NKLightNode.m
//  EMA Stage
//
//  Created by Leif Shackelford on 5/26/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#include "NKLightNode.h"
#include "NKSceneNode.h"

shared_ptr<NKLightNode> NKLightNode::lightNode(NKLightProperties properties){
    return make_shared<NKLightNode>(properties);
}

shared_ptr<NKLightNode> NKLightNode::lightNode(){
    NKLightProperties p;
    
    p.isLocal = true;
    p.isSpot = 0;
    
    p.color = V3Make(1.0,1.0,1.0);
    p.coneDirection = V3Make(0, 0, -1);
    p.halfVector = V3MakeF(0);
    
    p.spotCosCutoff = 10.;
    p.spotExponent = 2;
    p.ambient = 0.0;
    p.linearAttenuation = 1.0;
    p.quadraticAttenuation = 0;
    
    return make_shared<NKLightNode>(p);
}

shared_ptr<NKLightNode> NKLightNode::lightNode(NKColor color){
    
    shared_ptr<NKLightNode> newLight = NKLightNode::lightNode();
    
    newLight->properties.color = color.data.xyz;
    
    return newLight;
}


NKLightNode::NKLightNode(NKLightProperties properties_)
: NKMeshNode(NKPrimitiveSphere, nullptr, NKColor("lightyellow"), V3MakeF(.125)) {
    properties = properties_;
}

void NKLightNode::setColor(NKColor color){
    NKNode::setColor(color);
    properties.color = color.data.xyz;
}

void NKLightNode::setDirty(bool dirty){
    NKMeshNode::setDirty(dirty);
    if (dirty){
        if (scene()) {
            properties.position = V3MultiplyM16WithTranslation(scene()->camera->viewMatrix(), globalPosition());
            //NKLogV3("light position : ", properties.position);
        }
    }
}

void NKLightNode::setScene(NKSceneNode *scene_){
    NKMeshNode::setScene(scene_);
    scene()->lights.push_back(this);
}

void NKLightNode::chooseShader() {
    _shader = NKShaderProgram::shaderNamed("lightNodeShader",NKS_COLOR_MODE_UNIFORM,0,0);
}

void NKLightNode::removeFromParent(){
    erase_object(scene()->lights, this);
    NKMeshNode::removeFromParent();
}

void NKLightNode::draw(){
    if (scene()->drawLights) {
        NKMeshNode::draw();
    }
}

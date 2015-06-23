//
//  Light.m
//  EMA Stage
//
//  Created by Leif Shackelford on 5/26/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#include "Light.h"
#include "Scene.h"

shared_ptr<Light> Light::light(Scene* scene, Color color){
    LightProperties p;
    
    p.isLocal = true;
    p.isSpot = 0;
    
    p.color = color.rgb;
    p.coneDirection = V3(0, 0, -1);
    p.halfVector = V3(0);
    
    p.spotCosCutoff = 10.;
    p.spotExponent = 2;
    p.ambient = 0.0;
    p.linearAttenuation = 1.0;
    p.quadraticAttenuation = 0;
    
    return make_shared<Light>(scene, p);
}

Light::Light(Scene* scene, LightProperties properties_)
: Mesh(NKPrimitiveSphere, nullptr, Color("lightyellow"), V3(.125)) {
    _scene = scene;
    properties = properties_;
}

void Light::setColor(Color color){
    Node::setColor(color);
    properties.color = color.rgb;
}

void Light::setDirty(bool dirty){
    _dirty = dirty;
    if (dirty){
        if (scene()) {
            properties.position = V3MultiplyM16WithTranslation(scene()->camera->viewMatrix(), globalPosition());
        }
    }
}

void Light::chooseShader() {
    shader = Shader::shaderNamed("lightShader",NKS_COLOR_MODE_UNIFORM,0,0);
}

void Light::setParent(Node *parent){
    Node::setParent(parent);
    if (!_c::contains(scene()->lights, this)) {
        scene()->lights.push_back(this);
    }
}

void Light::removeFromParent(){
    _c::erase(scene()->lights, this);
    Mesh::removeFromParent();
}

void Light::draw(){
    if (scene()->drawLights) {
        Mesh::draw();
    }
}

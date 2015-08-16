//
//  Light.m
//  EMA Stage
//
//  Created by Leif Shackelford on 5/26/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#include "Light.h"
#include "Scene.h"

using namespace Shader;

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
    
    return std::make_shared<Light>(scene, p);
}

Light::Light(Scene* scene, LightProperties properties_)
: Mesh(NKPrimitiveSphere, nullptr, Color("lightyellow"), V3(.125)) {
    _scene = scene;
    _scene->lights.push_back(this);
    properties = properties_;
    globalTransform.setPullFunction([this]{
        auto super = Node::transform.get();
        properties.position = V3MultiplyM16WithTranslation(_scene->camera->viewMatrix.get(), super.translation);
        return super;
    });
}

void Light::setColor(Color color){
    Node::setColor(color);
    properties.color = color.rgb;
}

void Light::chooseShader() {
    shader = Program::shaderNamed("lightShader",NKS_COLOR_MODE_UNIFORM,0,0);
}

void Light::afterRemove(){
    _::erase(scene()->lights, this);
}

void Light::draw(){
    if (scene()->drawLights) {
        Mesh::draw();
    }
}

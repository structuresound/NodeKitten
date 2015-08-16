//
//  Light.h
//  EMA Stage
//
//  Created by Leif Shackelford on 5/26/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#ifndef __Light_h_
#define __Light_h_

#include "Mesh.h"
#include "ShaderProgram.h"

class Light : public Mesh
{
    
public:
    
    static std::shared_ptr<Light> light(Scene* scene, Color color = WHITE);
    
    Light(Scene* scene, Shader::LightProperties properties);

    Shader::LightProperties properties;
    
    void draw() override;
    void chooseShader() override;
    void afterRemove() override;
    void setColor(Color color) override;
    
};

#endif
//
//  NKSpriteEmitter.h
//  Pods
//
//  Created by Leif Shackelford on 5/24/15.
//
//

#pragma once

#include "mesh.h"

class SpriteEmitter : public Mesh
{

public:
    static std::shared_ptr<SpriteEmitter> nodeWithTexture(std::shared_ptr<Texture> texture, Color color = Color(1));

    SpriteEmitter(std::shared_ptr<Texture> texture, Color color) :
    Mesh(NKPrimitiveRect, texture, color, V3(1.0))
    {}

    bool sortEveryPass {false};

    // manual
    V4Stack colorStack;
    V2Stack positionStack;
    F1Stack scaleStack;
    // algorithmic / shader based
    F1Stack ageStack;

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    void setTexture(shared_ptr<Texture> texture) override;
#endif
    void chooseShader() override;
    void customDraw() override;
    void drawChildren() override;
    void updateWithTimeSinceLast(F1t dt) override;

    void drawGeometry(U1t start, U1t spritesInBatch, bool useColor);
};

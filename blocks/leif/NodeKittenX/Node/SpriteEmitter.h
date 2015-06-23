//
//  NKSpriteEmitter.h
//  Pods
//
//  Created by Leif Shackelford on 5/24/15.
//
//

#ifndef __NKSpriteEmitter__
#define __NKSpriteEmitter__

#include "Mesh.h"

class Shader;

class SpriteEmitter : public Mesh
{
    
public:
    static shared_ptr<SpriteEmitter> nodeWithTexture(shared_ptr<Texture> texture, Color color = Color(1));
    
    SpriteEmitter(shared_ptr<Texture> texture, Color color) :
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

#endif /* defined(__Pods__NKSpriteEmitter__) */

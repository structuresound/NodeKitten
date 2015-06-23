//
//  NKSpriteEmitter.cpp
//  Pods
//
//  Created by Leif Shackelford on 5/24/15.
//
//

#include "SpriteEmitter.h"
#include "ShaderProgram.h"
#include "Scene.h"

shared_ptr<SpriteEmitter> SpriteEmitter::nodeWithTexture(shared_ptr<Texture> texture, Color color) {
    return make_shared<SpriteEmitter>(texture, color);
}

void SpriteEmitter::chooseShader(){
#if !NK_SUPPORTS_INSTANCED_DRAWING
    // These don't support instanced drawing right now
#else
    if (_numTextures) {
        shader = Shader::shaderNamed("emitterWithTexture", {ShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, NK_BATCH_SIZE),ShaderModule::textureModule(1), ShaderModule::batchSpritePositionModule(NK_BATCH_SIZE)}, NK_BATCH_SIZE);
    }
    else {
        shader = Shader::shaderNamed("emitterWithColor", {ShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, NK_BATCH_SIZE), ShaderModule::batchSpritePositionModule(NK_BATCH_SIZE)}, NK_BATCH_SIZE);
    }
#endif
}

#if !NK_SUPPORTS_INSTANCED_DRAWING
void SpriteEmitter::setTexture(shared_ptr<Texture> texture) {
    for (auto& node : _children){
        static_pointer_cast<Mesh>(node)->setTexture(texture);
    }
}
#endif

void SpriteEmitter::drawChildren(){
#if !NK_SUPPORTS_INSTANCED_DRAWING
    // These don't support instanced drawing right now
    Mesh::drawChildren();
#endif
}

void SpriteEmitter::customDraw(){
#if !NK_SUPPORTS_INSTANCED_DRAWING
    // These don't support instanced drawing right now
#else
    pushStyle();
    
    bool useColor = false;
    
    _vertexBuffer->bind();
    
    bindTextures();
    
    int startingSprite = 0;
    int spritesInBatch = 0;
    
    positionStack.clear();
    scaleStack.clear();
    
    if (Shader::activeShader->hasUniformNamed(NKS_V4_COLOR)) {
        useColor = true;
        colorStack.clear();
    }
    
    if (sortEveryPass) {
        if (_children.size() > 1) {
            sort(_children.begin(), _children.end(),
                 [](const shared_ptr<Node> a, const shared_ptr<Node> b)
                 {
                     return a->position().z < b->position().z;
                 });
        }
    }
    
    for (auto child : _children){
        positionStack.append(child->position().xy);
        scaleStack.append(child->size().x * child->scale().x);
        for (auto c2 : child->children()){
            positionStack.append(child->position().xy + c2->position().xy);
            scaleStack.append(c2->size().x * c2->scale().x * child->size().x * child->scale().x);
        }
    }
    
    if (useColor) {
        for (auto child : _children){
            colorStack.append(child->glColor());
            for (auto c2 : child->children()){
                colorStack.append(c2->glColor());
            }
        }
    }
    
    setupViewMatrix();
    
    for (int i = 0; i < positionStack.size(); i++) {
        spritesInBatch++;
        if (spritesInBatch == NK_BATCH_SIZE || i == positionStack.size() - 1) {
            //nkLog("drawing sprites %d - %d ", startingSprite, startingSprite+spritesInBatch);
            drawGeometry(startingSprite, spritesInBatch, useColor);
            startingSprite += spritesInBatch;
            spritesInBatch = 0;
        }
    }
#endif
}

void SpriteEmitter::updateWithTimeSinceLast(F1t dt){
    // IF OVERRIDE, CALL SUPER
    
    //    if (_body && _body.isDynamic){
    //        [_body getTransform:&_localTransform];
    //        //NKLogV3(@"update physics position: ", V3GetM16Translation(localTransform));
    //        _dirty = true;
    //    }
    
    vector<shared_ptr<Action>> complete;
    
    auto runable = actions;
    for (auto action : runable){
        if (excecuteAction(action.get(), this, dt)) complete.push_back(action);
    }
    for (auto action : complete){
        removeAction(action);
    }
    
    if (_updateBlock){
        _updateBlock(dt);
    }
    
}

void SpriteEmitter::drawGeometry(U1t start, U1t spritesInBatch, bool useColor){
#if NK_SUPPORTS_INSTANCED_DRAWING
    Shader::activeShader->uniformNamed(NKS_V2_BATCH_POSITION).bindV2Array(positionStack.data()+start, spritesInBatch);
    Shader::activeShader->uniformNamed(NKS_F1_BATCH_SCALE).bindF1Array(scaleStack.data()+start, spritesInBatch);
    if (useColor) {
        Shader::activeShader->uniformNamed(NKS_V4_COLOR).bindV4Array(colorStack.data()+start, spritesInBatch);
    }
#if NK_USE_GLES
    glDrawArraysInstancedEXT(_drawMode, 0, _vertexBuffer->size(), spritesInBatch);
#else // DESKTOP GL
#ifdef NK_USE_ARB_EXT
    glDrawArraysInstancedARB(_drawMode, 0, (int)_vertexBuffer->size(), spritesInBatch);
#else
    glDrawArraysInstanced(_drawMode, 0, (int)_vertexBuffer->size(), spritesInBatch);
#endif
#endif
#endif
}

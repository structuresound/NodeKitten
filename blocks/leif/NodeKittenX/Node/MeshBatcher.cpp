//
//  MeshBatcher.m
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#include "MeshBatcher.h"
#include "ShaderProgram.h"
#include "Scene.h"

shared_ptr<MeshBatcher> MeshBatcher::nodeWithPrimitive(NKPrimitive primitive, shared_ptr<Texture> texture, Color color, V3t size){
    return make_shared<MeshBatcher>(primitive, texture, color, size);
}

shared_ptr<MeshBatcher> MeshBatcher::nodeWithObj(string objName, V3t size, bool normalize, bool anchor){
    return make_shared<MeshBatcher>(objName, size, normalize, anchor);
}

shared_ptr<MeshBatcher> MeshBatcher::nodeWithVbo(shared_ptr<VertexBuffer> buffer, GLenum drawMode, shared_ptr<Texture> texture, Color color, V3t size) {
    return make_shared<MeshBatcher>(buffer, drawMode, texture, color, size);
}

void MeshBatcher::chooseShader(){
#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    // These don't support instanced drawing right now
    return;
#endif
    if (!hitShader){
        hitShader = Shader::shaderNamed("b_HitShader",NKS_COLOR_MODE_UNIFORM ,0,0,NK_BATCH_SIZE);
    }
    if (_numTextures) {
        if (_is2d) {
            shader = Shader::shaderNamed("spriteBatch", {ShaderModule::textureModule(1), ShaderModule::batchSpritePositionModule(NK_BATCH_SIZE)}, NK_BATCH_SIZE);
        }
        else {
            shader = Shader::shaderNamed("b_ColorTextureLightShader",NKS_COLOR_MODE_UNIFORM,_numTextures,1 ,NK_BATCH_SIZE);
        }
    }
    else {
        shader = Shader::shaderNamed("b_colorLightShader",NKS_COLOR_MODE_UNIFORM,_numTextures,1,NK_BATCH_SIZE);
    }
}

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
void MeshBatcher::setTexture(shared_ptr<Texture> texture) {
    for (auto& node : _children){
        static_pointer_cast<Mesh>(node)->setTexture(texture);
    }
}
#endif

void MeshBatcher::drawChildren(){
#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    // These don't support instanced drawing right now
    Mesh::drawChildren();
    return;
#endif
    // handled in batch pass
}

void MeshBatcher::customDraw(){
    
#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    // These don't support instanced drawing right now
    return;
#endif
    
    pushStyle();
    
    bool useColor = false;
    
    _vertexBuffer->bind();
    
    bindTextures();
    
    if (_is2d) {
        positionStack.clear();
        scaleStack.clear();
    }
    else {
        mvpStack.clear();
        mvStack.clear();
        normalStack.clear();
    }
    if (Shader::activeShader->hasUniformNamed(NKS_V4_COLOR)) {
        useColor = true;
        childColors.clear();
    }
    
    vector<shared_ptr<Node>> toDraw;
    
    if (cullable && !_is2d) { // 99% only set non cullable on skybox etc.
        
        for (int i = 0; i < _children.size(); i++) {
            auto child = _children[i];
            
            child->modelViewCache = M16Multiply(scene()->camera->viewMatrix(),M16ScaleWithV3(child->globalTransform(), child->size()));
            
            // FAST CULL
            if (child->modelViewCache.m32 <= 0 || !child->cullable) {
                toDraw.push_back(child);
            }
        }
        if (toDraw.size() > 1) {
            sort(toDraw.begin(), toDraw.end(),
                 [](const shared_ptr<Node> a, const shared_ptr<Node> b)
                 {
                     // true means a push back
                     if (a->isOpaque() && b->isOpaque()) {
                         if (a->modelViewCache.m32 == b->modelViewCache.m32){
                             return a->siblingPosition < b->siblingPosition;
                         }
                         return a->modelViewCache.m32 < b->modelViewCache.m32;
                     }
                     else if (a->isOpaque()) {
                         return true;
                     }
                     else if (b->isOpaque()){
                         return false;
                     }
                     if (a->modelViewCache.m32 == b->modelViewCache.m32){
                         return a->siblingPosition < b->siblingPosition;
                     }
                     return a->modelViewCache.m32 < b->modelViewCache.m32;
                 });
        }
    }
    else {
        toDraw = _children;
    }
    
    
    if (_is2d) {
        for (auto child : toDraw){
            positionStack.append(child->globalPosition().xy);
            scaleStack.append(child->size().x * child->scale().x);
        }
    }
    else {
        for (auto child : toDraw){
            mvpStack.append(M16Multiply(scene()->camera->projectionMatrix(), child->modelViewCache));
            if (scene()->lights.size()) {
                mvStack.append(child->modelViewCache);
                normalStack.append(M16GetInverseNormalMatrix(child->modelViewCache));
            }
        }
    }
    
    if (useColor) {
        for (auto child : toDraw){
            childColors.append(child->glColor());
        }
    }
    
    if (_is2d) {
        auto mvp = M16Multiply(scene()->camera->viewProjectionMatrix(), M16ScaleWithV3(globalTransform(), V3(1.0)));
        Shader::activeShader->uniformNamed(NKS_M16_MVP).bindM16(mvp);
    }
    
    int startingSprite = 0;
    int spritesInBatch = 0;
    
    for (int i = 0; i < toDraw.size(); i++) {
        spritesInBatch++;
        if (spritesInBatch == NK_BATCH_SIZE || i == toDraw.size() - 1) {
            //nkLog("drawing sprites %d - %d ", startingSprite, startingSprite+spritesInBatch);
            drawGeometry(startingSprite, spritesInBatch, useColor);
            startingSprite += spritesInBatch;
            spritesInBatch = 0;
        }
    }
    
}


void MeshBatcher::drawWithHitShader() {
    
#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    // These don't support instanced drawing right now
    return;
#endif
    
    auto temp = scene()->hitDetectShader.get();
    hitShader->use();
    
    if (scene()->boundVertexBuffer != _vertexBuffer.get()) {
        _vertexBuffer->bind();
        scene()->boundVertexBuffer = _vertexBuffer.get();
    }
    
    mvpStack.clear();
    childColors.clear();
    
    int spritesInBatch = 0;
    int startingSprite = 0;
    
    for (int i = 0; i < _children.size(); i++) {
        auto child = _children[i];
        
        mvpStack.append(M16Multiply(scene()->camera->viewProjectionMatrix(), M16ScaleWithV3(child->globalTransform(), child->size())));
        childColors.append(Color{child->uidColor});
        
        spritesInBatch++;
        
        if (spritesInBatch == NK_BATCH_SIZE || i == _children.size() - 1) {
            drawGeometry(startingSprite, spritesInBatch, true);
            
            mvpStack.clear();
            childColors.clear();
            
            spritesInBatch = 0;
        }
    }
    
    temp->use();
}


void MeshBatcher::drawGeometry(U1t start, U1t spritesInBatch, bool useColor){
    if (_is2d) {
        Shader::activeShader->uniformNamed(NKS_V2_BATCH_POSITION).bindV2Array(positionStack.data()+start, spritesInBatch);
        Shader::activeShader->uniformNamed(NKS_F1_BATCH_SCALE).bindF1Array(scaleStack.data()+start, spritesInBatch);
    }
    else {
        Shader::activeShader->uniformNamed(NKS_M16_MVP).bindM16Array(mvpStack.data()+start, spritesInBatch);
        
        if (scene()->lights.size()) {
            Shader::activeShader->uniformNamed(NKS_M16_MV).bindM16Array(mvStack.data()+start, spritesInBatch);
            Shader::activeShader->uniformNamed(NKS_M9_NORMAL).bindM9Array(normalStack.data()+start, spritesInBatch);
        }
    }
    
    if (useColor) {
        Shader::activeShader->uniformNamed(NKS_V4_COLOR).bindV4Array(childColors.data()+start, spritesInBatch);
    }
    
    if (_primitiveType == NKPrimitiveLODSphere) {
#if TARGET_EMSCRIPTEN
        // NOT SUPPORTED
#elif TARGET_OS_IPHONE
        glDrawArraysInstancedEXT(_drawMode, _vertexBuffer->elementOffset[0], _vertexBuffer->elementSize[0], spritesInBatch);
#elif TARGET_RASPBERRY_PI
        // NOT SUPPORTED
#else
#ifdef NK_USE_ARB_EXT
        glDrawArraysInstancedARB(_drawMode, _vertexBuffer->elementOffset[0], _vertexBuffer->elementSize[0], spritesInBatch);
#else
        glDrawArraysInstanced(_drawMode, _vertexBuffer->elementOffset[0], _vertexBuffer->elementSize[0], spritesInBatch);
#endif
#endif
    }
    else {
#if TARGET_EMSCRIPTEN
        // NOT SUPPORTED
#elif TARGET_OS_IPHONE
        glDrawArraysInstancedEXT(_drawMode, 0, _vertexBuffer->size(), spritesInBatch);
#elif TARGET_RASPBERRY_PI
        // NOT SUPPORTED
#else // DESKTOP GL
#ifdef NK_USE_ARB_EXT
        //nkLog("draw %d vertices, %d indices", _vertexBuffer->size(), _vertexBuffer->_indices.size());
        glDrawArraysInstancedARB(_drawMode, 0, (int)_vertexBuffer->size(), spritesInBatch);
#else
        glDrawArraysInstanced(_drawMode, 0, (int)_vertexBuffer->size(), spritesInBatch);
#endif
#endif
    }
    
#if NK_LOG_METRICS
    Scene::draws++;
#endif
}

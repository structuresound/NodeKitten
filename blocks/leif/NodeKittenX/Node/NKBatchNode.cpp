//
//  NKBatchNode.m
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#include "NKBatchNode.h"
#include "NKShaderProgram.h"
#include "NKSceneNode.h"

shared_ptr<NKBatchNode> NKBatchNode::meshNodeWithPrimitive(NKPrimitive primitive, shared_ptr<NKTexture> texture, NKColor color, V3t size){
    return make_shared<NKBatchNode>(primitive, texture, color, size);
}

shared_ptr<NKBatchNode> NKBatchNode::meshNodeWithObj(string objName, V3t size, bool normalize, bool anchor){
    return make_shared<NKBatchNode>(objName, size, normalize, anchor);
}

shared_ptr<NKBatchNode> NKBatchNode::meshNodeWithVbo(shared_ptr<NKVertexBuffer> buffer, GLenum drawMode, shared_ptr<NKTexture> texture, NKColor color, V3t size) {
    return make_shared<NKBatchNode>(buffer, drawMode, texture, color, size);
}

void NKBatchNode::chooseShader(){

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    // These don't support instanced drawing right now
    return;
#endif

    if (!hitShader){
        hitShader = NKShaderProgram::shaderNamed("b_HitShader",NKS_COLOR_MODE_UNIFORM ,0,0,NK_BATCH_SIZE);
    }
    if (_numTextures) {
#warning video tex
        //        if ([_textures[0] isKindOfClass:[NKVideoTexture class]]) {
        //            self.shader = [NKShaderProgram newShaderNamed:@"b_videoTextureShader" colorMode:NKS_COLOR_MODE_UNIFORM numTextures:-1 numLights:1 withBatchSize:NK_BATCH_SIZE];
        //        }
        //        else {
        _shader = NKShaderProgram::shaderNamed("b_ColorTextureLightShader",NKS_COLOR_MODE_UNIFORM,_numTextures,1 ,NK_BATCH_SIZE);
        //        }
    }

    else {
        _shader = NKShaderProgram::shaderNamed("b_colorLightShader",NKS_COLOR_MODE_UNIFORM,_numTextures,1,NK_BATCH_SIZE);
    }
}

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
void NKBatchNode::setTexture(shared_ptr<NKTexture> texture) {
    for (auto& node : _children){
        static_pointer_cast<NKMeshNode>(node)->setTexture(texture);
    }
}
#endif

void NKBatchNode::drawChildren(){
#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    // These don't support instanced drawing right now
    NKMeshNode::drawChildren();
    return;
#endif
    // handled in batch pass
}

void NKBatchNode::customDraw(){

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
    // These don't support instanced drawing right now
    return;
#endif

    pushStyle();

    bool useColor = false;

    if (scene()->boundVertexBuffer != _vertexBuffer.get()) {
        _vertexBuffer->bind();
        scene()->boundVertexBuffer = _vertexBuffer.get();
    }

    bindTextures();

    mvpStack.clear();
    mvStack.clear();
    normalStack.clear();

    if (scene()->activeShader()->hasUniformNamed(NKS_V4_COLOR)) {
        useColor = true;
        childColors.clear();
    }

    vector<shared_ptr<NKNode>> toDraw;

    // NKLogV3("Camera view matrix", V3GetM16Translation(scene()->camera->viewMatrix()));

    for (int i = 0; i < _children.size(); i++) {
        auto child = _children[i];

        child->modelViewCache = M16Multiply(scene()->camera->viewMatrix(),M16ScaleWithV3(child->globalTransform(), child->size()));
        //child->modelViewCache = M16Multiply(scene()->camera->viewMatrix(), child->globalTransform());

        // FAST CULL
        if (child->modelViewCache.m32 < 0) {
            toDraw.push_back(child);
            //NKLogV3("cube pos: ", V3GetM16Translation(M16Multiply(scene()->camera->viewMatrix(), child->globalTransform())));
        }
        else {
            // nkLog("cull obj 'z' %1.2f", child->modelViewCache.m32);
        }

        //if (rand() % 100 > 90) {

        //}


        //child->modelViewProjectionCache = M16Multiply(scene()->camera->projectionMatrix(), child->modelViewCache);
        //child->cameraDistance = V3Distance(scene()->camera->globalPosition(), child->globalPosition());
    }

    sort(toDraw.begin(), toDraw.end(),
            [](const shared_ptr<NKNode> a, const shared_ptr<NKNode> b)
            {
                return a->modelViewCache.m32 < b->modelViewCache.m32;
                //return a->modelViewProjectionCache.m32 > b->modelViewProjectionCache.m32;
                //return a->cameraDistance > b->cameraDistance;
            });

    //    _children = [_children sortedArrayUsingComparator:^NSComparisonResult(NKNode * a, NKNode * b) {
    //                 return a.modelViewCache.m32 > b.modelViewCache.m32;
    //                 }];

    for (int i = 0; i < toDraw.size(); i++) {
        auto child = toDraw[i];

        mvpStack.append(M16Multiply(scene()->camera->projectionMatrix(), child->modelViewCache));
        //mvpStack.append(child->modelViewProjectionCache);

        if (useColor) {
            childColors.append(child->glColor());
        }
    }

    if (scene()->lights.size()) {
        for (auto child : toDraw){
            mvStack.append(child->modelViewCache);
            normalStack.append(M16GetInverseNormalMatrix(child->modelViewCache));
        }
    }

    int startingSprite = 0;
    int spritesInBatch = 0;

    for (int i = 0; i < toDraw.size(); i++) {
        spritesInBatch++;
        if (spritesInBatch == NK_BATCH_SIZE || i == toDraw.size() - 1) {
            drawGeometry(startingSprite, spritesInBatch, useColor);
            startingSprite += spritesInBatch;
            spritesInBatch = 0;
        }
    }

}


void NKBatchNode::drawWithHitShader() {

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
// These don't support instanced drawing right now
    return;
#endif

    auto temp = scene()->hitDetectShader.get();

    scene()->setActiveShader(hitShader.get());

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
        childColors.append(child->uidColor->c4Color());

        spritesInBatch++;

        if (spritesInBatch == NK_BATCH_SIZE || i == _children.size() - 1) {
            drawGeometry(startingSprite, spritesInBatch, true);

            mvpStack.clear();
            childColors.clear();

            spritesInBatch = 0;
        }
    }

    scene()->setActiveShader(temp);
    scene()->activeShader()->use();
}


void NKBatchNode::drawGeometry(U1t start, U1t spritesInBatch, bool useColor){

    scene()->activeShader()->uniformNamed(NKS_M16_MVP).bindM16Array(mvpStack.data()+start, spritesInBatch);

    if (scene()->lights.size()) {

        scene()->activeShader()->uniformNamed(NKS_M16_MV).bindM16Array(mvStack.data()+start, spritesInBatch);

        scene()->activeShader()->uniformNamed(NKS_M9_NORMAL).bindM9Array(normalStack.data()+start, spritesInBatch);
    }

    if (useColor) {
        scene()->activeShader()->uniformNamed(NKS_V4_COLOR).bindV4Array(childColors.data()+start, spritesInBatch);
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
#else
#ifdef NK_USE_ARB_EXT
        glDrawArraysInstancedARB(_drawMode, 0, (int)_vertexBuffer->size(), spritesInBatch);
#else
        glDrawArraysInstanced(_drawMode, 0, _vertexBuffer->size(), spritesInBatch);
#endif
#endif
    }


}

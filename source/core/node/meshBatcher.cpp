//
//  MeshBatcher.m
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#include "meshBatcher.h"
#include "shaderProgram.h"
#include "scene.h"

using namespace std;
using namespace Shader;

shared_ptr<MeshBatcher> MeshBatcher::nodeWithPrimitive(NKPrimitive primitive, shared_ptr<Texture> texture, Color color, V3t size){
  shared_ptr<MeshBatcher> ret (new MeshBatcher(primitive, texture, color, size));
  return ret;
}

shared_ptr<MeshBatcher> MeshBatcher::nodeWithObj(string objName, V3t size, bool normalize, bool anchor){
  shared_ptr<MeshBatcher> ret (new MeshBatcher(objName, size, normalize, anchor));
  return ret;
}

shared_ptr<MeshBatcher> MeshBatcher::nodeWithVbo(shared_ptr<VertexBuffer> buffer, GLenum drawMode, shared_ptr<Texture> texture, Color color, V3t size) {
  shared_ptr<MeshBatcher> ret (new MeshBatcher(buffer, drawMode, texture, color, size));
  return ret;
}

void MeshBatcher::chooseShader() {
#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
  // These don't support instanced drawing right now
  return;
#endif
  if (!hitShader){
    hitShader = Program::shaderNamed("b_HitShader",NKS_COLOR_MODE_UNIFORM ,0,0,NK_BATCH_SIZE);
  }
  if (_numTextures) {
    if (_is2d) {
      shader = Program::shaderNamed("spriteBatch", {
        ShaderModule::batchSpritePositionModule(NK_BATCH_SIZE),
        ShaderModule::textureModule(1),
        ShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, NK_BATCH_SIZE)}, NK_BATCH_SIZE);
    }
    else {
      shader = Program::shaderNamed("b_ColorTextureLightShader",NKS_COLOR_MODE_UNIFORM,_numTextures,1 ,NK_BATCH_SIZE);
    }
  }
  else {
    shader = Program::shaderNamed("b_colorLightShader",NKS_COLOR_MODE_UNIFORM,_numTextures,1,NK_BATCH_SIZE);
  }
}

#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
void MeshBatcher::setTexture(shared_ptr<Texture> texture) {
  for (auto& node : children()){
    static_pointer_cast<Mesh>(node)->setTexture(texture);
  }
}
#endif

void MeshBatcher::drawChildren() {
#if (TARGET_RASPBERRY_PI || TARGET_EMSCRIPTEN)
  // These don't support instanced drawing right now
  Mesh::drawChildren();
  return;
#endif
  // handled in batch pass
}

void MeshBatcher::customDraw() {

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
  if (GLState::activeShader()->hasUniformNamed(NKS_V4_COLOR)) {
    useColor = true;
    childColors.clear();
  }

  vector<Node*> toDraw;

  if (cullable() && !_is2d) { // 99% only set non cullable on skybox etc.
    for (auto child : allChildren()){
      // FAST CULL
      if (child->modelViewMatrix.get().m32 <= 0 || !child->cullable()) {
        toDraw.push_back(child);
      }
    }
    if (toDraw.size() > 1) {
      sort(toDraw.begin(), toDraw.end(), nearestOpaqueThenFurthestTransparent);
    }
  }
  else {
    getAllChildren(toDraw);
  }

  if (_is2d) {
    for (auto child : toDraw){
      positionStack.append(child->globalPosition().xy);
      scaleStack.append(child->size.get().x * child->scale.get().x);
    }
  }
  else {
    for (auto child : toDraw){
      mvpStack.append(child->modelViewProjectionMatrix.get());
      if (scene()->lights.size()) {
        mvStack.append(child->modelViewMatrix.get());
        normalStack.append(M16GetInverseNormalMatrix(child->modelViewMatrix.get()));
      }
    }
  }

  if (useColor) {
    for (auto child : toDraw){
      childColors.append(child->glColor());
    }
  }

  if (_is2d) {
    auto mvp = M16Multiply(scene()->camera->viewProjectionMatrix.get(), M16ScaleWithV3(globalTransform.get(), V3(1.0)));
    GLState::activeShader()->uniformNamed(NKS_M16_MVP).bindM16(mvp);
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

  for (int i = 0; i < children().size(); i++) {
    auto& child = children()[i];

    mvpStack.append(child->modelViewProjectionMatrix.get());
    childColors.append(Color{child->uidColor});

    spritesInBatch++;

    if (spritesInBatch == NK_BATCH_SIZE || i == children().size() - 1) {
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
    GLState::activeShader()->uniformNamed(NKS_V2_BATCH_POSITION).bindV2Array(positionStack.data()+start, spritesInBatch);
    GLState::activeShader()->uniformNamed(NKS_F1_BATCH_SCALE).bindF1Array(scaleStack.data()+start, spritesInBatch);
  }
  else {
    GLState::activeShader()->uniformNamed(NKS_M16_MVP).bindM16Array(mvpStack.data()+start, spritesInBatch);

    if (scene()->lights.size()) {
      GLState::activeShader()->uniformNamed(NKS_M16_MV).bindM16Array(mvStack.data()+start, spritesInBatch);
      GLState::activeShader()->uniformNamed(NKS_M9_NORMAL).bindM9Array(normalStack.data()+start, spritesInBatch);
    }
  }

  if (useColor) {
    GLState::activeShader()->uniformNamed(NKS_V4_COLOR).bindV4Array(childColors.data()+start, spritesInBatch);
  }

  if (_primitiveType == NKPrimitiveLODSphere) {
    nkDrawArraysInstanced(_drawMode, _vertexBuffer->elementOffset[0], _vertexBuffer->elementSize[0], spritesInBatch);
  }
  else {
    nkDrawArraysInstanced(_drawMode, 0, (int)_vertexBuffer->size(), spritesInBatch);
  }

#if NK_LOG_METRICS
  Scene::draws++;
#endif
}

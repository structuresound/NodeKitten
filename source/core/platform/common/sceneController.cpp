//
//  GLView.m
//  Wavefront OBJ Loader
//
//  Created by Jeff LaMarche on 12/14/08.
//  Copyright Jeff LaMarche 2008. All rights reserved.
//


#include "sceneController.h"
#include "../../gl/texture.h"
#include "../../gl/frameBuffer.h"
#include "../../gl/shaderProgram.h"
#include "../../node/mesh.h"
#include "../../view/scene.h"
#include "../../util/time.h"
#include "uxEvent.h"

using namespace std;

SceneController::SceneController(S2t size_) {
  size.set(size_);
  nkLog("init view : %1.2f , %1.2f", size_.width, size_.height);
}

void SceneController::afterResize(){
  if (_scene) _scene->size.set(size.get());
}

void SceneController::setScene(shared_ptr<Scene>scene){
  if (_scene) {
    _scene->moveFromView();
    _scene.reset();
  }

  _scene = scene;
  _scene->setView(this);

  lastTime = Time::getCurrentTime();
}

void SceneController::draw() {

  GLState::glInit();

  if (!_scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, size.get().width, size.get().height);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    return;
  }

  if (_scene->hitQueue.size()) {
    _scene->processHitBuffer();
  }

  if (framebuffer){
    framebuffer->bind();
  }
  else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, _scene->size.get().width, _scene->size.get().height);

  if (_scene) {
    D1t now = Time::getCurrentTime();
    _scene->updateWithTimeSinceLast((F1t)(now - lastTime));
    lastTime = now;
    _scene->draw();
  }

  if (framebuffer){
    framebuffer->unbind();
  }

  while (Dispatch::uiQueue.size()) {
    Dispatch::uiQueue.front()();
    Dispatch::uiQueue.pop_front();
  }
}

void SceneController::handleUXEvent(shared_ptr<UXEvent> event){
  event->node = _scene.get();
  _scene->handleUXEvent(event);
}

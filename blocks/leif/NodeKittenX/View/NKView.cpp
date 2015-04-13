//
//  GLView.m
//  Wavefront OBJ Loader
//
//  Created by Jeff LaMarche on 12/14/08.
//  Copyright Jeff LaMarche 2008. All rights reserved.
//


#include "NKView.h"
#include "NKTextureManager.h"
#include "NKTexture.h"
#include "NKShaderProgram.h"
#include "NKMeshNode.h"
#include "NKSceneNode.h"
#include "NKTime.h"
#include "NKEvent.h"

NKView::NKView(S2t size) : _size(size) {
    
}

void NKView::setScene(shared_ptr<NKSceneNode>scene){
    if (_scene) {
        _scene->moveFromView();
        _scene.reset();
    }
    
    _scene = scene;
    _scene->setView(this);
    _scene->moveToView();
    
    lastTime = Time::getCurrentTime();
}

void NKView::setSize(S2t size){
    if (_scene) _scene->setSize(size);
    _size = size;
}

void NKView::draw() {
    
    if (!_scene) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, _size.width, _size.height);
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
    glViewport(0, 0, _scene->size().width, _scene->size().height);
    
    if (_scene) {
        
        D1t now = Time::getCurrentTime();
        _scene->updateWithTimeSinceLast((F1t)(now - lastTime));
        lastTime = now;
        
        _scene->draw();
    }
    
    
}

void NKView::handleEvent(NKEvent &event){
    event.node = _scene.get();
    _scene->handleEvent(event);
}

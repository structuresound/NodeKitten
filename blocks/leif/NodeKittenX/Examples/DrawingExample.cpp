//
//  DrawingExample.cpp
//  Pods
//
//  Created by Leif Shackelford on 5/17/15.
//
//

#include "DrawingExample.h"

void DrawingExample::moveToView() {
    camera->setProjectionMode(NKProjectionModeOrthographic);
    
    cache = (View::viewWithSize(size.get().xy));
    cache->setBackgroundColor(Color(1,.3,0,1));
    addChild(cache);
  
    canvas = Node::node();
    addChild(canvas);
  
    cache->setShouldRasterize(true);
}

void DrawingExample::afterResize() {
  if (cache){
    cache->size.set(size.get().xy);
  }
}

void DrawingExample::addControlPointWithUXEvent(shared_ptr<UXEvent> event){
    if (!event->referenceNode) {
        auto store = View::viewWithSize(size.get().xy);
        canvas->addChild(store);
        event->referenceNode = store;
        
        auto layer = SpriteEmitter::nodeWithTexture(Texture::textureWithImageFile("soft64.png"), Color(0));
        store->addChild(layer);
        layer->position.set(event->screenLocation - (scene()->size.get().xy / 2.0));
        
        // DO FIRST POINT
        auto cp = Node::node(1,1);
        layer->addChild(cp);
        
        auto point = Node::node(V3(3));
        
        cp->addChild(point);
    }
    else {
        auto& lastCP = event->referenceNode->child()->children().back();
        auto& lastPoint = lastCP->children().back();
        
        auto delta = event->screenLocation - event->previousScreenLocation;
        
        F1t velocity = sqrtf(event->avgVelocity().length());
        F1t brushScale = 10.0;
        
        if (event->scale.x < 1.0) event->scale.x = 1.0;
        
        F1t targetSize = (velocity+.2) * brushScale * event->scale.x;
        F1t density = .6 / Platform::Screen::scale();
        
        auto cp = Node::node(Color(0), V3(1));
        cp->position.set(lastCP->position.get() + lastPoint->position.get());
        event->referenceNode->child()->addChild(cp);
        
        int numSteps = (delta.length() * density) + 1;
        
        for (int i = 0; i < numSteps; i++){
            F1t mu = (i+1) / (F1t)numSteps;
            auto point = Node::node(Color(0),V3(getTween(lastPoint->size.get().x, targetSize , mu)));
            cp->addChild(point);
            point->position.set(delta * mu);
            point->scale.set(1.0 - mu);
        }
    }
}

void DrawingExample::smoothLastPointForLayer(const shared_ptr<Node>& layer){
    // SMOOTH LAST POINT
    if (layer->children().size() > 3) {
        for (auto i = layer->children().size()-3; i < layer->children().size()-1; i++) {
            auto& cp = layer->children()[i];
            
            auto y0 = MAX(0,MIN(i-1,layer->children().size()-1));
            auto y1 = MAX(0,MIN(i,layer->children().size()-1));
            auto y2 = MAX(0,MIN(i+1,layer->children().size()-1));
            auto y3 = MAX(0,MIN(i+2,layer->children().size()-1));
            
            for (int c = 0; c < cp->children().size(); c++){
                F1t mu2 = (c+1) / (float)cp->children().size();
                auto& point = cp->children()[c];
                auto pos = getCatmullInterpolate(layer->children()[y0]->position.get().xy,
                                                 layer->children()[y1]->position.get().xy,
                                                 layer->children()[y2]->position.get().xy,
                                                 layer->children()[y3]->position.get().xy,
                                                 mu2);
                point->position.set(pos - cp->position.get().xy);
                point->scale.set(1.0);
            }
        }
    }
}

void DrawingExample::smoothPointsForLayer(const shared_ptr<Node>& layer){
    vector<Node*> release;
    
    // SMOOTHING
    for (int i = 0; i < layer->children().size(); i++) {
        auto& cp = layer->children()[i];
        
        auto y0 = MAX(0,MIN(i-1,layer->children().size()-1));
        auto y1 = MAX(0,MIN(i,layer->children().size()-1));
        auto y2 = MAX(0,MIN(i+1,layer->children().size()-1));
        auto y3 = MAX(0,MIN(i+2,layer->children().size()-1));
        
        for (int c = 0; c < cp->children().size(); c++){
            F1t mu2 = (c+1) / (F1t)cp->children().size();
            auto& point = cp->children()[c];
            auto pos = getCatmullInterpolate(layer->children()[y0]->position.get().xy,
                                             layer->children()[y1]->position.get().xy,
                                             layer->children()[y2]->position.get().xy,
                                             layer->children()[y3]->position.get().xy,
                                             mu2);
            point->position.set(pos - cp->position.get().xy);
            point->scale.set(1.0);
        }
        
        if (i >= layer->children().size() -3) {
            for (auto& node : cp->children()) {
                release.push_back(node.get());
            }
        }
    }
    
    for (int i = 0; i < release.size(); i++) {
        F1t mu = (i+1) / (F1t)release.size();
        release[i]->scale.set(1.0f - mu);
    }
}

void DrawingExample::addToCache(shared_ptr<Node>& node){
    if (!cache->children().size() || cache->children().back()->allChildren().size() > 2000){
        auto newLayer = View::viewWithSize(size.get().xy);
        newLayer->setShouldRasterize(true);
        cache->addChild(newLayer);
    }
    cache->children().back()->addChild(node);
}

void DrawingExample::handleUXEvent(shared_ptr<UXEvent> event){
    if (event->type == UXEventTypeTap){
        canvas->removeAllChildren();
        cache->removeAllChildren();
    }
    else if (event->phase == UXEventPhaseBegin){
        addControlPointWithUXEvent(event);
    }
    else if (event->phase == UXEventPhaseDrag){
        addControlPointWithUXEvent(event);
        smoothLastPointForLayer(event->referenceNode->child());
    }
    else if (event->phase == UXEventPhaseEnd){
        addControlPointWithUXEvent(event);
        smoothPointsForLayer((event->referenceNode)->child());
        addToCache(event->referenceNode);
    }
}
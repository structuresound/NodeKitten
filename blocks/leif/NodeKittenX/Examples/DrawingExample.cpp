//
//  DrawingExample.cpp
//  Pods
//
//  Created by Leif Shackelford on 5/17/15.
//
//

#include "DrawingExample.h"

void DrawingExample::moveToView() {
    removeAllChildren();
    
    backgroundColor = Color(1);
    
    camera->setProjectionMode(NKProjectionModeOrthographic);
    
    cache = View::viewWithSize(_size.xy);
    addChild(cache);
    
    canvas = View::viewWithSize(_size.xy);
    addChild(canvas);
    
    cache->setShouldRasterize(true);
}

void DrawingExample::addControlPointWithUXEvent(shared_ptr<UXEvent> event){
    if (!event->referenceNode) {
        auto store = View::viewWithSize(_size.xy);
        canvas->addChild(store);
        //
        event->referenceNode = store;
        
        auto layer = SpriteEmitter::nodeWithTexture(Texture::textureWithImageFile("soft64.png"), Color(0));
        store->addChild(layer);
        layer->setPosition(event->screenLocation - (scene()->size().xy / 2.0));
        
        // DO FIRST POINT
        auto cp = Node::node(Color(1), V3(1));
        layer->addChild(cp);
        
        auto point = Node::node(V3(3));
        
        cp->addChild(point);
    }
    else {
        auto lastCP = event->referenceNode->child()->children().back();
        auto lastPoint = lastCP->children().back();
        
        auto delta = event->screenLocation - event->previousScreenLocation;
        
        F1t velocity = sqrtf(event->avgVelocity().length());
        F1t brushScale = 10.0;
        
        if (event->scale.x < 1.0) event->scale.x = 1.0;
        
        F1t targetSize = (velocity+.2) * brushScale * event->scale.x;
        F1t density = .6 / Platform::Screen::scale();
        
        auto cp = Node::node(Color(0), V3(1));
        event->referenceNode->child()->addChild(cp);
        cp->setPosition(lastCP->position() + lastPoint->position());
        
        int numSteps = (delta.length() * density) + 1;
        
        for (int i = 0; i < numSteps; i++){
            F1t mu = (i+1) / (F1t)numSteps;
            auto point = Node::node(Color(0),V3(getTween(lastPoint->size().x, targetSize , mu)));
            cp->addChild(point);
            point->setPosition(delta * mu);
            point->setScale(1.0 - mu);
        }
    }
}

void DrawingExample::smoothLastPointForLayer(shared_ptr<Node> layer){
    // SMOOTH LAST POINT
    if (layer->children().size() > 3) {
        for (int i = layer->children().size()-3; i < layer->children().size()-1; i++) {
            auto cp = layer->children()[i];
            
            int y0 = MAX(0,MIN(i-1,layer->children().size()-1));
            int y1 = MAX(0,MIN(i,layer->children().size()-1));
            int y2 = MAX(0,MIN(i+1,layer->children().size()-1));
            int y3 = MAX(0,MIN(i+2,layer->children().size()-1));
            
            for (int c = 0; c < cp->children().size(); c++){
                F1t mu2 = (c+1) / (float)cp->children().size();
                auto point = cp->children()[c];
                auto pos = getCatmullInterpolate(layer->children()[y0]->position().xy,
                                                 layer->children()[y1]->position().xy,
                                                 layer->children()[y2]->position().xy,
                                                 layer->children()[y3]->position().xy,
                                                 mu2);
                point->setPosition(pos - cp->position().xy);
                point->setScale(1.0);
            }
        }
    }
}

void DrawingExample::smoothPointsForLayer(shared_ptr<Node> layer){
    vector<shared_ptr<Node>> release;
    
    // SMOOTHING
    for (int i = 0; i < layer->children().size(); i++) {
        auto cp = layer->children()[i];
        
        int y0 = MAX(0,MIN(i-1,layer->children().size()-1));
        int y1 = MAX(0,MIN(i,layer->children().size()-1));
        int y2 = MAX(0,MIN(i+1,layer->children().size()-1));
        int y3 = MAX(0,MIN(i+2,layer->children().size()-1));
        
        for (int c = 0; c < cp->children().size(); c++){
            F1t mu2 = (c+1) / (F1t)cp->children().size();
            auto point = cp->children()[c];
            auto pos = getCatmullInterpolate(layer->children()[y0]->position().xy,
                                             layer->children()[y1]->position().xy,
                                             layer->children()[y2]->position().xy,
                                             layer->children()[y3]->position().xy,
                                             mu2);
            point->setPosition(pos - cp->position().xy);
            point->setScale(1.0);
        }
        
        if (i >= layer->children().size() -3) {
            for (auto node : cp->children()) {
                release.push_back(node);
            }
        }
    }
    
    for (int i = 0; i < release.size(); i++) {
        F1t mu = (i+1) / (F1t)release.size();
        release[i]->setScale(1.0 - mu);
    }
}

void DrawingExample::addToCache(shared_ptr<View> node){
    if (!cache->children().size() || cache->children().back()->allChildren().size() > 2000){
        auto node = View::viewWithSize(_size.xy);
        node->setShouldRasterize(true);
        cache->addChild(node);
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
        smoothPointsForLayer(event->referenceNode->child());
        addToCache(static_pointer_cast<View>(event->referenceNode));
    }
}
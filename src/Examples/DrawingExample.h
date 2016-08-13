//
//  Text3DExample.h
//  Pods
//
//  Created by Leif Shackelford on 5/2/15.
//
//

#ifndef __DrawingExample__
#define __DrawingExample__

#include "NodeKitten.h"

class DrawingExample : public Scene {
  
    std::vector<Node*>controlPoints;
  
public:
    DrawingExample(S2t size) : Scene(size){};
    
    void moveToView() override;
    void afterResize() override;
  
    void handleUXEvent(shared_ptr<UXEvent> event) override;
    
    shared_ptr<View> cache;
    shared_ptr<Node> canvas;
  
    void addControlPointWithUXEvent(shared_ptr<UXEvent> event);
    
    void smoothLastPointForLayer(const shared_ptr<Node>& layer);
    void smoothPointsForLayer(const shared_ptr<Node>& layer);
    
    void addToCache(shared_ptr<Node>& node);
  
};


//class ControlPointEmitter : public SpriteEmitter {
//public:
//    using SpriteEmitter::SpriteEmitter;
//    
//    static shared_ptr<ControlPointEmitter> nodeWithTexture(shared_ptr<Texture> texture, Color color) {
//        return make_shared<ControlPointEmitter>(texture, color);
//    }
//    
//    vector<Node*> controlPoints;
//};

#endif /* defined(__Pods__Text3DExample__) */

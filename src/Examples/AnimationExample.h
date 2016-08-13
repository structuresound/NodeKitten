//
//  AnimationExample.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/26/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __NodeKittenX__AnimationExample__
#define __NodeKittenX__AnimationExample__

#include "NodeKitten.h"

class AnimationExample : public Scene {
public:
    
    AnimationExample(S2t size) : Scene(size){};
    
    void moveToView() override;
    void handleUXEvent(shared_ptr<UXEvent> event) override;
};

#endif /* defined(__NodeKittenX__AnimationExample__) */

//
//  ExampleScene.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/26/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __NodeKittenX__ExampleScene__
#define __NodeKittenX__ExampleScene__

#include "NodeKitten.h"

class ExampleScene : public NKSceneNode {
public:
    
    ExampleScene(S2t size) : NKSceneNode(size){};
    
    void moveToView() override;
    void handleEvent(NKEvent& event) override;
};

#endif /* defined(__NodeKittenX__ExampleScene__) */

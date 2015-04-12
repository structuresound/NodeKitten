//
//  NKEvent.h
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 6/13/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#ifndef __NKEvent_h_
#define __NKEvent_h_

#include "NKTypes.h"

typedef enum NKEventPhase {
    NKEventPhaseNone,
    NKEventPhaseBegin,
    NKEventPhaseMove,
    NKEventPhaseEnd,
    NKEventPhaseDrag,
    NKEventPhaseDoubleTap
} NKEventPhase;

class NKNode;
class NKEvent;

typedef std::function<void(NKEvent&)> EventBlock;

class NKEvent {
    
public:

    NKEvent();

    P2t startingScreenLocation;
    V3t startingWorldLocation;
    P2t screenLocation;
    
    V3t worldLocation;
    
    P2t scale;
    
    NKEventPhase phase;
    
    V3t linearVelocity;
    
    NKNode* node;
    
    void *systemEvent;
    
};

#endif


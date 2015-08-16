//
//  UXEvent.h
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 6/13/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#ifndef __UXUXEvent_h_
#define __UXUXEvent_h_

#include "CommonTypes.h"
#include "CommonUtils.h"

typedef enum UXEventPhase {
    UXEventPhaseNone,
    UXEventPhaseBegin,
    UXEventPhaseDrag,
    UXEventPhaseMove,
    UXEventPhaseEnd,
    UXEventPhaseCancel,
    
} UXEventPhase;

typedef enum UXEventType {
    UXEventTypeUnkown,
    UXEventTypeTap,
    UXEventTypePan,
    UXEventTypePinch,
    UXEventTypeSwipe
} UXEventType;

class Node;
class UXUXEvent;

class UXEvent {
    
    V2t _screenVelocityAlpha {0,0};
    V3t _worldVelocityAlpha {0,0,0};
    
public:

    UXEvent();
    ~UXEvent();
    
    V2t startingScreenLocation;
    V2t previousScreenLocation;
    V2t screenLocation;

    V2t scale {1.0,1.0};
    
    double initialTimeStamp;
    double previousTimeStamp;
    double timeStamp;
    
    double age(){return timeStamp - initialTimeStamp;};
    
    V2t screenVelocity(){
        auto dt = (timeStamp - previousTimeStamp);
        if (dt <= 0) return V2t{0,0};
        return (screenLocation - previousScreenLocation) / (dt * 1000.0);
    }
    
    V2t avgVelocity(){
        return _screenVelocityAlpha = (_screenVelocityAlpha * 0.95) + (screenVelocity() * 0.05);
    }
    
    F1t screenRotation(){
        return RAD_TO_DEG((screenLocation - previousScreenLocation).rotation());
    }
    
    UXEventPhase phase {UXEventPhaseBegin};
    UXEventType type {UXEventTypeUnkown};
    
    Node* node;
    std::shared_ptr<Node> referenceNode;
    void *referenceData;
    
    virtual void* id(){return this;};
    
#pragma mark - DEBUG
    
    static unsigned int createdObjects;
    static unsigned int destroyedObjects;
    static unsigned int liveObjects(){return createdObjects - destroyedObjects;};
};

#endif
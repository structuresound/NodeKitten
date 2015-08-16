//
//  UXEvent.m
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 6/13/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#include "UXEvent.h"

unsigned int UXEvent::createdObjects {0};
unsigned int UXEvent::destroyedObjects {0};

UXEvent::UXEvent(){
    createdObjects++;
}

UXEvent::~UXEvent(){
    destroyedObjects++;
}
//
//  NKAppleEvent.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/17/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//


#ifndef __NodeKittenX__NKAppleEvent__
#define __NodeKittenX__NKAppleEvent__

#include "NKEvent.h"

#if (TARGET_IOS || TARGET_OSX)

#if TARGET_IOS
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif

class NKAppleEvent : public NKEvent {
    
public:
#if TARGET_IOS
    UITouch *event;
    NKAppleEvent(UITouch* touch_){event = touch_;};
#endif
#if TARGET_OSX
    NSEvent *event;
    NKAppleEvent(NSEvent* event_){event = event_;};
#endif
    
};

#endif

#endif /* defined(__NodeKittenX__NKAppleEvent__) */


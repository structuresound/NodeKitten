//
//  CocoaUXEvent.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/17/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#pragma once

#include "../common/uxEvent.h"

#if (TARGET_IOS || TARGET_OSX)

//typedef NS_ENUM(NSUInteger, NSEventType) {        /* various types of events */
//    NSLeftMouseDown             = 1,
//    NSLeftMouseUp               = 2,
//    NSRightMouseDown            = 3,
//    NSRightMouseUp              = 4,
//    NSMouseMoved                = 5,
//    NSLeftMouseDragged          = 6,
//    NSRightMouseDragged         = 7,
//    NSMouseEntered              = 8,
//    NSMouseExited               = 9,
//    NSKeyDown                   = 10,
//    NSKeyUp                     = 11,
//    NSFlagsChanged              = 12,
//    NSAppKitDefined             = 13,
//    NSSystemDefined             = 14,
//    NSApplicationDefined        = 15,
//    NSPeriodic                  = 16,
//    NSCursorUpdate              = 17,
//    NSScrollWheel               = 22,
//    NSTabletPoint               = 23,
//    NSTabletProximity           = 24,
//    NSOtherMouseDown            = 25,
//    NSOtherMouseUp              = 26,
//    NSOtherMouseDragged         = 27,
//    /* The following event types are available on some hardware on 10.5.2 and later */
//    NSEventTypeGesture NS_ENUM_AVAILABLE_MAC(10_5)       = 29,
//    NSEventTypeMagnify NS_ENUM_AVAILABLE_MAC(10_5)       = 30,
//    NSEventTypeSwipe   NS_ENUM_AVAILABLE_MAC(10_5)       = 31,
//    NSEventTypeRotate  NS_ENUM_AVAILABLE_MAC(10_5)       = 18,
//    NSEventTypeBeginGesture NS_ENUM_AVAILABLE_MAC(10_5)  = 19,
//    NSEventTypeEndGesture NS_ENUM_AVAILABLE_MAC(10_5)    = 20,
//
//#if __LP64__
//    NSEventTypeSmartMagnify NS_ENUM_AVAILABLE_MAC(10_8) = 32,
//#endif
//    NSEventTypeQuickLook NS_ENUM_AVAILABLE_MAC(10_8) = 33,
//
//#if __LP64__
//    NSEventTypePressure NS_ENUM_AVAILABLE_MAC(10_10_3) = 34
//#endif
//};


#if TARGET_IOS
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif

class CocoaUXEvent : public UXEvent {
#if TARGET_IOS
  UITouch *_event;
#else
  NSEvent *_event;
#endif
public:
#if TARGET_IOS
  void* id() override{
    return (__bridge void*)_event;
  }
#else
  void* id() override{
    switch (_event.type){
      case NSLeftMouseDown: case NSLeftMouseDragged: case NSLeftMouseUp:
        return (char*)0;
      default: return (char*)rand();
    }
  }
#endif
#if TARGET_IOS
  CocoaUXEvent(UITouch* touch){_event = touch;};
#endif
#if TARGET_OSX
  CocoaUXEvent(NSEvent* event){_event = event;};
  ~CocoaUXEvent(){};
#endif
};

#endif

//
//  CocoaGLManager.h
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 6/14/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#pragma once

#include "../../util/util.h"

#if (TARGET_IOS || TARGET_OSX)

#if TARGET_IOS
#import <UIKit/UIKit.h>
#import <CoreImage/CoreImage.h>
#elif TARGET_OSX
#import <AppKit/AppKit.h>
#import <QuartzCore/QuartzCore.h>
#endif

@interface CocoaGLManager : NSObject
{
    CIContext* _ciContext;
}

#if TARGET_OS_IPHONE
@property (nonatomic, weak) EAGLContext *context;
#else
@property (nonatomic, weak) NSOpenGLContext *context;
@property (nonatomic, strong) NSOpenGLPixelFormat *pixelFormat;
#endif

+ (CocoaGLManager *)sharedInstance;
+ (void)updateWithTimeSinceLast:(F1t) dt;
+ (CIContext*)ciContext;

@end

#endif
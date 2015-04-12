//
//  NKAppleViews.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/23/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __NKAppleViews__
#define __NKAppleViews__

#include "NKTypes.h"
#include "NKView.h"
#include "NKFrameBuffer.h"

#if (TARGET_IOS || TARGET_OSX)

#include "NKAppleEvent.h"

#if TARGET_IOS

@class NKUIView;

@interface NKUIViewController : UIViewController

-(NKUIView*) NKView;

@end

#define NKDisplayLink CADisplayLink *
#define NK_NONATOMIC_IOSONLY nonatomic

@interface NKUIView : UIView
{
    EAGLContext *context;
    CADisplayLink *displayLink;
    
    NSTimeInterval lastTime;
    
    
    BOOL controllerSetup;
    bool animating;
    
    // 2.0 stuff
    GLuint _program;
    float _rotation;
    
}

@property (nonatomic)  NKView *nkView;
@property (nonatomic) float mscale;

-(void)startAnimation;
-(void)stopAnimation;

-(id)initGLES;

- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;

@end

#endif

#if TARGET_OSX

#define NKDisplayLink CVDisplayLinkRef

#define NK_NONATOMIC_IOSONLY atomic

// Attribute index.

#define USE_CV_DISPLAY_LINK 0

@class NKNSView;

@interface NKNSViewController : NSViewController

-(NKNSView*)NKView;

@end

@interface NKNSView : NSOpenGLView

{
    
#if USE_CV_DISPLAY_LINK
    static dispatch_queue_t displayThread;
    CVDisplayLinkRef displayLink;
#else
    NSTimer *displayTimer;
#endif
    
    BOOL controllerSetup;
    bool animating;
    
    // 2.0 stuff
    GLuint _program;
    float _rotation;
    
}

@property (nonatomic)  NKView *nkView;
@property (nonatomic, weak) NKNSViewController *controller;
@property (nonatomic) float mscale;

-(void)startAnimation;
-(void)stopAnimation;

/** uses and locks the OpenGL context */
-(void) lockOpenGLContext;

/** unlocks the openGL context */
-(void) unlockOpenGLContext;

/** returns the depth format of the view in BPP */
- (NSUInteger) depthFormat;

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime;

// private
+(void) load_;

@end

#endif

#endif

#endif
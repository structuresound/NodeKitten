//
//  NKAppleViews.m
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/23/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#include "NKAppleViews.h"
#include "NKFrameBuffer.h"
#include "NKGLManager.h"
#include "NKView.h"
#include "NKNode.h"

#if TARGET_IOS

#pragma mark -
#pragma mark - IOS
#pragma mark -

shared_ptr<NKFrameBuffer> genEAGLFrameBuffer(EAGLContext *context, id <EAGLDrawable> layer)

{
    //printf("GLES fb init with context %@\n", context);
    
    // 1 // Create the framebuffer and bind it.
    
    auto fbo = make_shared<NKFrameBuffer>();
    
    glGenFramebuffers(1, &fbo->glName);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->glName);
    
    printf("allocate gl buffer, %d\n", fbo->glName);
    // 2 // Create a color renderbuffer, allocate storage for it, and attach it to the framebuffer’s color attachment point.
    
    glGenRenderbuffers(1, &fbo->renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo->renderBuffer);
    
    printf("allocate gl buffer, %d\n", fbo->renderBuffer);
    
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &fbo->width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &fbo->height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fbo->renderBuffer);
    
    // 3 // Create a depth or depth/stencil renderbuffer, allocate storage for it, and attach it to the framebuffer’s depth attachment point.
    
    glGenRenderbuffers(1, &fbo->depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo->depthBuffer);
    
    printf("allocate gl depth buffer, %d\n", fbo->depthBuffer);
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fbo->width, fbo->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depthBuffer);
    
    // 4 // Test the framebuffer for completeness. This test only needs to be performed when the framebuffer’s configuration changes.
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        // NKCheckGLError(@"building frameBuffer\n");
        printf("failed to make complete framebuffer object %x\n", status);
    }
    
    return fbo;
}

@implementation NKUIViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

-(NKUIView*)nkView {
    return (NKUIView*)self.view;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
 {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end

@implementation NKUIView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

-(instancetype)initWithFrame:(CGRect)frame {
    
    if ((self = [super initWithFrame:frame]))
    {
        [self sharedInit];
    }
    return self;
    
}

- (id) initWithCoder:(NSCoder*)coder
{
    
    if ((self = [super initWithCoder:coder]))
    {
        [self sharedInit];
    }
    
    return self;
}

-(void)sharedInit {
    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    
    _mscale = 1.0f;

    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)]) {
        _mscale = [[UIScreen mainScreen] scale];
    }
    
    eaglLayer.contentsScale = _mscale;
    
    eaglLayer.opaque = TRUE;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    
    
#if NK_USE_GL3
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
#else
    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
#endif
    
    [[NKGLManager sharedInstance] setContext:context];
    
    if(!context){
        NSLog(@"failed to create EAGL context");
        return;
    }
    
    // DO SCENE
    
    _nkView = new NKView(V2Make(self.frame.size.width * _mscale, self.frame.size.height * _mscale));
    
    if (![self createFramebuffer]) {
        return;
    }
    else {
        NSLog(@"GLES Context && Frame Buffer loaded!");
    }

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(stopAnimation)
                                                 name:UIApplicationWillTerminateNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(stopAnimation)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(stopAnimation)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(startAnimation)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    
//    UITapGestureRecognizer *dt = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(doubleTap:)];
//    [dt setNumberOfTapsRequired:2];
//    [self addGestureRecognizer:dt];
    
}

-(void)layoutSubviews
{
    [super layoutSubviews];
    
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    NSLog(@"rebuilding framebuffer");
    [self createFramebuffer];
    
    [self drawView];
}


// Stop animating and release resources when they are no longer needed.


- (void)startAnimation
{
    if (!animating) {
        NSLog(@"Start animating");
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView)];
        [displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
        
        lastTime = CFAbsoluteTimeGetCurrent();
        animating = true;
    }
}

- (void)stopAnimation
{
    if (animating) {
        NSLog(@"Stop animating");
        [displayLink invalidate];
        animating = false;
    }
}

- (void)drawView
{
    if (_nkView){
        [EAGLContext setCurrentContext:context];
        
        _nkView->draw();
        
        glBindRenderbuffer(GL_RENDERBUFFER, _nkView->framebuffer->glName);
        [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
}

-(void)destroyFramebuffer {
    _nkView->framebuffer.reset();
}

-(BOOL) createFramebuffer {
    [EAGLContext setCurrentContext:context];
    
    _nkView->framebuffer = genEAGLFrameBuffer(context, (id<EAGLDrawable>)self.layer);
    
    if (_nkView->framebuffer) {
        return true;
    }
    
    NSLog(@"failed to create main ES framebuffer");
    return false;
}

- (void)dealloc
{
    [self stopAnimation];
    
    if([EAGLContext currentContext] == context)
    {
        [EAGLContext setCurrentContext:nil];
    }
    context = nil;
}

-(P2t)uiPointToNodePoint:(CGPoint)p {
    // port
    P2t size = P2Make(_nkView->size().width, _nkView->size().height);
    return V2Make(p.x*_mscale, size.height - (p.y*_mscale));
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *t in touches) {
        _nkView->events.push_back(NKEvent());
        
        _nkView->events.back().systemEvent = (__bridge void *)(t);
        _nkView->events.back().phase = NKEventPhaseBegin;
        _nkView->events.back().startingScreenLocation = [self uiPointToNodePoint:[t locationInView:self]];
        
        _nkView->handleEvent(_nkView->events.back());
    }
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *t in touches) {
        for (auto& e : _nkView->events) {
            if (e.systemEvent == (__bridge void *)(t)) {
                e.phase = NKEventPhaseMove;
                e.screenLocation = [self uiPointToNodePoint:[t locationInView:self]];
                
                _nkView->handleEvent(e);
            }
        }
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    
    for (UITouch *t in touches) {
        for (int i = 0; i < _nkView->events.size(); i++) {
            
            auto& e = _nkView->events[i];
            
            if (e.systemEvent == (__bridge void *)(t)) {
                
                e.phase = NKEventPhaseEnd;
                e.screenLocation = [self uiPointToNodePoint:[t locationInView:self]];
                //port
                e.node->handleEvent(e);
                _nkView->events.erase(_nkView->events.begin() + i);
            }
        }
    }
}

-(void)doubleTap:(UITapGestureRecognizer*)recognizer {
    // port
    //    NKEvent* event = [[NKEvent alloc]initWithTouch:nil];
    //    event.startingScreenLocation = [self uiPointToNodePoint:[recognizer locationInView:self]];
    //    event.phase = NKEventPhaseDoubleTap;
    //    [_scene dispatchEvent:event];
}


@end


#endif


#pragma mark -
#pragma mark - OS X
#pragma mark -


#if TARGET_OSX

@implementation NKNSViewController

#if NK_USE_GLES

- (void)viewDidLoad
{
    [super viewDidLoad];
    //
    if ([self.view isKindOfClass:[NKUIView class]]) {
        [(NKUIView*)self.view setController:self];
    }
    // Do any additional setup after loading the view.
}

-(void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [(NKUIView*)self.view startAnimation];
}

- (void)viewWillDisappear:(BOOL)animated {
    NSLog(@"view will dissappear");
    [(NKUIView*)self.view stopAnimation];
    [super viewWillDisappear:animated];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
#endif


@end


@implementation NKNSView

-(void)drawScene {
    
    int useFB = 0;
    
    if (_nkView){
    
        _nkView->draw();
    
    }

//        if (_nkView->framebuffer) {
//            
//            if (!_framebuffer) {
//                _framebuffer = [[NKFrameBuffer alloc] initWithWidth:_scene.size.width height:_scene.size.height];
//                rect = [[NKMeshNode alloc]initWithPrimitive:NKPrimitiveRect texture:_framebuffer.renderTexture color:NKWHITE size:V3Make(_scene.size.width, _scene.size.height, 1)];
//                
//                rect.shader = [NKShaderProgram newShaderNamed:@"fboDraw" colorMode:NKS_COLOR_MODE_NONE numTextures:1 numLights:0 withBatchSize:0];
//                
//                rect.forceOrthographic = true;
//                rect.usesDepth = false;
//                rect.cullFace = NKCullFaceFront;
//                rect.blendMode = NKBlendModeNone;
//            }
//            
//            [_framebuffer bind];
//            [_framebuffer clear];
//            
//            
//        }
//        else {
//
//            
//        }
//        
//        F1t dt = (CFAbsoluteTimeGetCurrent() - lastTime);
//        lastTime = CFAbsoluteTimeGetCurrent();
//        
//        [_scene updateWithTimeSinceLast:dt];
//        [_scene draw];
//        
//        if (useFB) {
//            
//            glBindFramebuffer(GL_FRAMEBUFFER, 0);
//            
//            glViewport(0, 0, self.visibleRect.size.width, self.visibleRect.size.height);
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//            
//            [rect setScene:_scene];
//            [rect customDraw];
//        }
//    
    else {
        // NO SCENE / DO RED SCREEN
        glViewport(0, 0, self.visibleRect.size.width, self.visibleRect.size.height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    }
    
    glFlush();
    
    
}

+(void) load_
{
    NSLog(@"%@ loaded", self);
}

-(instancetype)initWithCoder:(NSCoder *)aDecoder {
    
    self = [super initWithCoder:aDecoder];
    
    if (self) {
        
        
        //NSLog(@"NKView initWith Coder");
        //self = [self initWithFrame:self.frame shareContext:nil];
        [self becomeFirstResponder];
    }
    
    return self;
    
}

- (void) update
{
    // XXX: Should I do something here ?
    [super update];
}

- (void) awakeFromNib
{
    NSLog(@"awake from nib");
    
    _nkView = new NKView(V2Make(self.visibleRect.size.width, self.visibleRect.size.height));
    
    NSLog(@"pixel flags: %d %d %d %d %d %d",				NSOpenGLPFADoubleBuffer,
          NSOpenGLPFADepthSize, 24,NSOpenGLPFAOpenGLProfile,
          NSOpenGLProfileVersion3_2Core, 0 );
    
    
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        // Must specify the 3.2 Core Profile to use OpenGL 3.2
#if NK_USE_GL3
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
#endif
        //NSOpenGLPFASampleBuffers,4,
        //NSOpenGLPFASamples, 4,
        0
    };
    
    NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    
    if (!pf)
    {
        NSLog(@"No OpenGL pixel format");
    }
    
    
    
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    
    
#ifdef NK_GL_DEBUG
    // When we're using a CoreProfile context, crash if we call a legacy OpenGL function
    // This will make it much more obvious where and when such a function call is made so
    // that we can remove such calls.
    // Without this we'd simply get GL_INVALID_OPERATION error for calling legacy functions
    // but it would be more difficult to see where that function was called.
    //CGLEnable([context CGLContextObj], kCGLCECrashOnRemovedFunctions);
#endif
    
    [self setPixelFormat:pf];
    [self setOpenGLContext:context];
    [context setView:self];
    
    [[NKGLManager sharedInstance]setPixelFormat:pf];
    [[NKGLManager sharedInstance]setContext:context];
    
    
#if SUPPORT_RETINA_RESOLUTION
    // Opt-In to Retina resolution
    [self setWantsBestResolutionOpenGLSurface:YES];
#endif // SUPPORT_RETINA_RESOLUTION
    
    
    [self prepareOpenGL];
    
    GLint maj;
    GLint min;
    
    CGLGetVersion(&maj, &min);
    NSLog(@"NK GLView awake %1.0f %1.0f", self.visibleRect.size.width, self.visibleRect.size.height);
    NSLog(@"NK GLView using GL Version: %d.%d", maj, min);
    
    GetGLError();
    
    [self startAnimation];
}

- (void) prepareOpenGL
{
    [super prepareOpenGL];
    
    // Make all the OpenGL calls to setup rendering
    //  and build the necessary rendering objects
    [self initGL];
    
    
    //displayThread = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    
}

-(void)startAnimation {
    
    
    if (animating) return;
    animating = true;

    _mscale = 1.;
    
#if USE_CV_DISPLAY_LINK
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, (__bridge void *)(self));
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink);
#else
    displayTimer = [NSTimer timerWithTimeInterval:.015 target:self selector:@selector(drawView) userInfo:nil repeats:YES];
    
    [[NSRunLoop mainRunLoop] addTimer:displayTimer forMode:NSDefaultRunLoopMode];
    
    NSLog(@"start animation");
#endif
    
    // Register to be notified when the window closes so we can stop the displaylink
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowWillClose:)
                                                 name:NSWindowWillCloseNotification
                                               object:[self window]];
}

-(void)stopAnimation {
    if (!animating) return;
    animating = false;
    NSLog(@"stop animation");
    [displayTimer invalidate];
#if USE_CV_DISPLAY_LINK
#else
#endif
    
}

- (void) initGL
{
    // The reshape function may have changed the thread to which our OpenGL
    // context is attached before prepareOpenGL and initGL are called.  So call
    // makeCurrentContext to ensure that our OpenGL context current to this
    // thread (i.e. makeCurrentContext directs all OpenGL calls on this thread
    // to [self openGLContext])
    
    
    [[self openGLContext] makeCurrentContext];
    
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
}

- (void) reshape
{
    [super reshape];
    
#if USE_CV_DISPLAY_LINK
    // We draw on a secondary thread through the display link. However, when
    // resizing the view, -drawRect is called on the main thread.
    // Add a mutex around to avoid the threads accessing the context
    // simultaneously when resizing.
    CGLLockContext([[self openGLContext] CGLContextObj]);
#endif
    
    // Get the view size in Points
    NSRect viewRectPoints = [self bounds];
    
    _nkView->setSize(V2Make(self.bounds.size.width, self.bounds.size.height));
    
#if SUPPORT_RETINA_RESOLUTION
    
    // Rendering at retina resolutions will reduce aliasing, but at the potential
    // cost of framerate and battery life due to the GPU needing to render more
    // pixels.
    
    // Any calculations the renderer does which use pixel dimentions, must be
    // in "retina" space.  [NSView convertRectToBacking] converts point sizes
    // to pixel sizes.  Thus the renderer gets the size in pixels, not points,
    // so that it can set it's viewport and perform and other pixel based
    // calculations appropriately.
    // viewRectPixels will be larger (2x) than viewRectPoints for retina displays.
    // viewRectPixels will be the same as viewRectPoints for non-retina displays
    NSRect viewRectPixels = [self convertRectToBacking:viewRectPoints];
    
#else //if !SUPPORT_RETINA_RESOLUTION
    
    // App will typically render faster and use less power rendering at
    // non-retina resolutions since the GPU needs to render less pixels.  There
    // is the cost of more aliasing, but it will be no-worse than on a Mac
    // without a retina display.
    
    // Points:Pixels is always 1:1 when not supporting retina resolutions
    NSRect viewRectPixels = viewRectPoints;
    
#endif // !SUPPORT_RETINA_RESOLUTION
    
    // Set the new dimensions in our renderer
    //	[m_renderer resizeWithWidth:viewRectPixels.size.width
    //                      AndHeight:viewRectPixels.size.height];
    
    //[_scene setSize:S2Make(viewRectPixels.size.width*2., viewRectPixels.size.height*2.)];
    
#if USE_CV_DISPLAY_LINK
    
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
    
#endif
    
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    [self drawView];
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(__bridge NKNSView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (void) drawRect: (NSRect) theRect
{
    // Called during resize operations
    
    // Avoid flickering during resize by drawiing
    [self drawView];
}

- (void) drawView
{
    //dispatch_async(dispatch_get_main_queue(), ^{
    [[self openGLContext] makeCurrentContext];
    
    // We draw on a secondary thread through the display link
    // When resizing the view, -reshape is called automatically on the main
    // thread. Add a mutex around to avoid the threads accessing the context
    // simultaneously when resizing
#if USE_CV_DISPLAY_LINK
    CGLLockContext([[self openGLContext] CGLContextObj]);
#endif
    
    _nkView->draw();
    
    CGLFlushDrawable([[self openGLContext] CGLContextObj]);
    
#if USE_CV_DISPLAY_LINK
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
#endif
    //});
}


-(void) dealloc
{
#if USE_CV_DISPLAY_LINK
    if( displayLink ) {
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
    }
#else
    if (displayTimer){
        [displayTimer invalidate];
    }
#endif
}

-(void)keyDown:(NSEvent *)theEvent {
    //[_nkView keyDown:theEvent.keyCode];
}

-(void)keyUp:(NSEvent *)theEvent {
    //[_scene keyUp:theEvent.keyCode];
}

- (void)mouseDown:(NSEvent *)theEvent
{
    auto event = NKAppleEvent(theEvent);

    event.systemEvent = (__bridge void *)(theEvent);
    event.phase = NKEventPhaseBegin;
    event.startingScreenLocation = V2Make(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
    
    _nkView->events.push_back(event);
    
    _nkView->handleEvent(event);
    
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    
    for (auto& e : _nkView->events) {
        if (e.systemEvent == (__bridge void *)(theEvent)) {
            e.phase = NKEventPhaseMove;
            e.screenLocation = V2Make(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
            
            _nkView->handleEvent(e);
        }
    }
}

- (void)mouseUp:(NSEvent *)theEvent
{
    
    for (int i = 0; i < _nkView->events.size(); i++) {
        
        auto& e = _nkView->events[i];
        
        if (e.systemEvent == (__bridge void *)(theEvent)) {
            e.phase = NKEventPhaseEnd;
            e.screenLocation = V2Make(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
            if (e.node) e.node->handleEvent(e);
            _nkView->events.erase(_nkView->events.begin() + i);
            return;
        }
    }
    
    
}




@end

#endif

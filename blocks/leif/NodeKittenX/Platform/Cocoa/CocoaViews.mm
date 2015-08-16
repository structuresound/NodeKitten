//
//  CocoaViews.m
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/23/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#include "CocoaViews.h"
#include "FrameBuffer.h"
#include "NKGLManager.h"
#include "SceneController.h"
#include "Node.h"

bool isIpad(){
#if TARGET_OS_IPHONE
    if ( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad )
    {
        return true; /* Device is iPad */
    }
#endif
    return false;
}

bool isRetinaDisplay()
{
    // since we call this alot, cache it
    static CGFloat scale = 0.0;
    if (scale == 0.0)
    {
        scale = Platform::Screen::scale();
        return scale > 1.0;
        
    }
    return scale > 1.0;
}

int dpi()
{
#if TARGET_OS_IPHONE
    if (isIpad())
    {
        if (isRetinaDisplay()) return 264;
        return 130;
    }
    else
    {
        if (isRetinaDisplay()) return 320;
        return 160;
    }
#endif
    if (isRetinaDisplay()) {
        return 144;
    }
    else return 72;
}

#if TARGET_IOS

#pragma mark -
#pragma mark - IOS
#pragma mark -

unique_ptr<FrameBuffer>& genEAGLFrameBuffer(EAGLContext *context, id <EAGLDrawable> layer)

{
    //printf("GLES fb init with context %@\n", context);
    
    // 1 // Create the framebuffer and bind it.
    
    auto fbo = make_unique<FrameBuffer>();
    
    glGenFramebuffers(1, &fbo->glName);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo->glName);
    
    nkLog("allocate gl buffer, %d\n", fbo->glName);
    // 2 // Create a color renderbuffer, allocate storage for it, and attach it to the framebuffer’s color attachment point.
    
    glGenRenderbuffers(1, &fbo->renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo->renderBuffer);
    
    nkLog("allocate gl buffer, %d\n", fbo->renderBuffer);
    
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &fbo->width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &fbo->height);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fbo->renderBuffer);
    
    // 3 // Create a depth or depth/stencil renderbuffer, allocate storage for it, and attach it to the framebuffer’s depth attachment point.
    
    glGenRenderbuffers(1, &fbo->depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo->depthBuffer);
    
    nkLog("allocate gl depth buffer, %d\n", fbo->depthBuffer);
    
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fbo->width, fbo->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depthBuffer);
    
    // 4 // Test the framebuffer for completeness. This test only needs to be performed when the framebuffer’s configuration changes.
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        // NKCheckGLError(@"building frameBuffer\n");
        nkLog("failed to make complete framebuffer object %x\n", status);
    }
    
    return fbo;
}

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

F1t Platform::Screen::scale() {
    return [[UIScreen mainScreen] scale];
}

-(void)sharedInit {
    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    _mscale = 1.0f;
    
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)]) {
        _mscale = Platform::Screen::scale();
        nkLog("set pixel density scale : %1.2f", _mscale);
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
    
    _sceneController = new SceneController(V2(self.frame.size.width * _mscale, self.frame.size.height * _mscale));
    
    if (![self createFramebuffer]) {
        return;
    }
    else {
        NSLog(@"GLES Context && Frame Buffer loaded!");
    }
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(stopAnimation)
                                                 name:  UIApplicationWillTerminateNotification
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
    
    self.multipleTouchEnabled = true;
    
    UITapGestureRecognizer *dt = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(doubleTap:)];
    
    [dt setNumberOfTapsRequired:2];
    
    [self addGestureRecognizer:dt];
    
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
    if (_sceneController){
        [EAGLContext setCurrentContext:context];
        
        if (_sceneController) {
            _sceneController->draw();
        }
        
        glBindRenderbuffer(GL_RENDERBUFFER, _sceneController->framebuffer->glName);
        [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
}

-(void)destroyFramebuffer {
    _sceneController->framebuffer.reset();
}

-(BOOL) createFramebuffer {
    [EAGLContext setCurrentContext:context];
    
    _sceneController->framebuffer = genEAGLFrameBuffer(context, (id<EAGLDrawable>)self.layer);
    
    if (_sceneController->framebuffer) {
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
    P2t size = P2Make(_sceneController->size().width, _sceneController->size().height);
    return V2(p.x*_mscale, size.height - (p.y*_mscale));
}

-(void)touchesBegan:(NSSet *)touches withUXEvent:(UIUXEvent *)theEvent {
    for (UITouch *t in touches) {
        auto event = make_shared<CocoaUXEvent>(t);
        
        event->phase = UXEventPhaseBegin;
        
        event->startingScreenLocation = event->previousScreenLocation = event->screenLocation = [self uiPointToNodePoint:[t locationInView:self]];
        event->initialTimeStamp = event->previousTimeStamp = event->timeStamp = theEvent.timestamp;
        
        event->scale = V2([[t valueForKey:@"pathMajorRadius"] floatValue] / (dpi() / 10.0));
        
        //
        
        _sceneController->events.push_back(event);
        _sceneController->handleUXEvent(event);
    }
}

-(void)touchesMoved:(NSSet *)touches withUXEvent:(UIUXEvent *)theEvent {
    for (UITouch *t in touches) {
        for (auto e : _sceneController->events) {
            if (e->id() == (__bridge void *)(t)) {
                e->phase = UXEventPhaseDrag;
                e->type = UXEventTypePan;
                
                e->previousScreenLocation = e->screenLocation;
                e->screenLocation = [self uiPointToNodePoint:[t locationInView:self]];
                
                e->scale = e->scale * .9 + V2([[t valueForKey:@"pathMajorRadius"] floatValue] / (dpi() / 10.0)) * .1;
                e->previousTimeStamp = e->timeStamp;
                e->timeStamp = theEvent.timestamp;
                
                //nkLog("finger scale %1.2f", e->scale.x);
                
                _sceneController->handleUXEvent(e);
            }
        }
    }
}

-(void)touchesEnded:(NSSet *)touches withUXEvent:(UIUXEvent *)theEvent {
    
    for (UITouch *t in touches) {
        auto copy = _sceneController->events;
        
        for (auto e : copy) {
            if (e->id() == (__bridge void *)(t)) {
                e->phase = UXEventPhaseEnd;
                
                auto loc = [t locationInView:self];
                
                e->previousScreenLocation = e->screenLocation;
                e->screenLocation = [self uiPointToNodePoint:[t locationInView:self]];
                
                e->timeStamp = theEvent.timestamp;
                
                if (e->node) e->node->handleUXEvent(e);
                
                _::erase(_sceneController->events, e);
                
                return;
            }
        }
    }
}

-(void)touchesCancelled:(NSSet *)touches withUXEvent:(UIUXEvent *)theEvent {
    for (UITouch *t in touches) {
        auto copy = _sceneController->events;
        
        for (auto e : copy) {
            if (e->id() == (__bridge void *)(t)) {
                e->phase = UXEventPhaseCancel;
                if (e->node) {
                    e->node->handleUXEvent(e);
                }
                _::erase(_sceneController->events, e);
            }
        }
    }
}

-(void)doubleTap:(UITapGestureRecognizer*)recognizer {
    auto event = make_shared<CocoaUXEvent>((UITouch*)recognizer);
    
    event->type = UXEventTypeTap;
    event->phase = UXEventPhaseBegin;
    
    event->startingScreenLocation = event->previousScreenLocation = event->screenLocation = [self uiPointToNodePoint:[recognizer locationInView:self]];
    event->initialTimeStamp = event->previousTimeStamp = event->timeStamp = CFAbsoluteTimeGetCurrent();
    
    _sceneController->events.push_back(event);
    _sceneController->handleUXEvent(event);
    
    _::erase(_sceneController->events, event);
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
    
    if (_sceneController){
        
        _sceneController->draw();
        
    }
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

-(instancetype)initWithFrame:(NSRect)frameRect {
    if (self = [super initWithFrame:frameRect]) {
        [self awakeFromNib];
        [self becomeFirstResponder];
    }
    return self;
}

-(instancetype)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    
    if (self) {
        [self becomeFirstResponder];
    }
    
    return self;
}

- (void) update
{
    // XXX: Should I do something here ?
    [super update];
}

- (NSOpenGLPixelFormat*)pixelFormat {
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
    return pf;
}

F1t Platform::Screen::scale() {
    return [[NSScreen mainScreen] backingScaleFactor];
}

- (void) awakeFromNib
{
    NSLog(@"awake from nib");
    
    NSOpenGLPixelFormat *pf = [self pixelFormat];
    
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    
    // When we're using a CoreProfile context, crash if we call a legacy OpenGL function
    // This will make it much more obvious where and when such a function call is made so
    // that we can remove such calls.
    // Without this we'd simply get GL_INVALID_OPERATION error for calling legacy functions
    // but it would be more difficult to see where that function was called.
    CGLEnable([context CGLContextObj], kCGLCECrashOnRemovedFunctions);
    
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
    
    
    nkGetGLError();
    
    if ([[NSScreen mainScreen] backingScaleFactor]) {
        _mscale = [[NSScreen mainScreen] backingScaleFactor];
        nkLog("set retina scale, %1.2f", _mscale);
    }
    else {
        _mscale = 1.0;
    }
    
    NSLog(@"this should be first init of view");
    if (!_sceneController){
        _sceneController = new SceneController(V2(self.visibleRect.size.width * _mscale, self.visibleRect.size.height * _mscale));
    }
    
//    NSTapGestureRecognizer *dt = [[NSTapGestureRecognizer alloc]initWithTarget:self action:@selector(doubleTap:)];
//    [dt setNumberOfTapsRequired:2];
//    [self addGestureRecognizer:dt];
    
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
    
    if (_sceneController) {
        _sceneController->size.set(V2(self.bounds.size.width * _mscale, self.bounds.size.height * _mscale));
    }
    
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
    if (_sceneController){
        _sceneController->draw();
    }
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
    if (_sceneController){
        delete _sceneController;
    }
}

-(void)keyDown:(NSEvent *)theEvent {
    //[_sceneController keyDown:theEvent.keyCode];
}

-(void)keyUp:(NSEvent *)theEvent {
    //[_scene keyUp:theEvent.keyCode];
}

- (void)mouseDown:(NSEvent *)theEvent
{
    auto event = make_shared<CocoaUXEvent>(theEvent);
    
    event->startingScreenLocation = event->previousScreenLocation = event->screenLocation = V2(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
    event->initialTimeStamp = event->previousTimeStamp = event->timeStamp = theEvent.timestamp;
    
    _sceneController->events.push_back(event);
    _sceneController->handleUXEvent(event);
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    
    CocoaUXEvent event(theEvent);
    
    for (auto e : _sceneController->events) {
        if (e->id() == event.id()) {
            e->phase = UXEventPhaseDrag;
            
            e->previousScreenLocation = e->screenLocation;
            e->screenLocation = V2(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
            
            e->previousTimeStamp = e->timeStamp;
            e->timeStamp = theEvent.timestamp;
            _sceneController->handleUXEvent(e);
        }
    }
}

- (void)mouseUp:(NSEvent *)theEvent
{
    CocoaUXEvent event(theEvent);
    
    auto copy = _sceneController->events;
    for (auto e : copy) {
        if (e->id() == event.id()) {
            e->phase = UXEventPhaseEnd;
            
            e->previousScreenLocation = e->screenLocation;
            e->screenLocation = V2(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
            
            e->previousTimeStamp = e->timeStamp;
            e->timeStamp = theEvent.timestamp;
            
            if (e->node) e->node->handleUXEvent(e);
            _::erase(_sceneController->events, e);
            return;
        }
    }
}

@end

#endif

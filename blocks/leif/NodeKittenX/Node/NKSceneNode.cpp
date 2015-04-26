//
//  NKSceneNode.m
//  example-ofxTableView
//
//  Created by Chroma Developer on 2/17/14.
//
//

#include "NKSceneNode.h"
#include "NKFrameBuffer.h"
#include "NKView.h"
#include "NKCamera.h"
#include "NKTime.h"
#include "NKTexture.h"
#include "NKVertexBuffer.h"
#include "NKShaderProgram.h"
#include "NKLightNode.h"

NKSceneNode::NKSceneNode(S2t size){
    
#if NK_USE_MIDI
    [[NKMidiManager sharedInstance] setDelegate:self];
    _midiReceivedBlocks = [[NSMutableDictionary alloc]init];
#endif
    
#if NK_GL_DEBUG
    // Obtain iOS version
    int OSVersion_ = 0;
    
#if (TARGET_IOS || TARGET_OSX)
#if TARGET_IOS
    NSString *OSVer = [[UIDevice currentDevice] systemVersion];
#elif TARGET_OSX
    SInt32 versionMajor, versionMinor, versionBugFix;
    Gestalt(gestaltSystemVersionMajor, &versionMajor);
    Gestalt(gestaltSystemVersionMinor, &versionMinor);
    Gestalt(gestaltSystemVersionBugFix, &versionBugFix);
    
    NSString *OSVer = [NSString stringWithFormat:@"%d.%d.%d", versionMajor, versionMinor, versionBugFix];
#endif
    NSArray *arr = [OSVer componentsSeparatedByString:@"."];
    int idx = 0x01000000;
    for( NSString *str in arr ) {
        int value = [str intValue];
        OSVersion_ += value * idx;
        idx = idx >> 8;
    }
    NSLog(@"OS version: %@ (0x%08x)", OSVer, OSVersion_);
    //char* glExtensions = (char*)glGetString(GL_EXTENSIONS);
    //NSLog(@"GL EXT: %@",[NSString stringWithCString:glExtensions encoding: NSASCIIStringEncoding]);
#endif
    nkLog("GL_VENDOR:   %s", glGetString(GL_VENDOR) );
    nkLog("GL_RENDERER: %s", glGetString ( GL_RENDERER   ) );
    nkLog("GL_VERSION:  %s", glGetString ( GL_VERSION    ) );
#endif
    
    name = make_shared<string>("SCENE");
    
    backgroundColor = {.2, .2, .2, 1.0};
    shouldRasterize = false;
    _userInteractionEnabled = true;
    
    _blendMode = NKBlendModeNone;
    _cullFace = NKCullFaceNone;
    _usesDepth = false;
    
    setScene(this);
    camera = make_shared<NKCamera>(this);
    
    useColorDetection = true;
    
    hitDetectBuffer = make_shared<NKFrameBuffer>(size.width, size.height);
    
    hitDetectShader = NKShaderProgram::shaderNamed("hitShaderSingle",NKS_COLOR_MODE_UNIFORM,0,0,0);
    
    setSize(size);
    
#if NK_LOG_METRICS
    metricsTimer = [NSTimer timerWithTimeInterval:1. target:self selector:@selector(logMetricsPerSecond) userInfo:nil repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:metricsTimer forMode:NSDefaultRunLoopMode];
#endif
    printf("init scene with size, %f %f \n", size.width, size.height);
    
    GetGLError();
    
}

void NKSceneNode::setSize(S2t size){
    NKNode::setSize(size);
#if !NK_USE_WEBGL
    framebuffer = make_shared<NKFrameBuffer>(size.width, size.height);
#endif
}

void NKSceneNode::logMetricsPerSecond() {
    printf("fps %d : frametime: %1.3f nodes: %lu : textures: %lu : bBodies %lu : lights %lu", frames, (frameTime / frames), allChildren().size(), (unsigned long)666, (unsigned long)666, (unsigned long)lights.size());
    frames = 0;
    frameTime = 0;
}

void NKSceneNode::updateWithTimeSinceLast(F1t dt){
    
    if (loaded) {
        
#if NK_LOG_METRICS
        frames++;
        currentFrameTime = Time::getCurrentTime();
#endif
        
        
#warning  UPDATE SOUND GL ETC
        //    [NKSoundManager updateWithTimeSinceLast:dt];
        //    [NKGLManager updateWithTimeSinceLast:dt];
        //    [[NKBulletWorld sharedInstance] updateWithTimeSinceLast:dt];
        
        setDirty(true);
        
        NKNode::updateWithTimeSinceLast(dt);
        //nkLog("draw scene");
        
        camera->setDirty(true);
        camera->updateCameraWithTimeSinceLast(dt);
        
    }
    
}

void NKSceneNode::setDirty(bool dirty){
    
    NKNode::setDirty(dirty);
    
    if (dirty) {
        for (auto n : lights) {
            //printf("set dirty light \n");
            n->setDirty(dirty);
        }
    }
}

void NKSceneNode::drawHitBuffer() {
    
    nkGetGLError();
    
    _blendMode = NKBlendModeNone;
    glDisable(GL_BLEND);
    
    nkGetGLError();
    
    _activeShader = hitDetectShader.get();
    _activeShader->use();
    
    nkGetGLError();
    
    NKNode::drawWithHitShader();
    camera->drawWithHitShader();
    
    nkGetGLError();
}

void NKSceneNode::processHitBuffer() {
    
    hitDetectBuffer->bind();
    
    glViewport(0, 0, _size.width, _size.height);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    drawHitBuffer();
    
    while (hitQueue.size()) {
        hitQueue.front()();
        hitQueue.pop();
    }
}

void NKSceneNode::setActiveShader(NKShaderProgram *activeShader){
    
    nkGetGLError();
    
    if (!activeShader) {
        _activeShader = nullptr;
        glUseProgram(0);
    }
    
    else if (_activeShader != activeShader) {
        //NSLog(@"set shader: %@", activeShader.name);
        _activeShader = activeShader;
        
        _activeShader->use();
        nkGetGLError();
        
        // prep globals
        
        if  (_activeShader->hasUniformNamed(NKS_S2D_TEXTURE)){
            _activeShader->uniformNamed(NKS_S2D_TEXTURE).bindI1(0);
            glUniform1i(_activeShader->uniformNamed(NKS_S2D_TEXTURE).glLocation, 0);
            nkGetGLError();
        }

        if (activeShader->hasUniformNamed(NKS_LIGHT)){
            _activeShader->uniformNamed(NKS_I1_NUM_LIGHTS).bindI1((int)lights.size());
            nkGetGLError();
            if (lights.size() != 0) {
                for (int i = 0; i < lights.size(); i++) {
                    _activeShader->uniformNamed(NKS_LIGHT).bindLightProperties(lights[i]->properties, i);
                    nkGetGLError();
                }
            }
        }
        
        for (auto& m : _activeShader->modules) {
            m.updateUniforms();
            nkGetGLError();
        }
    }
    nkGetGLError();
}

void NKSceneNode::bindMainFrameBuffer(NKNode *sender){
    
    if (sender != this && framebuffer != nullptr) {
        framebuffer->bind();
    }
    else {
#if TARGET_IOS
        view->framebuffer->bind();
#elif (TARGET_OSX || NK_USE_WEBGL)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
    }
    //NSLog(@"bind fb %@ %@", view, view.framebuffer);
}

void NKSceneNode::draw() {
    
    clear();
    
    if (backgroundColor.data.a > 0) {
        glClearColor(backgroundColor.data.r, backgroundColor.data.g, backgroundColor.data.b, backgroundColor.data.a);
    }
#if DRAW_HIT_BUFFER
    drawHitBuffer();
#else
    NKNode::draw();
    camera->draw();
#endif
    
//#if (TARGET_IOS || TARGET_OSX)
    frameTime += NKTime::getCurrentTime() - currentFrameTime;
//#endif

    
    
}

//void NKSceneNode::drawStereo() {
//    glClearColor(0, 0, 0, 0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//    
//    for(int eye = 0; eye < 2; eye++){  // 0 = left , 1 = right
//        
//        camera->fovVertRadians = 120;
//        camera->aspect = _size.width / _size.height;
//      
//        
//        if(eye == 0){
//            glRotatef(-oculusRift.IPD*100.0, 0.0f, 1.0f, 0.0f);
//            //            glTranslatef(oculusRift.IPD, 0.0f, 0.0f);
//        }
//        else if (eye == 1){
//            glRotatef(oculusRift.IPD*100.0, 0.0f, 1.0f, 0.0f);
//            //            glTranslatef(-oculusRift.IPD, 0.0f, 0.0f);
//        }
//        glPushMatrix();
//        
//        // apply headset orientation
//        glMultMatrixf(oculusRift.orientation);
//        // mouse rotation
//        glRotatef(mouseRotation.y, 1, 0, 0);
//        glRotatef(mouseRotation.x, 0, 1, 0);
//        // draw scene
//        //-------------------------
//        // keyboard translation
//        scene.render(walkPosition.x, walkPosition.y);  // place for custom scene
//        //-------------------------
//        glPopMatrix();
//        // unbind framebuffer, now render to screen
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        
//        glBindTexture(GL_TEXTURE_2D, textureId);
//        glGenerateMipmap(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, 0);
//        
//        if(eye == 0)                        // left screen
//            glViewport (0, 0, w/2., h);
//        else if (eye == 1)                  // right screen
//            glViewport (w/2., 0, w/2., h);
//        
//        glMatrixMode (GL_PROJECTION);
//        glLoadIdentity ();
//        [self glPerspective:90.0f Aspect:(GLfloat)(w/2.0f)/(GLfloat)(h) Near:.1f Far:10.0f];
//        glMatrixMode (GL_MODELVIEW);
//        
//        glLoadIdentity();
//        glPushMatrix();
//        
//        if(warping){
//            glUseProgram(_program);
//            // preset suggestions from http://www.mtbs3d.com/phpbb/viewtopic.php?f=140&t=17081
//            const float Scale[2] = {0.1469278, 0.2350845};
//            const float ScaleIn[2] = {2, 2.5};
//            const float HmdWarpParam[4] = {1, 0.22, 0.24, 0};
//            const float LeftLensCenter[2] = {0.2863248*2.0, 0.5};
//            const float LeftScreenCenter[2] = {0.55, 0.5};
//            const float RightLensCenter[2] = {(0.7136753-.5) * 2.0, 0.5};
//            const float RightScreenCenter[2] = {0.45, 0.5};
//            // apply shader uniforms
//            glUniform2fv(uniforms[3], 1, Scale);
//            glUniform2fv(uniforms[4], 1, ScaleIn);
//            glUniform4fv(uniforms[5], 1, HmdWarpParam);
//            if(eye == 0){
//                glUniform2fv(uniforms[1], 1, LeftLensCenter);
//                glUniform2fv(uniforms[2], 1, LeftScreenCenter);
//            }
//            else{
//                glUniform2fv(uniforms[1], 1, RightLensCenter);
//                glUniform2fv(uniforms[2], 1, RightScreenCenter);
//            }
//        }
//        else{  // no warp, closer to fill screen
//            glTranslatef(0, 0, -1.0);
//        }
//        // draw scene on a quad for each side
//        glBindTexture(GL_TEXTURE_2D, textureId);
//        [self drawQuad];
//        glBindTexture(GL_TEXTURE_2D, 0);
//        
//        if(warping)
//            glUseProgram(0);
//        
//        glPopMatrix();
//    }
//    glFlush();
//}

void NKSceneNode::clear() {
    
    _activeShader = nullptr;
    glUseProgram(0);
    
    if (boundTexture){
        boundTexture->unbind();
        boundTexture = nullptr;
    }
    if (boundVertexBuffer){
        boundVertexBuffer->unbind();
        boundVertexBuffer = nullptr;
    }
}

void NKSceneNode::setLineSmooth(bool smooth) {
#if !NK_USE_GLES
    if (smooth) {
        glEnable(GL_LINE_SMOOTH);
    }
    else {
        glDisable(GL_LINE_SMOOTH);
    }
    nkGetGLError();
#endif
}

void NKSceneNode::setCullFace(NKCullFaceMode cullFace) {
    
    if (_cullFace != cullFace) {
        
        switch (cullFace) {
                
            case NKCullFaceNone:
                glDisable(GL_CULL_FACE);
                break;
                
            case NKCullFaceFront:
                if (_cullFace < 1) {
                    glEnable(GL_CULL_FACE);
                }
                glCullFace(GL_FRONT);
                break;
                
            case NKCullFaceBack:
                if (_cullFace < 1) {
                    glEnable(GL_CULL_FACE);
                }
                glCullFace(GL_BACK);
                break;
                
            case NKCullFaceBoth:
                if (_cullFace < 1) {
                    glEnable(GL_CULL_FACE);
                }
                glCullFace(GL_FRONT_AND_BACK);
                break;
                
            default:
                break;
        }
        
        nkGetGLError();
        _cullFace = cullFace;
    }
    
}

void NKSceneNode::setBlendMode(NKBlendMode blendMode) {
    
    if (_blendMode != blendMode) {
        
        switch (blendMode){
            case NKBlendModeNone:
                glDisable(GL_BLEND);
                break;
                
            case NKBlendModeAlpha:{
                glEnable(GL_BLEND);
                //#ifndef TARGET_OPENGLES
                //				glBlendEquation(GL_FUNC_ADD);
                //#endif
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            }
                
            case NKBlendModeAdd:{
                glEnable(GL_BLEND);
                //#ifndef TARGET_OPENGLES
                //				glBlendEquation(GL_FUNC_ADD);
                //#endif
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            }
                
            case NKBlendModeMultiply:{
                glEnable(GL_BLEND);
                //#ifndef TARGET_OPENGLES
                //				glBlendEquation(GL_FUNC_ADD);
                //#endif
                glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
                break;
            }
                
            case NKBlendModeScreen:{
                glEnable(GL_BLEND);
                //#ifndef TARGET_OPENGLES
                //				glBlendEquation(GL_FUNC_ADD);
                //#endif
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
                break;
            }
                
            case NKBlendModeSubtract:{
                glEnable(GL_BLEND);
                //#ifndef TARGET_OPENGLES
                //                glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
                //#else
                //                NSLog(@"OF_BLENDMODE_SUBTRACT not currently supported on OpenGL ES");
                //#endif
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            }
                
            default:
                break;
        }
        
        nkGetGLError();
        _blendMode = blendMode;
    }
    
}

void NKSceneNode::setUsesDepth(bool usesDepth) {
    if (_usesDepth != usesDepth) {
        if (usesDepth) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
        }
        else {
            glDisable(GL_DEPTH_TEST);
        }
        
        nkGetGLError();
        _usesDepth = usesDepth;
    }
}

//void NKSceneNode::setUniformIdentity() {
//    NKS_SCALAR s;
//    
//    s.I1[0] = 0;
//    _activeShader->uniformNamed(NKS_INT_NUM_TEXTURES).pushValue(s);
//    
//    s.M16 = M16IdentityMake();
//    _activeShader->uniformNamed(NKS_INT_NUM_TEXTURES).pushValue(s);
//    
//}

// SCENE DOES NOT TRANSFORM CHILDREN

void NKSceneNode::addChild(shared_ptr<NKNode> child) {
    _children.push_back(child);
    child->setScene(this);
}

M16t NKSceneNode::globalTransform() {
    return M16IdentityMake();
}

V3t NKSceneNode::globalPosition() {
    return V3Make(0, 0, 0);
}

void NKSceneNode::alertDidSelectOption(int option) {
    if (option == 0) {
        alertDidCancel();
    }
    // OVERRIDE IN SUBCLASS FOR OTHER OPTIONS
}

void NKSceneNode::alertDidCancel() {
    dismissAlertAnimated(true);
}

void NKSceneNode::presentAlert(NKAlertSprite* alert, bool animated) {
#warning ALERT SPRITE
    //    _alertSprite = alert;
    //    alert.delegate = self;
    //    [self addChild:alert];
    //
    //    if (animated) {
    //
    //        [_alertSprite setPosition2d:V2Make(0, -self.size.height)];
    //        [_alertSprite runAction:[NKAction move2dTo:V2Make(0, 0) duration:.3]];
    //    }
}


void NKSceneNode::dismissAlertAnimated(bool animated){
#warning ALERT SPRITE
    //    if (animated) {
    //        [_alertSprite runAction:[NKAction move2dTo:V2Make(0, -self.size.height) duration:.3] completion:^{
    //         [self removeChild:_alertSprite];
    //         _alertSprite = nil;
    //         }];
    //    }
    //    else {
    //        [self removeChild:_alertSprite];
    //        _alertSprite = nil;
    //    }
}

void NKSceneNode::pushMultiplyMatrix(M16t matrix) {
    stack.pushMultiply(matrix);
}

void NKSceneNode::pushScale(V3t scale) {
    stack.pushScale(scale);
}

void NKSceneNode::pushStyle(){
    
}

//void NKSceneNode::keyDown(U1t key) {
//
//    printf("key down %d \n", key);
// 
//    V3t p = _position;
//
//    switch (key) {
//
//        case 123:
//            setPosition(V3Make(p.x-1, p.y, p.z));
//            break;
//        case 124:
//
//            setPosition(V3Make(p.x+1, p.y, p.z));
//            break;
//
//        case 126: //up arrow
//
//            setPosition(V3Make(p.x, p.y, p.z+1)));
//            break;
//
//        case 125: // down arrow
//            setPosition(V3Make(p.x, p.y, p.z-1));
//            break;
//
//        default:
//            break;
//    }
//}

void NKSceneNode::dispatchEvent(NKEvent& event) {
    //NSLog(@"dispatch event for location %f %f",location.x, location.y);
    
#warning dispatchEvent
    //
    //    if (useColorDetection) {
    //
    //        HitCallback callBack = [this, event]{
    //            NKByteColor hc;
    //            glReadPixels(event.screenLocation.x, event.screenLocation.y,
    //                         1, 1,
    //                         GL_RGBA, GL_UNSIGNED_BYTE, &hc);
    //#warning do Shader Manager
    //           NKNode *hit = [NKShaderManager nodeForColor:hc];
    //
    //            if (!hit){
    //                hit = this;
    //            }
    //
    //            event.node = hit;
    //
    //            hit->handleEvent(event);
    //        };
    //
    //        hitQueue.push(callBack);
    //    }
    //
    //    else if (useBulletDetection) {
    //        // IMPLEMENT THIS
    //    }
}

void NKSceneNode::setView(NKView* view_){
    view = view_;
    loaded = true;
}

NKSceneNode::~NKSceneNode() {
    if (loaded) {
        unload();
    }
}

void NKSceneNode::unload() {
    
    glFinish();
    
    clear();
    
    loaded = false;
    
    while (hitQueue.size()) {
        hitQueue.pop();
    }
    
#if NK_LOG_METRICS
    [metricsTimer invalidate];
#endif
    
#if NK_USE_MIDI
    [_midiReceivedBlocks removeAllObjects];
#endif
    
    for (auto c : _children) {
        c->removeFromParent();
    }
    
#warning bullet
    //   [NKBulletWorld reset];
    
    lights.clear();
    
    hitDetectBuffer->unload();
}

#if NK_USE_MIDI
-(void)handleMidiCommand:(MIKMIDICommand *)command {
    for (MidiReceivedBlock block in _midiReceivedBlocks.allValues){
        block(command);
    }
    //else {
    // NSLog(@"received midi, provide scene a MidiReceivedBlock, or instead, implement: -(void)handleMidiCommand:(MIKMIDICommand *)command in subclass");
    //}
}
#endif


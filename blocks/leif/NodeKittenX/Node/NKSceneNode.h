/***********************************************************************
 
 Written by Leif Shackelford
 Copyright (c) 2014 Chroma.io
 All rights reserved. *
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met: *
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of CHROMA GAMES nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission. *
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE. *
 ***********************************************************************/

//#define SHOW_HIT_DETECTION


#ifndef __NKSceneNode_h_
#define __NKSceneNode_h_

#include "NKNode.h"
#include "NKCamera.h"

#if USE_OVR
#include "NKOculusInterface.h"
#endif

//#import "NKAlertSprite.h"

//#if NK_USE_MIDI
//#import "MIKMIDI.h"
//typedef void (^MidiReceivedBlock)(MIKMIDICommand* command);
//#define newMidiReceivedBlock (MidiReceivedBlock)^(MIKMIDICommand* command)
//#endif

typedef function<void(void)> HitCallback;

class NKView;
class NKCamera;
class NKShaderProgram;
class NKVertexBuffer;
class NKAlertSprite;
class NKFrameBuffer;
class NKTexture;
class NKLightNode;

//class NKBulletWorld;

class NKSceneNode : public NKNode

{
    int frames;
    double currentFrameTime;
    double frameTime;
    
    
    NKVertexBuffer *axes = nullptr;
    
#if NK_LOG_METRICS
    NSTimer *metricsTimer;
#endif
    
    NKShaderProgram* _activeShader = nullptr;
    
public:
    bool loaded = false;
    
#if USE_OVR
    NKOculusInterface oculus;
#endif
    
    NKSceneNode(S2t size);
    ~NKSceneNode();
    
    // NODE HIERARCHY
    virtual void moveToView(){};
    virtual void moveFromView(){unload();};
    
    NKView *view = nullptr;
    
    //@property (nonatomic) void *view;
    
    
    bool shouldRasterize = false;
    NKColor backgroundColor {0.3,0.3,0.3,1.0};
    NKColor borderColor;
    
    shared_ptr<NKCamera> camera;
    M16Stack stack;
    // NKAlertSprite *alertSprite;
    
    bool drawLights;
    vector<NKLightNode*> lights;
    
    // HIT DETECTION
    
    void dispatchEvent(NKEvent* event);
    
    
    // COLOR BASED HIT DETECTION
    bool useColorDetection;
    shared_ptr<NKShaderProgram> hitDetectShader;
    shared_ptr<NKFrameBuffer> hitDetectBuffer;
    
    queue<HitCallback> hitQueue;
    
    // PHYSICS HIT DETECTION
    
    bool useBulletDetection;
    
    // GL STATE MACHINE CONTROL
    
    void setActiveShader(NKShaderProgram* activeShader);
    NKShaderProgram* activeShader(){return _activeShader;};
    NKVertexBuffer *boundVertexBuffer = nullptr;
    NKTexture *boundTexture = nullptr;
    bool fill;
    
    // EXTERNAL MODULES
#warning midi
    //#if NK_USE_MIDI
    //    void handleMidiCommand:(MIKMIDICommand*)command;
    //    NSMutableDictionary* midiReceivedBlocks;//MidiReceivedBlock midiReceivedBlock;
    //#endif
    
    void setView(NKView* view);
    
    void bindMainFrameBuffer(NKNode* sender);
    void clear();
    void unload();
    
    void pushMultiplyMatrix(M16t matrix);
    void pushScale(V3t scale);
    void popMatrix();
    
    void setUniformIdentity();
    void drawAxes();
    
    // EVENTS
    
    void dispatchEvent(NKEvent& event);
    
    // UPDATE
    
    // NODE OVERRIDES
    
    void setSize(S2t size) override;
    void pushStyle() override;
    void updateWithTimeSinceLast(F1t dt) override;
    void setDirty(bool dirty) override;
    void addChild(shared_ptr<NKNode>child) override;
    
    // DRAW
    
    void draw() override;
    void drawStereo();
    
    void setLineSmooth(bool smooth) override;
    void setBlendMode(NKBlendMode blendMode) override;
    void setCullFace(NKCullFaceMode cullFace) override;
    void setUsesDepth(bool usesDepth) override;
    
    M16t globalTransform() override;
    V3t globalPosition() override;
    
    // HIT BUFFER
    
    void processHitBuffer();
    void drawHitBuffer();
    
    void logMetricsPerSecond();
    
    // ALERT
    
    void presentAlert(NKAlertSprite* alert, bool animated);
    void alertDidSelectOption(int option);
    void alertDidCancel();
    void dismissAlertAnimated(bool animated);
    
    
};

#endif



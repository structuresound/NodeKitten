/***********************************************************************
 * Written by Leif Shackelford
 * Copyright (c) 2014 Chroma Games
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Chroma Games nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/


#ifndef __NKAction_h_
#define __NKAction_h_

#include "NKTypes.h"

class NKNode;
class NKAction;

union _ActionData {
    struct {V3t startPos; F1t startFloat; V3t endPos; F1t endFloat;};
    struct {Q4t startOrientation; Q4t endOrientation;};
    struct {C4t startColor; C4t endColor;};
} __attribute__((aligned(16)));

typedef union _ActionData ActionData;

typedef std::function<void(NKAction*, NKNode*, F1t)> ActionBlock;

typedef enum NKActionTimingMode {
    NKActionTimingLinear,
    NKActionTimingEaseIn,
    NKActionTimingEaseOut,
    NKActionTimingEaseInEaseOut
} NKActionTimingMode;

class NKAction : public enable_shared_from_this<NKAction>
{
    U1t _numFrames {0};
    U1t _nFrames {0};
    
    F1t _duration {0};

    U1t _subDivide {0};
    U1t _currentSubdivision {0};
    I1t _repeats {0};
    I1t _initialRepeats {0};

    NKActionTimingMode _timingMode {NKActionTimingLinear};
    
public:
    
    NKAction(F1t duration);
    
    U1t frameCount {0};
    bool shouldInit {1};
    bool serial {false};
    
    // TIMING
    
    ActionBlock actionBlock {nullptr};
    CompletionBlock completionBlock {nullptr};
    
    void setDuration(F1t duration);
    void setSubdivide(U1t subdivide);
    
    // NODE / HIERATCHY
    string key;
    NKAction *parentAction {nullptr};
    NKNode *parentNode {nullptr};
    
    vector<shared_ptr<NKAction>> children; // THESE STAY FOR REPEATS
    vector<shared_ptr<NKAction>> actions; // THESE COPY FROM CHILDREN ON EACH REPEAT
    
    void removeAction(shared_ptr<NKAction> action);
    void sharedReset();
    bool completeOrRepeat();
    bool completeWithTimeSinceLast(F1t dt, NKNode *node);
    bool updateWithTimeSinceLast(F1t dt, NKNode *node);
    
    NKNode *target {nullptr};
    
    ActionData data;
    
    // TWEEN STORAGE
    V3t startPos;
    V3t endPos;
    Q4t startOrientation;
    Q4t endOrientation;
    
    F1t startFloat;
    F1t endFloat;
    
    // STATE
    U1t mode {0};
    bool flag {false};
    
    shared_ptr<NKAction> reversedAction;
    
    void runCompletion(CompletionBlock block);
    void stop();
    
    bool updateWithTimeSinceLast(F1t dt);
    
    
    // 3D Additions

    static shared_ptr<NKAction> rotateXByAngle(F1t radians ,F1t sec);
    static shared_ptr<NKAction> rotateYByAngle(F1t radians , F1t sec);
    
    static shared_ptr<NKAction> rotateByAngles(V3t angles , F1t sec);
    static shared_ptr<NKAction> rotateToAngles(V3t angles , F1t sec);
    
    static shared_ptr<NKAction> rotateAxis(V3t axis, F1t radians, F1t sec);
    
    static shared_ptr<NKAction> moveByX(F1t deltaX, F1t deltaY , F1t sec);
    static shared_ptr<NKAction> moveByX(F1t deltaX, F1t deltaY , F1t deltaZ, F1t sec);
    static shared_ptr<NKAction> moveBy(V3t delta, F1t sec);
    
    static shared_ptr<NKAction> moveToX(F1t x , F1t sec);
    static shared_ptr<NKAction> moveToY(F1t y , F1t sec);
    static shared_ptr<NKAction> moveToX(F1t x, F1t y , F1t sec);
    static shared_ptr<NKAction> moveTo(V2t location , F1t sec);
    static shared_ptr<NKAction> moveTo(V3t location , F1t sec);
    
    static shared_ptr<NKAction> moveToFollowNode(NKNode* target , F1t sec);
    static shared_ptr<NKAction> followNode(NKNode* target , F1t sec);
    
    static shared_ptr<NKAction> rotateZByAngle(F1t radians , F1t sec);
    
    static shared_ptr<NKAction> rotateToAngle(F1t radians , F1t sec);
    static shared_ptr<NKAction> rotateToAngle(F1t radians , F1t sec, bool shortestUnitArc);
    //
    //static shared_ptr<NKAction> resizeByWidth(F1t width height(F1t height , (F1t), ;
    static shared_ptr<NKAction> resizeToWidth(F1t width, F1t height, F1t sec);
    static shared_ptr<NKAction> resize(V3t newSize , F1t sec);
    //static shared_ptr<NKAction> resizeToWidth(F1t width , (F1t), ;
    //static shared_ptr<NKAction> resizeToHeight(F1t height , (F1t), ;
    //
    static shared_ptr<NKAction> scaleBy(F1t scale , F1t sec);
    static shared_ptr<NKAction> scaleXBy(F1t xScale , F1t yScale , F1t sec);
    static shared_ptr<NKAction> scaleTo(F1t scale , F1t sec);
    static shared_ptr<NKAction> scaleXTo(F1t xScale ,F1t yScale , F1t sec);
    static shared_ptr<NKAction> scaleXTo(F1t scale , F1t sec);
    static shared_ptr<NKAction> scaleYTo(F1t scale , F1t sec);
    
    static shared_ptr<NKAction> sequence(vector<shared_ptr<NKAction>> actions);
    static shared_ptr<NKAction> group(vector<shared_ptr<NKAction>> actions);
    
    static shared_ptr<NKAction> delay(F1t sec);
    
    shared_ptr<NKAction> repeat(U1t count);
    shared_ptr<NKAction> repeatForever();
    shared_ptr<NKAction> timingMode(NKActionTimingMode mode);
    
    I1t repeats() {return _initialRepeats;};
    
    static shared_ptr<NKAction> fadeByEnvelopeWithWaitTime(F1t waitTime, F1t inTime, F1t holdTime ,F1t outTime);
    
    static shared_ptr<NKAction> fadeInWithDuration(F1t sec);
    static shared_ptr<NKAction> fadeBlendTo(F1t alpha , F1t sec);
    static shared_ptr<NKAction> fadeColorTo(NKColor color , F1t sec);
    
    static shared_ptr<NKAction> fadeOutWithDuration(F1t sec);
    static shared_ptr<NKAction> fadeAlphaBy(F1t factor , F1t sec);
    static shared_ptr<NKAction> fadeAlphaTo(F1t alpha , F1t sec);
    static shared_ptr<NKAction> strobeAlpha(U1t onFrames, U1t offFrames , F1t sec);
    
    //
    //static shared_ptr<NKAction> setTexture(ofTexture *)texture;
    //static shared_ptr<NKAction> animateWithTextures(NSArray *)textures timePerFrameF1t sec);
    //static shared_ptr<NKAction> animateWithTextures(NSArray *)textures timePerFrameF1t sec) resize(BOOL)resize restore(BOOL)restore;
    //
    ///* name must be the name or path of a file of a platform supported audio file format. Use a LinearPCM format audio file with 8 or 16 bits per channel for best performance */
    //static shared_ptr<NKAction> playSoundFileNamed(string*)soundFile waitForCompletion(BOOL)wait;
    //
    //static shared_ptr<NKAction> colorizeWithColor(NKColor *)color colorBlendFactor(F1t colorBlendFactor , F1t sec);
    //static shared_ptr<NKAction> colorizeWithColorBlendFactor(F1t colorBlendFactor , F1t sec);
    //
    //static shared_ptr<NKAction> followPath(CGPathRef)path , F1t sec);
    //static shared_ptr<NKAction> followPath(CGPathRef)path asOffset(BOOL)offset orientToPath(BOOL)orient , F1t sec);
    //
    //static shared_ptr<NKAction> speedBy(F1t speed , F1t sec);
    //static shared_ptr<NKAction> speedTo(F1t speed , F1t sec);
    //
    //static shared_ptr<NKAction> waitForDurationF1t sec);
    //static shared_ptr<NKAction> waitForDurationF1t sec) withRange(F1t), Range;
    //
    //static shared_ptr<NKAction> removeFromParent;
    //
    //static shared_ptr<NKAction> performSelector(SEL)selector onTarget(id)target;
    //
    //static shared_ptr<NKAction> runBlock(dispatch_block_t)block;
    //static shared_ptr<NKAction> runBlock(dispatch_block_t)block queue(dispatch_queue_t)queue;
    //
    //static shared_ptr<NKAction> runAction(NKAction *)action onChildWithName(string*)name;
    //
    static shared_ptr<NKAction> customActionWithDuration(F1t sec, ActionBlock block);
    
    // SCROLL NODE
    
    static shared_ptr<NKAction> scrollToPoint(P2t point , F1t sec);
    static shared_ptr<NKAction> scrollToChild(NKNode* child , F1t sec);
    
    // LOOK
    
    static shared_ptr<NKAction> panTolookAtNode(NKNode* target, F1t sec);
    static shared_ptr<NKAction> snapLookToNode(NKNode* target, F1t sec);
    
    // ORBIT
    
    static shared_ptr<NKAction> enterOrbitAtLongitude(F1t longitude ,F1t latitude, F1t radius, F1t sec, V3t offset = V3MakeF(0));
    static shared_ptr<NKAction> enterOrbitForNode(NKNode* target, F1t longitude ,F1t latitude, F1t radius, F1t sec, V3t offset = V3MakeF(0));
    
    static shared_ptr<NKAction> maintainOrbitDelta(F1t deltaLongitude, F1t deltaLatitude, F1t deltaRadius, F1t sec, V3t offset = V3MakeF(0));
    static shared_ptr<NKAction> maintainOrbitForNode(NKNode * target, F1t deltaLongitude, F1t deltaLatitude, F1t deltaRadius, F1t sec, V3t offset = V3MakeF(0));
    
};

#endif

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

#ifndef __NKNode_h_
#define __NKNode_h_

class NKAction;

#include "NKEvent.h"

class NKSceneNode;
class NKDrawDepthShader;
class NKShaderProgram;
class NKBulletBody;
class NKFrameBuffer;

typedef enum NKBlendMode {
    NKBlendModeNone,
    NKBlendModeAlpha,
    NKBlendModeAdd,
    NKBlendModeMultiply,
    NKBlendModeScreen,
    NKBlendModeSubtract
} NKBlendMode;

typedef enum NKCullFaceMode {
    NKCullFaceNone,
    NKCullFaceFront,
    NKCullFaceBack,
    NKCullFaceBoth
} NKCullFaceMode;

class NKNode;
class NKEvent;

class NKNode : public enable_shared_from_this<NKNode>
{
    
#pragma mark - POSITION / GEOMETRY
    
private:
    
    NKNode *_parent = nullptr;
    NKSceneNode* _scene = nullptr;
    
protected:
    M16t _localTransform;
    M16t _cachedGlobalTransform;
    
    Q4t _orientation;// matrix set orientation
    V3t _size;
    V3t _scale; // matrix set scale
    V3t _position;// matrix set translation
    V3t _anchorPoint;
    
    vector<shared_ptr<NKNode>> _children;
    vector<shared_ptr<NKEvent>> _touches;
    
    NKColor _color {1,1,1,1};
    
    vector<shared_ptr<NKAction>> actions;
    
    // CACHED PROPS
    F1t _alpha {1.0};
    F1t parentAlpha {1.0};
    F1t intAlpha {1.0};
    F1t _colorBlendFactor {1.0};
    
    EventBlock _eventBlock {nullptr};
    
    bool _dirty {true};
    
    V3t _upVector;
    
    NKBlendMode _blendMode {NKBlendModeAlpha};
    NKCullFaceMode _cullFace {NKCullFaceNone};
    bool _usesDepth {true};
    bool _userInteractionEnabled {true};
    
#pragma mark - SHADER PROPERTIES
    
    shared_ptr<NKShaderProgram> _shader;
    shared_ptr<NKShaderProgram> _postProcess;
    
public:
    
    M16t modelViewCache;
    M16t modelViewProjectionCache;
    F1t cameraDistance;
    
#pragma mark - NODE TREE
    
    shared_ptr<string> name;
    shared_ptr<NKByteColor> uidColor;
    shared_ptr<NKFrameBuffer> framebuffer;
    
#pragma mark - STATE + INTERACTION PROPERTIES
    
    bool paused {false};
    bool hidden {false};
    
#pragma mark - COLOR + BLEND
    
    C4t glColor();
    F1t colorBlendFactor();
    void setColorBlendFactor(F1t colorBlendFactor);
    virtual void setColor(NKColor color);
    NKColor& color();
    
    void setTransparency(F1t transparency);
    void recursiveAlpha(F1t alpha);
    void setAlpha(F1t alpha);
    F1t alpha(){return _alpha;};
    
#pragma mark - GL States
    
    virtual void setLineSmooth(bool smooth);
    virtual void setBlendMode(NKBlendMode blendMode) {_blendMode = blendMode;};
    virtual void setCullFace(NKCullFaceMode cullFace) {_cullFace = cullFace;};
    virtual void setUsesDepth(bool usesDepth) {_usesDepth = usesDepth;};
    
#pragma mark - SHADER PROPERTIES
    
    void setShader(shared_ptr<NKShaderProgram>shader);
    void setPostProcess(shared_ptr<NKShaderProgram>postProcess);
    
#pragma mark - INIT
    
    static shared_ptr<NKNode> node();
    NKNode();
    NKNode(V3t size);
    ~NKNode();
    
#pragma mark - HIERARCHY METHODS
    vector<shared_ptr<NKNode>> children();
    
    virtual void addChild(shared_ptr<NKNode> node);
    void removeChild(shared_ptr<NKNode> node);
    void removeChildNamed(string& name);
    
    void fadeInChild(shared_ptr<NKNode> child, F1t seconds = 1.0, CompletionBlock completion = [](){});
    void fadeOutChild(shared_ptr<NKNode> child, F1t seconds = 1.0, CompletionBlock completion = [](){});
    
    //-(P2t)childLocationIncludingRotation(NKNode*)child;
    void insertChild(shared_ptr<NKNode> node, int atIndex = 0);
    void removeChildrenInArray(vector<shared_ptr<NKNode>> nodes);
    
    void removeAllChildren();
    virtual void removeFromParent();
    
    shared_ptr<NKNode> childNodeNamed(string& name);
    
    vector<shared_ptr<NKNode>>allChildren();
    
    //- (NKNode *)childNodeWithName(NSString *)name;
    //-(NSArray*)allChildren;
    //- (NKNode *)randomChild;
    //-(NKNode*)randomLeaf;
    //void enumerateChildNodesWithName(NSString *)name usingBlock(void (^)(NKNode *node, BOOL *stop))block;
    
    NKNode* parent();
    
    void setParent(NKNode * parent);
    bool inParentHierarchy(NKNode *parent);
    
    virtual void setDirty(bool dirty);
    
    NKSceneNode* scene();
    virtual void setScene(NKSceneNode *scene);
    
#pragma mark - PHYSICS
    
    //NKBulletBody *body;
    
#pragma mark - GEOMETRY METHODS
    
    bool containsPoint(P2t location);
    
    void setXPosition(float position);
    void setYPosition(float position);
    void setZPosition(float position);
    
    V3t position();
    
    void setPosition(V3t position);
    void setPosition(V2t position);
    
    virtual V3t globalPosition();
    void setGlobalPosition(V3t globalPosition);
    
    virtual M16t globalTransform();
    
    M16t transformMatrixInNode(NKNode* n);
    
    V2t positionInNode(NKNode* node);
    V3t positionInNode3d(NKNode* node);
    
    V2t convertPointToNode(P2t point, NKNode * fromNode);
    V2t convertPointFromNode(P2t point, NKNode * toNode);
    
    V3t convertPointToNode(V3t point, NKNode *fromNode);
    V3t convertPointFromNode(V3t point, NKNode * toNode);
    
#pragma mark - UPDATE / DRAW CYCLE
    
    //**
    virtual void updateWithTimeSinceLast(F1t dt);
    //**
    virtual void draw();
    virtual void drawChildren();
    
    //**
    // draw encompasses these 2 states
    virtual void setupViewMatrix();
    virtual void customDraw();
    //**
    
    // drawing for hit detection
    void drawWithHitShader();
    virtual void customDrawWithHitShader();
    
    void drawMultiPass(int numPasses);
    
    bool shouldCull();
    
    int numVisibleNodes();
    int numNodes();
    
#pragma mark - SHADER / FBO
    
    void loadShaderNamed(string& name);
    
#pragma mark - STATE MAINTENANCE
    
    virtual void pushStyle();
    
#pragma mark - MATRIX METHODS
    
    void setLocalTransform(M16t localTransform);
    M16t localTransform();
    void createMatrix();
    
#pragma mark - POSITION METHODS
    
    virtual void setSize(S2t size);
    void setSize(V3t size);
    
    V2t size2d();
    V3t size();
    
    R4t getDrawFrame();
    R4t getGlobalFrame();
    R4t calculateAccumulatedFrame();
    
    void setAnchorPoint(V3t anchorPoint);
    void setAnchorPoint(V2t anchorPoint);
    
    V3t anchorPoint();
    V2t anchorPoint2d();
    
#pragma mark - ORIENTATION METHODS
    
    void  rotateMatrix(M16t M16);
    void  globalRotateMatrix(M16t M16);
    void  setOrientation(const Q4t q);
    void  setGlobalOrientation(const Q4t q);
    void  setOrientationEuler(const V3t eulerAngles);
    
    Q4t getGlobalOrientation();
    Q4t orientation();
    V3t getOrientationEuler();
    F1t getYOrientation();
    
    void setZRotation(F1t rotation);
    
    // look etc.
    
#pragma mark - GEODATA
    
    F1t latitude {0};
    F1t longitude {0};
    F1t radius {0};
    
    void setOrbit(V3t orbit);
    V3t currentOrbit();
    V3t orbitForLongitude(float longitude, F1t latitude, F1t radius);
    V3t upVector();
    void lookAtPoint(V3t point);
    void lookAtNode(NKNode* node);
    M16t getLookMatrix(V3t lookAtPosition);
    
#pragma mark - SCALE METHODS
    
    void setScale(V3t scale);
    void setScaleF(F1t s);
    void setXScale(F1t s);
    void setYScale(F1t s);
    void setZScale(F1t s);
    
    V3t scale();
    V2t scale2d();
    
#pragma mark - TOUCH
    
    void setUserInteractionEnabled(bool enabled);
    bool userInteractionEnabled() {return _userInteractionEnabled;};
    
    void setEventBlock(EventBlock eventBlock);
    
    virtual void handleEvent(NKEvent& event);
    
    // UTIL
    void logCoords();
    void logPosition();
    void logMatrix(M16t M16);
    
#pragma mark - ACTIONS
    
    void runAction(shared_ptr<NKAction> action);
    void runAction(shared_ptr<NKAction> action, CompletionBlock completion);
    void runAction(shared_ptr<NKAction> actionm, string& key);
    
    void runCompletionBlockForAction(shared_ptr<NKAction> action);
    
    U1t hasActions();
    
    shared_ptr<NKAction> actionForKey(string& key);
    
    void removeAction(shared_ptr<NKAction> action);
    void removeActionForKey(string& key);
    void removeAllActions();
    
};


#endif

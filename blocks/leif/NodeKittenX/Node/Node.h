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

class Action;

#include "Generics.h"

class Scene;
class NKDrawDepthShader;
class Shader;
class NKBulletBody;
class FrameBuffer;

class Node;
class UXEvent;

//template <class T>
//class Node {
//    
//};

class Node : public Nodal<Node>, public SizeProperty<V3>, public enable_shared_from_this<Node>
{
    
#pragma mark - POSITION / GEOMETRY
    
private:
    
    Node *_parent = nullptr;
    
protected:
    
    Scene* _scene = nullptr;
    
    // NO NEED FOR DEFAULTS ON THESE, ASSIGNED IN CONSTRUCTOR OR BEFORE USE
    M16t _localTransform;
    M16t _cachedGlobalTransform;
    bool _gtDirty {true};
    
    Q4t _orientation;// matrix set orientation
    V3t _scale; // matrix set scale
    V3t _position;// matrix set translation
    V3t _anchorPoint;
    V3t _upVector;
    
    vector<shared_ptr<Node>> _children;
    vector<shared_ptr<UXEvent>> _touches;
    
    Color _color;
    
    vector<shared_ptr<Action>> actions;
    
    // CACHED PROPS
    F1t _alpha {1.0};
    F1t parentAlpha {1.0};
    F1t intAlpha {1.0};
    F1t _colorBlendFactor {1.0};
    
    std::function<void(shared_ptr<UXEvent>)> _eventBlock {nullptr};
    std::function<void(F1t)> _updateBlock {nullptr};
    
    bool _dirty {true};
    bool _shouldRasterize {false};
    
    GLBlendMode _blendMode {GLBlendModeAlpha};
    GLCullFaceMode _cullFace {GLCullFaceNone};
    bool _usesDepth {true};
    bool _forceOrthographic {false};
    bool _ignoresLights {false};
    bool _userInteractionEnabled {true};
    
#pragma mark - SHADER PROPERTIES
    
public:
    
#pragma mark - CONVENIENCE CONSTRUCTORS
    
    shared_ptr<Node> node();
    shared_ptr<Node> node(V3t size);
    shared_ptr<Node> node(Color color, V3t size);
    
    shared_ptr<Shader> shader;
    
    bool cullable {true};
    
    M16t modelViewCache;
    M16t modelViewProjectionCache;
    
    F1t cameraDistance;
    
#pragma mark - NODE TREE
    
    string name;
    UB4t uidColor;
    
#pragma mark - STATE + INTERACTION PROPERTIES
    
    bool paused {false};
    bool hidden {false};
    
#pragma mark - COLOR + BLEND
    
    V4t glColor();
    F1t colorBlendFactor();
    void setColorBlendFactor(F1t colorBlendFactor);
    virtual void setColor(Color color);
    Color& color();
    
    void setTransparency(F1t transparency);
    void recursiveAlpha(F1t alpha);
    void setAlpha(F1t alpha);
    F1t alpha(){return _alpha;};
    
    bool isOpaque();
    int siblingPosition;
    
#pragma mark - GL States
    
    void setOrthographic(bool orthographic = true){_forceOrthographic = orthographic; if (_forceOrthographic) cullable = false;};
    void setIgnoresLights(bool ignoresLights = true){_ignoresLights = ignoresLights;};
    
    virtual void setBlendMode(GLBlendMode blendMode) {_blendMode = blendMode;};
    virtual void setCullFace(GLCullFaceMode cullFace) {_cullFace = cullFace;};
    virtual void setUsesDepth(bool usesDepth) {_usesDepth = usesDepth;};
    
#pragma mark - SHADER PROPERTIES
    
    virtual void chooseShader();
    
#pragma mark - INIT
    
    Node(Color color = Color(0), V3t size = {1,1,1});
    Node(V3t size) : Node(Color(0), size){};
    Node(V2t size) : Node(Color(0), V3t {size.x, size.y, 1}){};
    ~Node();
    
#pragma mark - HIERARCHY METHODS
    
    
    vector<shared_ptr<Node>> children();
    shared_ptr<Node> child();
    
    virtual void addChild(shared_ptr<Node> node);
    void removeChild(shared_ptr<Node> node);
    void removeChildNamed(string& name);
    
    void fadeInChild(shared_ptr<Node> child, F1t seconds = 1.0, CompletionBlock completion = [](){});
    void fadeOutChild(shared_ptr<Node> child, F1t seconds = 1.0, CompletionBlock completion = [](){});
    
    //-(P2t)childLocationIncludingRotation(Node*)child;
    void insertChild(shared_ptr<Node> node, int atIndex = 0);
    void removeChildrenInArray(vector<shared_ptr<Node>> nodes);
    
    void removeAllChildren();
    virtual void removeFromParent();
    
    shared_ptr<Node> childNodeNamed(string& name);
    
    // EXPENSIVE
    vector<shared_ptr<Node>>allChildren();
    
    shared_ptr<Node> randomChild();
    shared_ptr<Node> randomLeaf();
    
    Node* parent();
    
    Scene* scene(){
        if (_scene) return _scene;
        return _scene = getScene();
    }
    
    Scene* getScene(){
        if (_scene) return _scene;
        if (_parent) return _parent->getScene();
        else return (Scene*)this;
    }
    
    Scene* setScene(Scene* scene){
        _scene = scene;
    }
    
    virtual void setParent(Node * parent);
    bool inParentHierarchy(Node *parent);
    
    virtual void setDirty(bool dirty = true);
    
#pragma mark - PHYSICS
    
    //NKBulletBody *body;
    
#pragma mark - GEOMETRY METHODS
    
    void setXPosition(float position);
    void setYPosition(float position);
    void setZPosition(float position);
    
    V3t position();
    
    void setPosition(V3t position);
    void setPosition(V2t position);
    void setPosition(F1t x, F1t y);
    void setPosition(F1t x, F1t y, F1t z);
    
    virtual V3t globalPosition();
    void setGlobalPosition(V3t globalPosition);
    
    virtual M16t globalTransform();
    
    M16t transformMatrixInNode(Node* n);
    
    V2t positionInNode(Node* node);
    V3t positionInNode3d(Node* node);
    
    V2t convertPointToNode(P2t point, Node * fromNode);
    V2t convertPointFromNode(P2t point, Node * toNode);
    
    V3t convertPointToNode(V3t point, Node *fromNode);
    V3t convertPointFromNode(V3t point, Node * toNode);
    
    V2t screenPosition();
    
#pragma mark - UPDATE / DRAW CYCLE
    
    // 1 //
    virtual void updateWithTimeSinceLast(F1t dt);
    
    virtual void draw(); // DON'T DIRECTLY OVERRIDE WITHOUT GOOD REASON - LOOK FOR SUBFUNCTIONS
    
    // **** DRAW - SUB FUNCTIONS **** //
    virtual void setupShader();
    virtual void setupViewMatrix();
    virtual void customDraw();
    virtual void drawChildren();
    virtual void restoreGLStates();
    // ******** //
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
    
    void setAnchorPoint(V3t anchorPoint);
    void setAnchorPoint(V2t anchorPoint);
    void setAnchorPoint(F1t x, F1t y);
    
    V3t anchorPoint();
    
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
    void lookAtNode(Node* node);
    M16t getLookMatrix(V3t lookAtPosition);
    
#pragma mark - SCALE METHODS
    
    void setScale(V3t scale);
    void setScale(F1t s);
    void setXScale(F1t s);
    void setYScale(F1t s);
    void setZScale(F1t s);
    
    V3t scale();
    V2t scale2d();
    
#pragma mark - TOUCH
    
    void setUserInteractionEnabled(bool enabled);
    bool userInteractionEnabled() {return _userInteractionEnabled;};
    
    void setUXEventBlock(std::function<void(shared_ptr<UXEvent>)> eventBlock);
    void setUpdateBlock(std::function<void(F1t)> updateBlock){_updateBlock = updateBlock;};
    
    virtual void handleUXEvent(shared_ptr<UXEvent> event);
    
    // UTIL
    void logCoords();
    void logPosition();
    void logMatrix(M16t M16);
    
#pragma mark - ACTIONS
    
    void runAction(shared_ptr<Action> action);
    void runAction(shared_ptr<Action> action, CompletionBlock completion);
    void runAction(shared_ptr<Action> actionm, const string& key);
    
    void runCompletionBlockForAction(shared_ptr<Action> action);
    
    U1t hasActions();
    
    shared_ptr<Action> actionForKey(const string& key);
    
    void removeAction(shared_ptr<Action> action);
    void removeActionForKey(const string& key);
    void removeAllActions();

#pragma mark - DEBUG
    
    static unsigned int createdObjects;
    static unsigned int destroyedObjects;
    static unsigned int liveObjects(){return createdObjects - destroyedObjects;};
};

bool excecuteAction(Action *action, Node* node, F1t dt);

#endif

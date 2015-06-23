//
//  Node.m
//  example-ofxTableView
//
//  Created by Chroma Developer on 2/17/14.
//
//

#include "Node.h"
#include "Action.h"
#include "FrameBuffer.h"
#include "ShaderProgram.h"
#include "Scene.h"
#include "Mesh.h"
#include "UXEvent.h"

#define TEST_IGNORE 1
#pragma mark - init

unsigned int Node::createdObjects {0};
unsigned int Node::destroyedObjects {0};

bool excecuteAction(Action *action, Node* node, F1t dt);

shared_ptr<Node> Node::node(){
    return make_shared<Node>();
}

shared_ptr<Node> Node::node(V3t size){
    return make_shared<Node>(Color(1), size);
}

shared_ptr<Node> Node::node(Color color, V3t size){
    return make_shared<Node>(color, size);
}

Node::Node(Color color, V3t size) : _color(color) {
    createdObjects++;
    SizeProperty<V3>::setSize(size);
    setScale(1.0);
    setOrientationEuler(V3(0));
    setPosition(V3(0));
    _upVector = V3(0,1,0);
}

#pragma mark - Node Hierarchy

vector<shared_ptr<Node>> Node::children() {
    return _children;
}

shared_ptr<Node> Node::child() {
    return _children[0];
}

void Node::addChild(shared_ptr<Node> child) {
    child->siblingPosition = (int)_children.size();
    child->setParent(this);
    _children.push_back(child);
}

void Node::insertChild(shared_ptr<Node> child, int atIndex){
    if (!_children.size()) {
        addChild(child);
    }
    else {
        child->siblingPosition = atIndex;
        child->setParent(this);
        _children.insert(_children.begin() + atIndex, child);
        setDirty();
    }
}

void Node::fadeInChild(shared_ptr<Node> child, F1t seconds, CompletionBlock completion) {
    addChild(child);
    child->_alpha = 0;
    completion();
}

void Node::fadeOutChild(shared_ptr<Node> child, F1t seconds, CompletionBlock completion) {
    
    child->removeFromParent();
    completion();
    //    [child runAction:[Action fadeAlphaTo:0. duration:seconds] completion:^{
    //
    //        if (block){
    //            block();
    //        }
    //
    //        [child removeFromParent];
    //
    //    }];
    //
}

void Node::setUserInteractionEnabled(bool enabled) {
    
    _userInteractionEnabled = enabled;
    
    if (_userInteractionEnabled && _parent) {
        if (!uidColor) {
            
            //[ShaderManager newUIDColorForNode:self];
        }
        _parent->setUserInteractionEnabled(true);
    }
    
}


void Node::setParent(Node *parent){
    if (_parent) {
        V3t p = globalPosition();
        //NKLogV3(@"global position", p);
        _parent->removeChild(shared_from_this());
        _parent = parent;
        setGlobalPosition(p);
    }
    else {
        _parent = parent;
    }
    
    if (_userInteractionEnabled && _parent) {
        _parent->setUserInteractionEnabled(true);
    }
    
    setDirty();
}

Node* Node::parent() {
    return _parent;
}

int Node::numNodes() {
    
    int count = 0;
    
    for (auto child : _children) {
        count += child->numNodes();
        count++;
    }
    
    return count;
    
}

int Node::numVisibleNodes() {
    
    int count = 0;
    
    for (auto child : _children) {
        if (!child->hidden) {
            count += child->numVisibleNodes();
            count++;
        }
    }
    
    return count;
    
}

void Node::removeChildrenInArray(vector<shared_ptr<Node>> nodes){
    _c::subtract(_children, nodes);
}

void Node::removeAllChildren(){
    auto copy = _children;

    for (auto child : copy) {
        child->removeFromParent();
    }
    
    _children.clear();
}


void Node::removeChild(shared_ptr<Node> node) {
    _c::erase(_children, node);
    setDirty();
}

void Node::removeChildNamed(string &name){
    childNodeNamed(name)->removeFromParent();
}

shared_ptr<Node> Node::childNodeNamed(string &name){
    for (auto n : _children) {
        if (n->name == name) {
            return n;
        }
    }
    return nullptr;
}

shared_ptr<Node> Node::randomChild() {
    if (!_children.size()) {
        return shared_from_this();
    }
    return _children[random() % _children.size()];
}

shared_ptr<Node> Node::randomLeaf() {
    if (_children.size()) {
        return randomChild()->randomLeaf();
    }
    return shared_from_this();
}

// THIS GETS EXPENSIVE WITH A LOT OF NODES - QUERY Scene::nodes FOR DEBUG
vector<shared_ptr<Node>>Node::allChildren(){
    
    auto allChildren = _children;
    
    for (auto child : _children){
        for (auto rec : child->allChildren()){
            allChildren.push_back(child);
        }
    }
    
    return allChildren;
}

void Node::removeFromParent(){
    
    removeAllActions();
    removeAllChildren();
    
    if (_parent){
        _parent->removeChild(shared_from_this());
    }
}

Node::~Node() {
    removeAllActions();
    for (auto child : _children){
        child->removeAllActions();
        child->removeFromParent();
    }
    destroyedObjects++;
}

#pragma mark - UPDATE / DRAW
void Node::updateWithTimeSinceLast(F1t dt){
    // IF OVERRIDE, CALL SUPER
    
    //    if (_body && _body.isDynamic){
    //        [_body getTransform:&_localTransform];
    //        //NKLogV3(@"update physics position: ", V3GetM16Translation(localTransform));
    //        _dirty = true;
    //    }
    
    vector<shared_ptr<Action>> complete;
    
    auto runable = actions;
    for (auto action : runable){
        if (excecuteAction(action.get(), this, dt)) complete.push_back(action);
    }
    for (auto action : complete){
        removeAction(action);
    }
    
    if (_updateBlock){
        _updateBlock(dt);
    }
    
    auto copy = _children;
    for (auto child : copy) {
        child->updateWithTimeSinceLast(dt);
    }
}

void Node::pushStyle(){
    GLState::setCullFace(_cullFace);
    GLState::setBlendMode(_blendMode);
    GLState::setUsesDepth(_usesDepth);
}


void Node::drawMultiPass(int passes){
    
    
    //    Mesh* fboSurface = [NKStaticDraw fboSurface];
    //
    //    if (passes % 2 == 1) { // odd number of passes
    //        passes++;
    //    }
    //
    //    for (int i = 0; i < passes;i++) {
    //
    //        [[_shader uniformNamed:NKS_INT_CURRENT_PASS] bindI1:i];
    //
    //        if (i == 0) { // first pass draw geometry / setupFBO
    //            if (!_framebuffer) {
    //                _framebuffer = [[FrameBuffer alloc]initWithWidth:self.size.width height:self.size.height];
    //            }
    //
    //            [_framebuffer bindPong];
    //            [_framebuffer clear];
    //
    //            [self customDraw];
    //            [self drawChildren];
    //
    //            fboSurface.size = self.size;
    //            fboSurface.scene = self.scene;
    //        }
    //        else if (i%2 == 0) {
    //            [_framebuffer bindPong];
    //            [fboSurface setTexture:self.framebuffer.renderTexture];
    //        }
    //        else {
    //            [_framebuffer bindPing];
    //            [fboSurface setTexture:_framebuffer.renderTexture2];
    //        }
    //
    //        if (i != 0){ // set my shader (for scene etc)
    //            if (self.postProcess) {
    //                self.scene.activeShader = self.postProcess;
    //            }
    //            [[_shader uniformNamed:NKS_INT_CURRENT_PASS] bindI1:i];
    //
    //            [fboSurface customDraw];
    //        }
    //    }
    
}

void Node::drawChildren() {
    
    vector<shared_ptr<Node>> toDraw;
    
    if (cullable) { // 99% only set non cullable on skybox etc.
        for (int i = 0; i < _children.size(); i++) {
            auto child = _children[i];
            
            child->modelViewCache = M16Multiply(scene()->camera->viewMatrix(),M16ScaleWithV3(child->globalTransform(), child->size()));
            
            // FAST CULL
            if (child->modelViewCache.m32 <= .01 || !child->cullable) {
                toDraw.push_back(child);
            }
        }
        if (toDraw.size() > 1) {
            sort(toDraw.begin(), toDraw.end(),
                 [](const shared_ptr<Node> a, const shared_ptr<Node> b)
                 {
                     // true means a push back
                     if (a->isOpaque() && b->isOpaque()) {
                         if (a->modelViewCache.m32 == b->modelViewCache.m32){
                             return a->siblingPosition < b->siblingPosition;
                         }
                         return a->modelViewCache.m32 < b->modelViewCache.m32;
                     }
                     else if (a->isOpaque()) {
                         return true;
                     }
                     else if (b->isOpaque()){
                         return false;
                     }
                     if (a->modelViewCache.m32 == b->modelViewCache.m32){
                         return a->siblingPosition < b->siblingPosition;
                     }
                     return a->modelViewCache.m32 < b->modelViewCache.m32;
                 });
        }
        for (auto child : toDraw) {
            child->draw();
        }
    }
    else {
        for (auto child : _children) {
            child->draw();
        }
    }
    
}

void Node::draw() {
    if (hidden) return;
    
    setupShader();
    nkGetGLError();

    if (shader && shader->numPasses) {
        // MOVE TO drawMultiStartingFBO
        // [self drawMultiPass:_shader.numPasses];
    }
    else {
        customDraw();
        nkGetGLError();
        restoreGLStates();
        nkGetGLError();
        drawChildren();
    }
    
    setDirty(false);
}

void Node::restoreGLStates(){
    if (_ignoresLights){
        if (Shader::activeShader){
            if (Shader::activeShader->hasUniformNamed(NKS_I1_NUM_LIGHTS)) {
                Shader::activeShader->uniformNamed(NKS_I1_NUM_LIGHTS).bindI1(scene()->lights.size());
            }
        }
    }
}

void Node::setupViewMatrix() {
}

void Node::setupShader() {
    if (!shader) {
        chooseShader();
    }
    if (shader) {
        if (shader->use()){
            if (shader->hasUniformNamed(NKS_I1_NUM_LIGHTS)) {
                scene()->bindLights();
            }
        }
        if (_ignoresLights){
            if (Shader::activeShader->hasUniformNamed(NKS_I1_NUM_LIGHTS)) {
                Shader::activeShader->uniformNamed(NKS_I1_NUM_LIGHTS).bindI1(0);
            }
        }
    }
}

void Node::drawWithHitShader() {
    if (_userInteractionEnabled) {
        customDrawWithHitShader();
        for (auto child : _children) {
            child->drawWithHitShader();
        }
    }
}

void Node::customDraw() {
    // OVERRIDE IN SUB CLASS
}

void Node::customDrawWithHitShader() {
    // OVERRIDE IN SUB CLASS
}

#pragma mark - GEOMETRY

//-(P2t)inverseProjectedPoint:(P2t)location {
//
//    M16t globalTransform = [self globalTransform];
//
//    //    bool isInvertible;
//
//    V3t transformed = V3MultiplyM16(globalTransform, V3(location.x, location.y, 0));
//
//    //    if (!isInvertible) {
//    //        NSLog(@"node inversion failed");
//    //    }
//
//    P2t p = V2(transformed.x, transformed.y);
//
//    return p;
//
//}


V3t Node::globalPosition(){
    return V3GetM16Translation(globalTransform());
}

bool Node::shouldCull() {
    return 0;
}

#pragma mark - MATH



#pragma mark - PROPERTIES

#pragma mark - MATRIX

void Node::setLocalTransform(M16t localTransform){
    setDirty();
    
    _localTransform = localTransform;
    _scale = V3GetM16Scale(_localTransform);
    
    //    if (_body) {
    //        [_body setTransform:[self globalTransform]];
    //    }
    
}

M16t Node::globalTransform() {
    
    if (_gtDirty) {
        _gtDirty = false;
        if (!_parent) {
            return _cachedGlobalTransform = _localTransform;
        }
        return _cachedGlobalTransform = M16Multiply(_parent->globalTransform(),_localTransform);
    }
    else {
        return _cachedGlobalTransform;
    }
    
}

#pragma mark - POSITION

V3t Node::position() {
    return _position;
}

V2t Node::screenPosition() {
    return globalPosition().xy + (scene()->size().xy * .5);
}

// BASE, all position methods should call this:
void Node::setPosition(V3t position){
    _position = position;
    M16SetV3Translation(&_localTransform, _position);
    
    setDirty();
    
    //    if (_body){
    //        [_body setTransform:[self globalTransform]];
    //    }
}

void Node::setPosition(V2t position){
    setPosition({position.x, position.y, _position.z});
}

void Node::setPosition(F1t x, F1t y){
    setPosition({x, y, _position.z});
}

void Node::setPosition(F1t x, F1t y, F1t z){
    setPosition({x, y, z});
}

void Node::setXPosition(float position){
    _position.x = position;
    setPosition(_position);
}
void Node::setYPosition(float position){
    _position.y = position;
    setPosition(_position);
}
void Node::setZPosition(float position){
    _position.z = position;
    setPosition(_position);
}

void Node::setGlobalPosition(V3t p){
    if(_parent == NULL) {
        setPosition(p);
    } else {
        M16t global = _parent->globalTransform();
        M16Invert(&global);
        setPosition(V3MultiplyM16WithTranslation(global, p));
    }
}

void Node::setDirty(bool dirty){
    _dirty = dirty;
    
    if (_dirty) {
        _gtDirty = _dirty;
        for (auto node : _children){
            node->_gtDirty = true;
        }
        if (_parent) {
            _parent->setDirty();
        }
    }
}

M16t Node::transformMatrixInNode(Node* n){
    
    if (_parent == n || !_parent) {
        return _localTransform;
    }
    else {
        // recursive add
        return M16Multiply(_parent->transformMatrixInNode(n), _localTransform);
    }
}

P2t Node::positionInNode(Node *node){
    return positionInNode3d(node).xy;
}

V3t Node::positionInNode3d(Node *node){
    return V3GetM16Translation(transformMatrixInNode(node));
}

#pragma mark - ANCHOR

void Node::setAnchorPoint(V3t anchorPoint){
    _anchorPoint = anchorPoint;
}

V3t Node::anchorPoint() {
    return _anchorPoint;
}

void Node::setAnchorPoint(V2t anchorPoint){
    _anchorPoint.x = anchorPoint.x;
    _anchorPoint.y = anchorPoint.y;
}

void Node::setAnchorPoint(F1t x, F1t y){
    _anchorPoint.x = x;
    _anchorPoint.y = y;
}

#pragma mark - Orientation

M16t Node::localTransform(){
    return _localTransform;
}

void Node::createMatrix(){
    setDirty();
    
    _localTransform = M16Multiply(M16MakeScale(_scale), M16MakeRotate(_orientation));
    M16SetV3Translation(&(_localTransform), _position);
    
#warning fix bullet
    //    if (_body) {
    //        [_body setTransform:[self globalTransform]];
    //    }
}

//----------------------------------------
void Node::setOrientation(const Q4t q){
    _orientation = q;
    createMatrix();
}

//----------------------------------------
void Node::setOrientationEuler(const V3t eulerAngles){
    setOrientation(Q4FromV3(eulerAngles));
}

Q4t Node::orientation(){
    return _orientation;
    // return Q4FromMatrix(_localTransform);
}

Q4t Node::getGlobalOrientation() {
    return Q4GetM16Rotate(globalTransform());
}

V3t Node::getOrientationEuler() {
    return V3FromQ4(Q4GetM16Rotate(globalTransform()));
}

F1t Node::getYOrientation() {
    V3t raw = V3FromQ4(Q4GetM16Rotate(_localTransform));
    
    if (raw.x < 0) {
        return -raw.y;
    }
    
    return raw.y;
}

void Node::setZRotation(F1t rotation){
    Q4t zRot = Q4FromAngleAndV3(rotation, V3(0,0,1));
    setOrientation(zRot);
}

void Node::setOrbit(V3t orbit){
    longitude = orbit.x;
    latitude = orbit.y;
    radius = orbit.z;
    
    //    if (_latitude >= 360) _latitude-=360.;
    //    else if (_latitude <= -360) _latitude+=360.;
    //    if (_longitude >= 360) _longitude-=360.;
    //    else if (_longitude <= -360) _longitude+=360.;
}

V3t Node::currentOrbit() {
    return orbitForLongitude(longitude, latitude, radius);
}

V3t Node::orbitForLongitude(float longitude, F1t latitude, F1t radius){
    V3t p = V3RotatePoint(V3(0, 0, radius), longitude, V3(0, 1, 0));
    return V3RotatePoint(p, latitude, V3(1, 0, 0));
}

void Node::rotateMatrix(M16t M16){
    M16t m = M16MakeScale(_scale);
    _localTransform = M16Multiply(m,M16);
    M16SetV3Translation(&_localTransform, _position);
}

void Node::lookAtNode(Node *node){
    lookAtPoint(node->globalPosition());
}

void Node::lookAtPoint(V3t point){
    
    M16t lookMatrix = getLookMatrix(point);
    rotateMatrix(lookMatrix);
}

M16t Node::getLookMatrix(V3t lookAtPosition){
    
    return M16MakeLookAt(globalPosition(), lookAtPosition, upVector());
}

V3t Node::upVector() {
    if (!_parent){
        return _upVector;
    }
    return V3MultiplyM16(_parent->globalTransform(), _upVector);
}

#pragma mark - SCALE

void Node::setXScale(F1t s){
    V3t nScale = _scale;
    nScale.x = s;
    setScale(nScale);
}

void Node::setYScale(F1t s){
    V3t nScale = _scale;
    nScale.y = s;
    setScale(nScale);
}

void Node::setZScale(F1t s){
    V3t nScale = _scale;
    nScale.z = s;
    setScale(nScale);
}

void Node::setScale(F1t s) {
    _scale = V3(s);
}

void Node::setScale(V3t scale){
    _scale = scale;
    createMatrix();
}

V3t Node::scale(){
    return _scale;
}

V2t Node::scale2d(){
    return _scale.xy;
}

#pragma mark - ALPHA / BLEND

void Node::setTransparency(F1t transparency){
    intAlpha = transparency;
    _alpha = transparency * parentAlpha;
}

void Node::setAlpha(F1t alpha){
    intAlpha = alpha;
    _alpha = intAlpha * parentAlpha;
    
    for (auto n : _children){
        n->recursiveAlpha(_alpha);
    }
}

void Node::recursiveAlpha(F1t alpha){
    parentAlpha = alpha;
    _alpha = intAlpha * parentAlpha;
    
    for (auto n : _children){
        n->recursiveAlpha(_alpha);
    }
}

bool Node::isOpaque() {
    if (_alpha < 1.0) return false;
    if (_color.a < 1.0) return false;
    return true;
//    
//    if (_alpha < 1.0) return false;
//    for (auto child : _children){
//        if (child->_alpha < 1.0) return false;
//    }
//    return true;
}

#pragma mark - SHADER / FBO

void Node::chooseShader() {
}

#pragma mark - COLOR

F1t Node::colorBlendFactor() {
    return _colorBlendFactor;
}

void Node::setColorBlendFactor(F1t colorBlendFactor){
    _colorBlendFactor = colorBlendFactor;
}

void Node::setColor(Color color){
    _color = color;
}

Color& Node::color() {
    return _color;
}

V4t Node::glColor() {
    if (_colorBlendFactor == 1.0f) {
        return _color.fade(_alpha);
    }
    else {
        return _color.blend(_colorBlendFactor).fade(_alpha);
    }
}

#pragma mark - EVENT HANDLING

void Node::setUXEventBlock(std::function<void(shared_ptr<UXEvent> event)> block){
    _eventBlock = block;
}

void Node::handleUXEvent(shared_ptr<UXEvent> event){
    if (_eventBlock) {
        _eventBlock(event);
    }
}

#pragma mark - ANIMATION HANDLER

bool excecuteAction(Action *action, Node* node, F1t dt){
    
    if (!action) {
        //printf("error executing nil action \n");
        return true;
    }
    
    if (action->children.size()) { // GROUPS
        
        if (!action->actions.size()) {
            return action->completeWithTimeSinceLast(dt, node);
        }
        
        if (action->serial) {
            if (excecuteAction(action->actions.front().get(),node, dt)){
                action->removeAction(action->actions.front());
            }
            if (!action->actions.size()) {
                return action->completeWithTimeSinceLast(dt, node);
            }
            return false;
        }
        
        else { // parallel
            
            //printf("process group \n");
            
            bool active = false;
            
            vector<shared_ptr<Action>> complete;
            
            for (auto ac : action->actions) {
                if (ac->repeats() == 0) active = true;
                if (excecuteAction(ac.get(), node, dt)){
                    complete.push_back(ac);
                }
            }
            
            for (auto ac : complete) {
                action->removeAction(ac);
            }
            
            if (!active) {
                //printf("reset because remaining actions are loops \n");
                return (action->completeWithTimeSinceLast(dt, node));
            }
            
            return false;
        }
    }
    
    else {
        
        if (action->updateWithTimeSinceLast(dt, node)){
            return action->completeWithTimeSinceLast(dt, node);
        }
        
        return false;
        
    }
}

void Node::runCompletionBlockForAction(shared_ptr<Action> action){
    auto block = action->completionBlock;
    block();
}

void Node::runAction(shared_ptr<Action> action){
    if (action) {
        actions.push_back(action);
        
        action->parentNode = this;
        action->sharedReset();
    }
}

void Node::runAction(shared_ptr<Action> action, const string &key){
    action->key = key;
    runAction(action);
}

shared_ptr<Action> Node::actionForKey(const string &key){
    
    for (auto action : actions) {
        if (action->key == key) {
            return action;
        }
        
        // TODO WRITE RECURSIVE FUNCITON
        
        for (auto child : action->children) {
            if (child->key == key) {
                return child;
            }
        }
    }
    
    return nullptr;
}

void Node::removeAction(shared_ptr<Action> action){
    _c::erase(actions, action);
}

void Node::removeActionForKey(const string &key){
    shared_ptr<Action> action = actionForKey(key);
    if (action) {
        if (action->parentAction) action->parentAction->removeAction(action);
        else removeAction(action);
    }
}

void Node::runAction(shared_ptr<Action> action, CompletionBlock block){
    action->completionBlock = block;
    runAction(action);
}

U1t Node::hasActions(){
    return (U1t)actions.size();
}


void Node::removeAllActions() {
    for (auto action : actions){
        action->stop();
    }
    actions.clear();
}



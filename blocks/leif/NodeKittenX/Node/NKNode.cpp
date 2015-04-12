//
//  NKNode.m
//  example-ofxTableView
//
//  Created by Chroma Developer on 2/17/14.
//
//

#include "NKNode.h"
#include "NKAction.h"
#include "NKFrameBuffer.h"
#include "NKShaderProgram.h"
#include "NKSceneNode.h"
#include "NKMeshNode.h"
#include "NKEvent.h"

#define TEST_IGNORE 1
#pragma mark - init

bool excecuteAction(NKAction *action, NKNode* node, F1t dt);

shared_ptr<NKNode> NKNode::node(){
    return make_shared<NKNode>();
}

NKNode::NKNode() : NKNode::NKNode(V3Make(1., 1., 1.)){
}

NKNode::NKNode(V3t size) : _color(0,0,0,0){
    
    name = make_shared<string>("NEW NODE");
    
    setSize(size);
    setScaleF(1.0);
    setOrientationEuler(V3MakeF(0));
    setPosition(V3MakeF(0));
    _upVector = V3Make(0,1,0);
    
}

//-(instancetype)initWithSize:(V3t)size {
//
//    self = [super init];
//
//    if (self) {
//        _size = size;
//    }
//
//    return self;
//}

#pragma mark - Node Hierarchy

vector<shared_ptr<NKNode>> NKNode::children() {
    return _children;
}

void NKNode::addChild(shared_ptr<NKNode> child) {
    _children.push_back(child);
    child->setParent(this);
}


void NKNode::insertChild(shared_ptr<NKNode> node, int atIndex){
    if (!_children.size()) {
        _children.push_back(node);
        return;
    }
    
    _children.insert(_children.begin() + atIndex, node);
}

void NKNode::fadeInChild(shared_ptr<NKNode> child, F1t seconds, CompletionBlock completion) {
    
    addChild(child);
    child->_alpha = 0;
    completion();
}

void NKNode::fadeOutChild(shared_ptr<NKNode> child, F1t seconds, CompletionBlock completion) {
    
    child->removeFromParent();
    completion();
    //    [child runAction:[NKAction fadeAlphaTo:0. duration:seconds] completion:^{
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

void NKNode::setUserInteractionEnabled(bool enabled) {
    
    _userInteractionEnabled = enabled;
    
    if (_userInteractionEnabled && _parent) {
        if (!uidColor) {
            
            //[NKShaderManager newUIDColorForNode:self];
        }
        _parent->setUserInteractionEnabled(true);
    }
    
}


void NKNode::setParent(NKNode *parent){
    
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
    
    setScene(parent->scene());
    
    if (_userInteractionEnabled && _parent) {
        _parent->setUserInteractionEnabled(true);
    }
}

void NKNode::setScene(NKSceneNode *scene){
    if (_userInteractionEnabled && !uidColor) {
        //[NKShaderManager newUIDColorForNode:self];
    }
    _scene = scene;
}

NKSceneNode *NKNode::scene() {
    
    if (!_scene) { // CACHE POINTER
        
        if (_parent) {
            _scene = _parent->scene();
            return _parent->scene();
        }
        
        return nullptr;
    }
    
    return _scene;
    
}

NKNode* NKNode::parent() {
    return parent();
}

S2t NKNode::size2d() {
    return _size.xy;
}

V3t NKNode::size() {
    return _size;
}

void NKNode::setSize(S2t size) {
    setSize(V3Make(size.x, size.y, _size.z == 0 ? 1 : _size.z));
}

void NKNode::setSize(V3t size) {
    _size = size;
}

int NKNode::numNodes() {
    
    int count = 0;
    
    for (auto child : _children) {
        count += child->numNodes();
        count++;
    }
    
    return count;
    
}

int NKNode::numVisibleNodes() {
    
    int count = 0;
    
    for (auto child : _children) {
        if (!child->hidden) {
            count += child->numVisibleNodes();
            count++;
        }
    }
    
    return count;
    
}

R4t NKNode::calculateAccumulatedFrame() {
    
    R4t rect = getDrawFrame();
    
    for (auto child : _children) {
        
        R4t childFrame = child->getDrawFrame();
        
        if (childFrame.x < rect.x) {
            rect.x = childFrame.x;
        }
        
        
        if (childFrame.x + childFrame.size.width > rect.x + rect.size.width) {
            rect.size.width = rect.x + childFrame.x + childFrame.size.width;
        }
        
        if (childFrame.y < rect.y) {
            rect.y = childFrame.y;
        }
        
        
        if (childFrame.y + childFrame.size.height > rect.y + rect.size.height) {
            rect.size.height = rect.y + childFrame.y + childFrame.size.height;
        }
        
    }
    
    return rect;
}

void NKNode::removeChildrenInArray(vector<shared_ptr<NKNode>> nodes){
    substract_vector(_children, nodes);
}

void NKNode::removeAllChildren(){
    auto copy = _children;
    for (auto child : copy) {
        child->removeFromParent();
    }
    _children.clear();
}


void NKNode::removeChild(shared_ptr<NKNode> node) {
    erase_object(_children, node);
}

void NKNode::removeChildNamed(string &name){
    childNodeNamed(name)->removeFromParent();
}

shared_ptr<NKNode> NKNode::childNodeNamed(string &name){
    for (auto n : _children) {
        if (*n->name == name) {
            return n;
        }
    }
    return nullptr;
}

//-(NKNode*)randomChild {
//    if (!_children.count) {
//        return self;
//    }
//    return _children[arc4random() % _children.count];
//}
//
//-(NKNode*)randomLeaf {
//
//    if (_children.count) {
//        return [[self randomChild] randomLeaf];
//    }
//
//    return self;
//
//}


vector<shared_ptr<NKNode>>NKNode::allChildren(){
    
    auto allChildren = _children;
    
    for (auto child : _children){
        for (auto rec : child->allChildren()){
            allChildren.push_back(child);
        }
    }
    
    return allChildren;
}

void NKNode::removeFromParent(){
    
    removeAllActions();
    
    auto copy = _children;
    
    for (auto child : copy){
        child->removeFromParent();
    }
    
    if (_parent){
        _parent->removeChild(shared_from_this());
    }
    
    if (_scene){
        // _scene->removeChild(this);
    }
}

NKNode::~NKNode() {
    removeAllActions();
    for (auto child : _children){
        child->removeAllActions();
        child->removeFromParent();
    }
}

#pragma mark - UPDATE / DRAW
void NKNode::updateWithTimeSinceLast(F1t dt){
    // IF OVERRIDE, CALL SUPER
    
    //    if (_body && _body.isDynamic){
    //        [_body getTransform:&_localTransform];
    //        //NKLogV3(@"update physics position: ", V3GetM16Translation(localTransform));
    //        _dirty = true;
    //    }
    
    vector<shared_ptr<NKAction>> complete;
    
    auto runable = actions;
    for (auto action : runable){
        if (excecuteAction(action.get(), this, dt)) complete.push_back(action);
    }
    for (auto action : complete){
        removeAction(action);
    }
    
    auto copy = _children;
    for (auto child : copy) {
        child->updateWithTimeSinceLast(dt);
    }
}

void NKNode::pushStyle(){
    _scene->setCullFace(_cullFace);
    _scene->setBlendMode(_blendMode);
    _scene->setUsesDepth(_usesDepth);
}

void NKNode::drawMultiPass(int passes){
    
    
    //    NKMeshNode* fboSurface = [NKStaticDraw fboSurface];
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
    //                _framebuffer = [[NKFrameBuffer alloc]initWithWidth:self.size.width height:self.size.height];
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

void NKNode::drawChildren() {

    vector<shared_ptr<NKNode>> toDraw;

    for (int i = 0; i < _children.size(); i++) {
        auto child = _children[i];

        child->modelViewCache = M16Multiply(scene()->camera->viewMatrix(),M16ScaleWithV3(child->globalTransform(), child->size()));

        // FAST CULL
        if (child->modelViewCache.m32 < 0) {
            toDraw.push_back(child);
        }
        else {
            // nkLog("cull obj 'z' %1.2f", child->modelViewCache.m32);
        }
    }

    sort(toDraw.begin(), toDraw.end(),
            [](const shared_ptr<NKNode> a, const shared_ptr<NKNode> b)
            {
                return a->modelViewCache.m32 < b->modelViewCache.m32;
            });

    for (auto child : _children) {
        child->draw();
    }
    
}

void NKNode::draw() {
    
    if (hidden) return;
    
    if (_shader) {
        _scene->setActiveShader(_shader.get());
    }
    
    if (_shader && _shader->numPasses) {
        // MOVE TO drawMultiStartingFBO
        // [self drawMultiPass:_shader.numPasses];
    }
    
    else {
        if (framebuffer) {
            framebuffer->bind();
            framebuffer->clear();
        }
        nkGetGLError();
        customDraw();
        nkGetGLError();
        drawChildren();
    }
    
    if (framebuffer) {
        _scene->bindMainFrameBuffer(this);
        
        NKMeshNode&  fboSurface = NKMeshNode::fboSurface();
        
        fboSurface.setTexture(framebuffer->renderTexture);
        fboSurface.setSize(V3Make(framebuffer->width, framebuffer->height, 1));
        fboSurface.setScene(_scene);
        
        if (_postProcess) {
            _scene->setActiveShader(_postProcess.get());
        }
        else {
            _scene->setActiveShader(fboSurface._shader.get());
        }
        
        fboSurface.customDraw();
    }
}


void NKNode::setupViewMatrix() {
}

void NKNode::drawWithHitShader() {
    if (_userInteractionEnabled) {
        customDrawWithHitShader();
        for (auto child : _children) {
            child->drawWithHitShader();
        }
    }
}

void NKNode::customDraw() {
    // OVERRIDE IN SUB CLASS
}

void NKNode::customDrawWithHitShader() {
    // OVERRIDE IN SUB CLASS
}

#pragma mark - GEOMETRY

//-(P2t)inverseProjectedPoint:(P2t)location {
//
//    M16t globalTransform = [self globalTransform];
//
//    //    bool isInvertible;
//
//    V3t transformed = V3MultiplyM16(globalTransform, V3Make(location.x, location.y, 0));
//
//    //    if (!isInvertible) {
//    //        NSLog(@"node inversion failed");
//    //    }
//
//    P2t p = V2Make(transformed.x, transformed.y);
//
//    return p;
//
//}

bool NKNode::containsPoint(P2t location) {
    
    // OLD METHOD
    // ADDING LOCAL TRANSFORMATION
    
    P2t p = location;
    //P2tp = [self transformedPoint:location];
    
    //NSLog(@"world coords: %f %f %f", p.x, p.y, p.z);
    
    R4t r = getGlobalFrame();
    
    //bool withinArea = false;
    if ( p.x > r.x && p.x < r.x + r.size.width && p.y > r.y && p.y < r.y + r.size.height)
    {
        // [self logCoords];
        return true;
    }
    return false;
    
    //    P2t p = [self inverseProjectedPoint:location];
    //
    //    V3t globalPos = [self globalPosition];
    //
    //    R4t r = R4Make(globalPos.x - _size.x * _anchorPoint3d.x, globalPos.y - _size.y *_anchorPoint3d.y, _size.x, _size.y);
    //
    //    //bool withinArea = false;
    //    if ( p.x > r.x && p.x < r.x + r.size.width && p.y > r.y && p.y < r.y + r.size.height)
    //    {
    //        // [self logCoords];
    //        return true;
    //    }
    //    return false;
    
}

V3t NKNode::globalPosition(){
    return V3GetM16Translation(globalTransform());
}

R4t NKNode::getGlobalFrame(){
    V3t g = globalPosition();
    return R4Make(g.x - _size.x * _anchorPoint.x, g.y - _size.y *_anchorPoint.y, _size.x, _size.y);
}


R4t NKNode::getDrawFrame() {
    //[self logCoords];
    //V3t g = node->getPosition();
    //return R4Make(g.x - _size.width * _anchorPoint.x, g.y - _size.height *_anchorPoint.y, _size.width, _size.height);
    return R4Make(-_size.x * _anchorPoint.x, -_size.y *_anchorPoint.y, _size.x, _size.y);
}


bool NKNode::shouldCull() {
    return 0;
}

#pragma mark - MATH



#pragma mark - PROPERTIES

#pragma mark - MATRIX

void NKNode::setLocalTransform(M16t localTransform){
    setDirty(true);
    
    _localTransform = localTransform;
    _scale = V3GetM16Scale(_localTransform);
    
    //    if (_body) {
    //        [_body setTransform:[self globalTransform]];
    //    }
    
}

M16t NKNode::globalTransform() {
    
    if (_dirty) {
        _dirty = false;
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

V3t NKNode::position() {
    return _position;
}

// BASE, all position methods should call this:
void NKNode::setPosition(V3t position){
    _position = position;
    M16SetV3Translation(&_localTransform, _position);
    
    setDirty(true);
    
    //    if (_body){
    //        [_body setTransform:[self globalTransform]];
    //    }
}

void NKNode::setPosition(V2t position){
    setPosition(V3Make(position.x, position.y, _position.z));
}

void NKNode::setXPosition(float position){
    _position.x = position;
    setPosition(_position);
}
void NKNode::setYPosition(float position){
    _position.y = position;
    setPosition(_position);
}
void NKNode::setZPosition(float position){
    _position.z = position;
    setPosition(_position);
}

void NKNode::setGlobalPosition(V3t p){
    if(_parent == NULL) {
        setPosition(p);
    } else {
        M16t global = _parent->globalTransform();
        M16Invert(&global);
        setPosition(V3MultiplyM16WithTranslation(global, p));
    }
}

void NKNode::setDirty(bool dirty){
    _dirty = dirty;
    if (dirty) {
        for (auto n : _children){
            n->setDirty(dirty);
        }
    }
}


M16t NKNode::transformMatrixInNode(NKNode* n){
    
    if (_parent == n || !_parent) {
        return _localTransform;
    }
    else {
        // recursive add
        return M16Multiply(_parent->transformMatrixInNode(n), _localTransform);
    }
}

P2t NKNode::positionInNode(NKNode *node){
    return positionInNode3d(node).xy;
}

V3t NKNode::positionInNode3d(NKNode *node){
    return V3GetM16Translation(transformMatrixInNode(node));
}

#pragma mark - ANCHOR

void NKNode::setAnchorPoint(V3t anchorPoint){
    _anchorPoint = anchorPoint;
}

V3t NKNode::anchorPoint() {
    return _anchorPoint;
}

void NKNode::setAnchorPoint(V2t anchorPoint){
    _anchorPoint.x = anchorPoint.x;
    _anchorPoint.y = anchorPoint.y;
}

V2t NKNode::anchorPoint2d() {
    return V2Make(_anchorPoint.x, _anchorPoint.y);
}

#pragma mark - Orientation

M16t NKNode::localTransform(){
    return _localTransform;
}

void NKNode::createMatrix(){
    setDirty(true);
    
    _localTransform = M16Multiply(M16MakeScale(_scale), M16MakeRotate(_orientation));
    M16SetV3Translation(&(_localTransform), _position);
    
#warning fix bullet
    //    if (_body) {
    //        [_body setTransform:[self globalTransform]];
    //    }
}

//----------------------------------------
void NKNode::setOrientation(const Q4t q){
    _orientation = q;
    createMatrix();
}

//----------------------------------------
void NKNode::setOrientationEuler(const V3t eulerAngles){
    setOrientation(Q4FromV3(eulerAngles));
}

Q4t NKNode::orientation(){
    return _orientation;
    // return Q4FromMatrix(_localTransform);
}

Q4t NKNode::getGlobalOrientation() {
    return Q4GetM16Rotate(globalTransform());
}

V3t NKNode::getOrientationEuler() {
    return V3FromQ4(Q4GetM16Rotate(globalTransform()));
}

F1t NKNode::getYOrientation() {
    V3t raw = V3FromQ4(Q4GetM16Rotate(_localTransform));
    
    if (raw.x < 0) {
        return -raw.y;
    }
    
    return raw.y;
}

void NKNode::setZRotation(F1t rotation){
    Q4t zRot = Q4FromAngleAndV3(rotation, V3Make(0,0,1));
    setOrientation(zRot);
}

void NKNode::setOrbit(V3t orbit){
    longitude = orbit.x;
    latitude = orbit.y;
    radius = orbit.z;
    
    //    if (_latitude >= 360) _latitude-=360.;
    //    else if (_latitude <= -360) _latitude+=360.;
    //    if (_longitude >= 360) _longitude-=360.;
    //    else if (_longitude <= -360) _longitude+=360.;
}

V3t NKNode::currentOrbit() {
    return orbitForLongitude(longitude, latitude, radius);
}

V3t NKNode::orbitForLongitude(float longitude, F1t latitude, F1t radius){
    V3t p = V3RotatePoint(V3Make(0, 0, radius), longitude, V3Make(0, 1, 0));
    return V3RotatePoint(p, latitude, V3Make(1, 0, 0));
}

void NKNode::rotateMatrix(M16t M16){
    M16t m = M16MakeScale(_scale);
    _localTransform = M16Multiply(m,M16);
    M16SetV3Translation(&_localTransform, _position);
}

void NKNode::lookAtNode(NKNode *node){
    lookAtPoint(node->globalPosition());
}

void NKNode::lookAtPoint(V3t point){
    
    M16t lookMatrix = getLookMatrix(point);
    
    rotateMatrix(lookMatrix);
}

M16t NKNode::getLookMatrix(V3t lookAtPosition){
    
    return M16MakeLookAt(globalPosition(), lookAtPosition, upVector());
}

V3t NKNode::upVector() {
    if (!_parent){
        return _upVector;
    }
    return V3MultiplyM16(_parent->globalTransform(), _upVector);
}

#pragma mark - SCALE

void NKNode::setXScale(F1t s){
    V3t nScale = _scale;
    nScale.x = s;
    setScale(nScale);
}

void NKNode::setYScale(F1t s){
    V3t nScale = _scale;
    nScale.y = s;
    setScale(nScale);
}

void NKNode::setZScale(F1t s){
    V3t nScale = _scale;
    nScale.z = s;
    setScale(nScale);
}

void NKNode::setScaleF(F1t s) {
    _scale = V3MakeF(s);
}

void NKNode::setScale(V3t scale){
    _scale = scale;
    createMatrix();
}

V3t NKNode::scale(){
    return _scale;
}

V2t NKNode::scale2d(){
    return _scale.xy;
}

#pragma mark - ALPHA / BLEND

void NKNode::setTransparency(F1t transparency){
    intAlpha = transparency;
    _alpha = transparency * parentAlpha;
}

void NKNode::setAlpha(F1t alpha){
    intAlpha = alpha;
    _alpha = intAlpha * parentAlpha;
    
    for (auto n : _children){
        n->recursiveAlpha(_alpha);
    }
}

void NKNode::recursiveAlpha(F1t alpha){
    parentAlpha = alpha;
    _alpha = intAlpha * parentAlpha;
    
    for (auto n : _children){
        n->recursiveAlpha(_alpha);
    }
}

#pragma mark - SHADER / FBO

void NKNode::setPostProcess(shared_ptr<NKShaderProgram> postProcess){
    _postProcess = postProcess;
    
    if (!framebuffer) {
        framebuffer = make_shared<NKFrameBuffer>((GLuint)_size.width, (GLuint)_size.height);
    }
}

void NKNode::setShader(shared_ptr<NKShaderProgram> shader){
    _shader = shader;
}

#pragma mark - GL / STATE

void NKNode::setLineSmooth(bool smooth){
    
}

#pragma mark - COLOR

F1t NKNode::colorBlendFactor() {
    return _colorBlendFactor;
}

void NKNode::setColorBlendFactor(F1t colorBlendFactor){
    _colorBlendFactor = colorBlendFactor;
}

void NKNode::setColor(NKColor color){
    _color = color;
}

NKColor& NKNode::color() {
    return _color;
}

C4t NKNode::glColor() {
    C4t color;
    
    if (_colorBlendFactor == 1.0f) {
        color = _color.data;
    }
    else {
        color = _color.colorWithBlendFactor(_colorBlendFactor);
    }
    
    color.a *= _alpha;
    
    return color;
}

#pragma mark - EVENT HANDLING

void NKNode::setEventBlock(EventBlock block){
    _eventBlock = block;
}

void NKNode::handleEvent(NKEvent& event){
    if (_eventBlock) {
        _eventBlock(event);
    }
}

#pragma mark - ANIMATION HANDLER

bool excecuteAction(NKAction *action, NKNode* node, F1t dt){
    
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
            
            vector<shared_ptr<NKAction>> complete;
            
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

void NKNode::runCompletionBlockForAction(shared_ptr<NKAction> action){
    auto block = action->completionBlock;
    block();
}

void NKNode::runAction(shared_ptr<NKAction> action){
    if (action) {
        actions.push_back(action);
        
        action->parentNode = this;
        action->sharedReset();
    }
}

void NKNode::runAction(shared_ptr<NKAction> action, string &key){
    action->key = key;
    runAction(action);
}

shared_ptr<NKAction> NKNode::actionForKey(string &key){
    
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

void NKNode::removeAction(shared_ptr<NKAction> action){
    erase_object(actions, action);
}

void NKNode::removeActionForKey(string &key){
    shared_ptr<NKAction> action = actionForKey(key);
    if (action) {
        if (action->parentAction) action->parentAction->removeAction(action);
        else removeAction(action);
    }
}

void NKNode::runAction(shared_ptr<NKAction> action, CompletionBlock block){
    action->completionBlock = block;
    runAction(action);
}

U1t NKNode::hasActions(){
    return (U1t)actions.size();
}


void NKNode::removeAllActions() {
    for (auto action : actions){
        action->stop();
    }
    actions.clear();
}



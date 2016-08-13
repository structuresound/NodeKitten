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

using namespace std;
using namespace Shader;

bool excecuteAction(Action *action, Node* node, F1t dt);

Node::Node(Color color, V3t size_)
{
  setColor(color);
  position.set(0);
  size.set(size_);
  scale.set(V3{1});
  orientation.set(Q4Identity);
  _upVector = V3(0,1,0);
  
  globalTransform.setPullFunction([this]{
    if (this->parent()) {
      return M16Multiply(parent()->globalTransform.get(), transform.get());
    }
    return transform.get();
  });
  globalTransform.afterSet([this](bool changed){
    modelViewMatrix.setDirty();
  });
  modelViewMatrix.afterSet([this](bool changed){
    modelViewProjectionMatrix.setDirty();
  });
  modelViewMatrix.setPullFunction([this]{
    M16t v = forceOrthographic() ? M16MakeLookAt({0,0,1}, {0,0,0}, {0,1,0}) : scene()->camera->viewMatrix.get();
    return M16Multiply(v,M16ScaleWithV3(globalTransform.get(), size.get()));
  });
  modelViewProjectionMatrix.setPullFunction([this]{
    M16t p = forceOrthographic() ? scene()->orthographicMatrix() : scene()->camera->projectionMatrix.get();
    return M16Multiply(p, modelViewMatrix.get());
  });
}

#pragma mark - Node Hierarchy

void Node::fadeInChild(shared_ptr<Node>& child, F1t seconds, CompletionBlock completion) {
  addChild(child);
  child->setAlpha(0);
  completion();
}

void Node::fadeOutChild(shared_ptr<Node>& child, F1t seconds, CompletionBlock completion) {
  
  child->removeFromParent();
  completion();
}

void Node::setUserInteractionEnabled(bool enabled) {
  
  _userInteractionEnabled = enabled;
  
  if (_userInteractionEnabled && parent()) {
    if (!uidColor) {
      
      //[ShaderManager newUIDColorForNode:self];
    }
    parent()->setUserInteractionEnabled(true);
  }
  
}

int Node::numVisibleNodes() {
  
  int count = 0;
  
  for (auto& child : children()) {
    if (!child->hidden()) {
      count += child->numVisibleNodes();
      count++;
    }
  }
  
  return count;
  
}

void Node::beforeRemove(){
  removeAllActions();
  removeAllChildren();
}

//Node::~Node() {
//    beforeRemove();
//}

#pragma mark - UPDATE / DRAW
void Node::updateWithTimeSinceLast(F1t dt){
  // IF OVERRIDE, CALL SUPER
  
  //    if (_body && _body.isDynamic){
  //        [_body getTransform:&_localTransform];
  //        //NKLogV3(@"update physics position: ", V3GetM16Translation(localTransform));
  //        _dirty = true;
  //    }
  
  //  _::filterWithLambda(actions, [this, dt](auto action){
  //    return excecuteAction(action.get(), this, dt);
  //  });
  
  for (auto action : _::rawPtrCopy(actions)){
    if (excecuteAction(action, this, dt)){
      _::eraseSmartFromRaw(actions, action);
    }
  }
  
  if (_updateBlock){
    _updateBlock(dt);
  }
  
    for (auto child : _::rawPtrCopy(children())){
      child->updateWithTimeSinceLast(dt);
    }
  
  //  _::eachMutable(children(), [dt](auto& child){
  //    child->updateWithTimeSinceLast(dt);
  //  });
  
}

void Node::pushStyle(){
  GLState::setCullFace(cullFace());
  GLState::setBlendMode(blendMode());
  GLState::setUsesDepth(usesDepth());
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

bool Node::nearestOpaqueThenFurthestTransparent (const Node* a, const Node* b) {
  // true means a push back
  if (a->isOpaque() && b->isOpaque()) {
    if (a->modelViewMatrix.get().m32 == b->modelViewMatrix.get().m32){
      return a->siblingPosition() < b->siblingPosition();
    }
    return a->modelViewMatrix.get().m32 > b->modelViewMatrix.get().m32;
  }
  else if (a->isOpaque()) {
    return true;
  }
  else if (b->isOpaque()){
    return false;
  }
  else { // neither is opaque
    if (a->modelViewMatrix.get().m32 == b->modelViewMatrix.get().m32){
      return a->siblingPosition() < b->siblingPosition();
    }
    return a->modelViewMatrix.get().m32 < b->modelViewMatrix.get().m32;
  }
}


void Node::drawChildren() {
  
  vector<Node*> toDraw;
  
  if (cullable()) { // 99% only set non cullable on skybox etc.
    for (int i = 0; i < children().size(); i++) {
      auto& child = children()[i];
      // FAST CULL
      if (child->modelViewMatrix.get().m32 <= .01 || !child->cullable()) {
        toDraw.push_back(child.get());
      }
    }
    if (toDraw.size() > 1) {
      sort(toDraw.begin(), toDraw.end(), nearestOpaqueThenFurthestTransparent);
    }
    for (auto child : toDraw) {
      child->draw();
    }
  }
  else {
    for (auto& child : children()) {
      child->draw();
    }
  }
  
}

void Node::draw() {
  if (hidden()) return;
  
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
}

void Node::restoreGLStates() const{
  if (ignoresLights()){
    if (GLState::activeShader()){
      if (GLState::activeShader()->hasUniformNamed(NKS_I1_NUM_LIGHTS)) {
        GLState::activeShader()->uniformNamed(NKS_I1_NUM_LIGHTS).bindU1(scene()->lights.size());
      }
    }
  }
}

void Node::setupViewMatrix() const {
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
    if (ignoresLights()){
      if (GLState::activeShader()->hasUniformNamed(NKS_I1_NUM_LIGHTS)) {
        GLState::activeShader()->uniformNamed(NKS_I1_NUM_LIGHTS).bindI1(0);
      }
    }
  }
}

void Node::drawWithHitShader() const {
  if (_userInteractionEnabled) {
    customDrawWithHitShader();
    for (auto& child : children()) {
      child->drawWithHitShader();
    }
  }
}

void Node::customDraw() {
  // OVERRIDE IN SUB CLASS
}

void Node::customDrawWithHitShader() const {
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


V3t Node::globalPosition() const{
  return globalTransform.get().translation;
}

bool Node::shouldCull() {
  return 0;
}

#pragma mark - MATH



#pragma mark - PROPERTIES

#pragma mark - MATRIX



#pragma mark - POSITION

V2t Node::screenPosition() {
  return globalPosition().xy + (scene()->size.get().xy * .5);
}

void Node::setGlobalPosition(V3t p){
  if(parent() == NULL) {
    position.set(p);
  } else {
    M16t global = parent()->globalTransform.get();
    M16Invert(&global);
    position.set(V3MultiplyM16WithTranslation(global, p));
  }
}

M16t Node::transformMatrixInNode(Node* n){
  
  if (parent() == n || !parent()) {
    return transform.get();
  }
  else {
    // recursive add
    return M16Multiply(parent()->transformMatrixInNode(n), transform.get());
  }
}

P2t Node::positionInNode(Node *node){
  return positionInNode3d(node).xy;
}

V3t Node::positionInNode3d(Node *node){
  return V3GetM16Translation(transformMatrixInNode(node));
}

#pragma mark - SCALE


//void Node::setTransparency(F1t transparency){
//  Drawable<V4>::setTransparency(transparency * parentAlpha);
//  setAlpha(transparency * parentAlpha);
//  Drawable<V4>::setAlpha(transparency * parentAlpha);
//}

//void Node::setAlpha(F1t alpha){
//  intAlpha = alpha;
//  _alpha = intAlpha * parentAlpha;
//
//  for (auto& n : children()){
//    n->recursiveAlpha(_alpha);
//  }
//}
//
//void Node::recursiveAlpha(F1t alpha){
//  parentAlpha = alpha;
//  _alpha = intAlpha * parentAlpha;
//
//  for (auto& n : children()){
//    n->recursiveAlpha(_alpha);
//  }
//}

#pragma mark - SHADER / FBO

void Node::chooseShader() {
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
    printf("error executing nil action \n");
    return true;
  }
  
  if (action->children.size()) { // GROUPS
    
    if (!action->queue.size()) {
      return action->completeWithTimeSinceLast(dt, node);
    }
    
    if (action->serial) {
      if (excecuteAction(action->queue.front().get(),node, dt)){
        action->removeAction(action->queue.front().get());
      }
      if (!action->queue.size()) {
        return action->completeWithTimeSinceLast(dt, node);
      }
      return false;
    }
    
    else { // parallel
      
      //printf("process group \n");
      
      bool active = false;
      
      vector<Action*> complete;
      
      for (auto ac : action->queue) {
        if (ac->repeats() == 0) active = true;
        if (excecuteAction(ac.get(), node, dt)){
          complete.push_back(ac.get());
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

void Node::runCompletionBlock(CompletionBlock block){
  block();
}

Action* Node::runAction(shared_ptr<Action> action){
  actions.push_back(action);
  
  action->parentNode = this;
  action->sharedReset();
  return action.get();
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

void Node::removeAction(Action* action){
  _::eraseSmartFromRaw(actions, action);
}

void Node::removeActionForKey(const string &key){
  shared_ptr<Action> action = actionForKey(key);
  if (action) {
    if (action->parentAction) action->parentAction->removeAction(action.get());
    else removeAction(action.get());
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
  for (auto& action : actions){
    action->stop();
  }
  actions.clear();
}



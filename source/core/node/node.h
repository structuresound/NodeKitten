#pragma once

#include "../generic/generic.h"

namespace Shader {
  class Program;
}

class Action;
class Scene;
class NKDrawDepthShader;
class NKBulletBody;
class FrameBuffer;

class Node;
class UXEvent;
class View;

class Node :
public Nodal<Node>,
public Transformable<Node, V3>,
public Drawable<Node, Color>,
public Sizeable<V3>,
public CountedClass<Node>
{

#pragma mark - POSITION / GEOMETRY

protected:
  std::vector<std::shared_ptr<UXEvent>> _touches;
  std::vector<std::shared_ptr<Action>> actions;

  std::function<void(std::shared_ptr<UXEvent>)> _eventBlock {nullptr};
  std::function<void(F1t)> _updateBlock {nullptr};

  mutable Scene* _scene = nullptr;

  V3t _orbit;

  F1t parentAlpha {1.0};

  bool _userInteractionEnabled {true};

#pragma mark - Privatize constructor
public:

  UB4t uidColor;

  // PUBLIC DATA MEMBERS
  F1t cameraDistance;
  std::shared_ptr<Shader::Program> shader;
  Reactive::Variable<M16> globalTransform;
  Reactive::Variable<M16> modelViewMatrix;
  Reactive::Pullable<M16> modelViewProjectionMatrix;

#pragma mark - NODE TREE

  std::string name;

#pragma mark - CONVENIENCE CONSTRUCTORS

  static std::shared_ptr<Node> node(){
    return std::make_shared<Node>();
  }

  static std::shared_ptr<Node> node(V3t size){
    return std::make_shared<Node>(Color(1), size);
  }

  static std::shared_ptr<Node> node(Color color, V3t size){
    return std::make_shared<Node>(color, size);
  }

#pragma mark - INIT
  Node(Color color = Color(0), V3t size = {1,1,1});
  Node(V3t size) : Node(Color(0), size){};
  Node(V2t size) : Node(Color(0), V3t {size.x, size.y, 1}){};
  virtual ~Node() {};

#pragma mark - SHADER PROPERTIES

  virtual void chooseShader();

#pragma mark - NODAL

  void beforeParentChanged() override {
    position.set(globalPosition());
  }

  void afterParentChanged() override {
    setGlobalPosition(position.get());
    if (_userInteractionEnabled && parent()) {
      static_cast<Node*>(parent())->setUserInteractionEnabled(true);
    }
  }

  void beforeRemove() override;

  void fadeInChild(std::shared_ptr<Node>& child, F1t seconds, CompletionBlock completion);
  void fadeOutChild(std::shared_ptr<Node>& child, F1t seconds, CompletionBlock completion);

#pragma mark - SCENE METHODS

  Scene* scene() const{
    if (_scene) return _scene;
    return _scene = getScene();
  }

  Scene* getScene() const{
    if (_scene) return _scene;
    if (parent()) return _scene = static_cast<Node*>(parent())->getScene();
    else return (Scene*)this;
  }

  void setScene(Scene* scene){
    _scene = scene;
  }

#pragma mark - PHYSICS

  //NKBulletBody *body;

#pragma mark - GEOMETRY METHODS

  virtual V3t globalPosition() const;
  void setGlobalPosition(V3t globalPosition);

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
  virtual void setupViewMatrix() const;
  virtual void customDraw();
  virtual void drawChildren();
  virtual void restoreGLStates() const;
  // ******** //
  // drawing for hit detection
  void drawWithHitShader() const;
  virtual void customDrawWithHitShader() const;

  void drawMultiPass(int numPasses);

  bool shouldCull();

  int numVisibleNodes();
  int numNodes();

#pragma mark - SHADER / FBO

  void loadShaderNamed(std::string& name);

#pragma mark - STATE MAINTENANCE

  virtual void pushStyle();

#pragma mark - MATRIX METHODS

  void setLocalTransform(M16t localTransform);
  M16t localTransform();
  void createMatrix();

#pragma mark - TRANSFORM
  void afterResize() override {
    transform.setDirty();
    afterTransform();
  }
  virtual void afterTransform() override {
    globalTransform.setDirty();
    modelViewMatrix.setDirty();
    modelViewProjectionMatrix.setDirty();
    for (auto& child : children()) child->afterTransform();
  }

  Q4t getGlobalOrientation() {
    return Q4GetM16Rotate(globalTransform.get());
  }

  V3t getOrientationEuler() {
    return V3FromQ4(Q4GetM16Rotate(globalTransform.get()));
  }

  void lookAtNode(Node* node){
    lookAtPoint(node->globalPosition());
  }

#pragma mark - GEODATA

  void setOrbit(V3t latitudeLongitudeRadius) {_orbit = latitudeLongitudeRadius;}
  V3t orbit() const {return _orbit;}

  V3t positionForOrbit() const{
    V3t p = V3RotatePoint(V3(0, 0, _orbit.radius), _orbit.longitude, V3(1, 0, 0));
    return V3RotatePoint(p, _orbit.latitude, V3(0, 1, 0));
  }

#pragma mark - TOUCH

  void setUserInteractionEnabled(bool enabled);
  bool userInteractionEnabled() {return _userInteractionEnabled;};

  void setUXEventBlock(std::function<void(std::shared_ptr<UXEvent>)> eventBlock);
  void setUpdateBlock(std::function<void(F1t)> updateBlock){_updateBlock = updateBlock;};

  virtual void handleUXEvent(std::shared_ptr<UXEvent> event);

  // UTIL
  void logCoords();
  void logPosition();
  void logMatrix(M16t M16);

#pragma mark - ACTIONS

  Action* runAction(std::shared_ptr<Action> action);
  void runAction(std::shared_ptr<Action> action, CompletionBlock completion);
  void runAction(std::shared_ptr<Action> action, const std::string& key);
  void runCompletionBlock(CompletionBlock block);

  U1t hasActions();

  std::shared_ptr<Action> actionForKey(const std::string& key);

  void removeAction(Action* action);
  void removeActionForKey(const std::string& key);
  void removeAllActions();

#pragma mark - NON-MEMBER FUNCTIONS

  static bool nearestOpaqueThenFurthestTransparent (const Node* a, const Node* b);

};

#pragma mark - Drawable
template<> inline
F1t Drawable<Node,V4>::alpha() const {
  if (self().parent()){
    return _alpha * self().parent()->alpha();
  }
  return _alpha;
}

template<> inline
V3 Transformable<Node,V3>::upVector() const {
  if (!self().parent()){
    return _upVector;
  }
  return V3MultiplyM16(self().parent()->globalTransform.get(), _upVector);
}

template<> inline
M16t Transformable<Node,V3>::getLookMatrix(V3t lookAtPosition){
  return M16MakeLookAt(self().globalPosition(), lookAtPosition, upVector());
}

bool excecuteAction(Action *action, Node* node, F1t dt);

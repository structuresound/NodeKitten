#ifndef __Action_h_
#define __Action_h_

#include "../types/types.h"

class Node;
class Action;

union _ActionData {
  struct {V3t position[2]; F1t scale[2];};
  struct {Q4t orientation[2];};
  struct {V4t color[2];};

  _ActionData(){}
} __attribute__((aligned(16)));

typedef union _ActionData ActionData;

typedef std::function<void(Action*, Node*, F1t)> ActionBlock;

typedef enum ActionTimingMode {
  ActionTimingLinear,
  ActionTimingEaseIn,
  ActionTimingEaseOut,
  ActionTimingEaseInEaseOut
} ActionTimingMode;

class Action : public std::enable_shared_from_this<Action>
{
  U1t _numFrames {0};
  U1t _nFrames {0};
  U1t _subDivide {0};
  U1t _currentSubdivision {0};
  I1t _repeats {0};
  I1t _initialRepeats {0};
  F1t _duration {0};


  ActionTimingMode _timingMode {ActionTimingLinear};

public:

  Action(F1t duration = 0, CompletionBlock completionBlock = []{});
  ~Action(){destroyedObjects++;}

  U1t frameCount {0};
  bool shouldInit {1};
  bool serial {false};

  // TIMING

  ActionBlock actionBlock = [](Action*, Node*, F1t){};
  CompletionBlock completionBlock {nullptr};

  std::shared_ptr<Action> setDuration(F1t duration);
  std::shared_ptr<Action> setSubdivide(U1t subdivide);

  std::shared_ptr<Action> repeat(U1t count);
  std::shared_ptr<Action> repeatForever();
  std::shared_ptr<Action> timingMode(ActionTimingMode mode);

  std::shared_ptr<Action> then(CompletionBlock&& block){
    completionBlock = block;
    return shared_from_this();
  }

  I1t repeats() {return _initialRepeats;};

  // NODE / HIERATCHY
  std::string key;
  Action *parentAction {nullptr};
  Node *parentNode {nullptr};

  std::vector<std::shared_ptr<Action>> children; // THESE STAY FOR REPEATS
  std::vector<std::shared_ptr<Action>> queue; // THESE COPY FROM CHILDREN ON EACH REPEAT

  void removeAction(Action* action);
  void sharedReset();
  bool completeOrRepeat();
  bool completeWithTimeSinceLast(F1t dt, Node *node);
  bool updateWithTimeSinceLast(F1t dt, Node *node);

  Node *target {nullptr};

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

  std::shared_ptr<Action> reversedAction;

  void runCompletion(CompletionBlock block);
  void stop();

  bool updateWithTimeSinceLast(F1t dt);


  // 3D Additions

  static std::shared_ptr<Action> rotateXByAngle(F1t radians ,F1t sec);
  static std::shared_ptr<Action> rotateYByAngle(F1t radians , F1t sec);

  static std::shared_ptr<Action> rotateByAngles(V3t angles , F1t sec);
  static std::shared_ptr<Action> rotateToAngles(V3t angles , F1t sec);

  static std::shared_ptr<Action> rotateAxis(V3t axis, F1t radians, F1t sec);
  static std::shared_ptr<Action> multiRotateAxis(V3t axis, F1t radians, F1t sec);

  static std::shared_ptr<Action> moveByX(F1t deltaX, F1t deltaY , F1t sec);
  static std::shared_ptr<Action> moveByX(F1t deltaX, F1t deltaY , F1t deltaZ, F1t sec);
  static std::shared_ptr<Action> moveBy(V3t delta, F1t sec);

  static std::shared_ptr<Action> moveToX(F1t x , F1t sec);
  static std::shared_ptr<Action> moveToY(F1t y , F1t sec);
  static std::shared_ptr<Action> moveToX(F1t x, F1t y , F1t sec);
  static std::shared_ptr<Action> moveTo(V2t location , F1t sec);
  static std::shared_ptr<Action> moveTo(V3t location , F1t sec);

  static std::shared_ptr<Action> moveToFollowNode(Node* target , F1t sec);
  static std::shared_ptr<Action> followNode(Node* target , F1t sec);

  static std::shared_ptr<Action> rotateZByAngle(F1t radians , F1t sec);

  static std::shared_ptr<Action> rotateToAngle(F1t radians , F1t sec);
  static std::shared_ptr<Action> rotateToAngle(F1t radians , F1t sec, bool shortestUnitArc);

  static std::shared_ptr<Action> resizeToWidth(F1t width, F1t height, F1t sec);
  static std::shared_ptr<Action> resize(V3t newSize , F1t sec);

  static std::shared_ptr<Action> scaleBy(F1t scale , F1t sec);
  static std::shared_ptr<Action> scaleXBy(F1t xScale , F1t yScale , F1t sec);
  static std::shared_ptr<Action> scaleTo(F1t scale , F1t sec);
  static std::shared_ptr<Action> scaleXTo(F1t xScale ,F1t yScale , F1t sec);
  static std::shared_ptr<Action> scaleXTo(F1t scale , F1t sec);
  static std::shared_ptr<Action> scaleYTo(F1t scale , F1t sec);

  static std::shared_ptr<Action> sequence(std::vector<std::shared_ptr<Action>> actions);
  static std::shared_ptr<Action> group(std::vector<std::shared_ptr<Action>> actions);

  static std::shared_ptr<Action> delay(F1t sec);
  static std::shared_ptr<Action> block(CompletionBlock&& completionBlock);

  static std::shared_ptr<Action> fadeByEnvelopeWithWaitTime(F1t waitTime, F1t inTime, F1t holdTime ,F1t outTime);

  static std::shared_ptr<Action> fadeInWithDuration(F1t sec);
  static std::shared_ptr<Action> fadeBlendTo(F1t alpha , F1t sec);
  static std::shared_ptr<Action> fadeColorTo(Color color , F1t sec);

  static std::shared_ptr<Action> fadeOutWithDuration(F1t sec);
  static std::shared_ptr<Action> fadeAlphaBy(F1t factor , F1t sec);
  static std::shared_ptr<Action> fadeAlphaTo(F1t alpha , F1t sec);
  static std::shared_ptr<Action> strobeAlpha(U1t onFrames, U1t offFrames , F1t sec);

  //
  //static shared_ptr<Action> setTexture(ofTexture *)texture;
  //static shared_ptr<Action> animateWithTextures(NSArray *)textures timePerFrameF1t sec);
  //static shared_ptr<Action> animateWithTextures(NSArray *)textures timePerFrameF1t sec) resize(BOOL)resize restore(BOOL)restore;
  //
  ///* name must be the name or path of a file of a platform supported audio file format. Use a LinearPCM format audio file with 8 or 16 bits per channel for best performance */
  //static shared_ptr<Action> playSoundFileNamed(string*)soundFile waitForCompletion(BOOL)wait;
  //
  //static shared_ptr<Action> colorizeWithColor(Color *)color colorBlendFactor(F1t colorBlendFactor , F1t sec);
  //static shared_ptr<Action> colorizeWithColorBlendFactor(F1t colorBlendFactor , F1t sec);
  //
  //static shared_ptr<Action> followPath(CGPathRef)path , F1t sec);
  //static shared_ptr<Action> followPath(CGPathRef)path asOffset(BOOL)offset orientToPath(BOOL)orient , F1t sec);
  //
  //static shared_ptr<Action> speedBy(F1t speed , F1t sec);
  //static shared_ptr<Action> speedTo(F1t speed , F1t sec);
  //
  //static shared_ptr<Action> waitForDurationF1t sec);
  //static shared_ptr<Action> waitForDurationF1t sec) withRange(F1t), Range;
  //
  //static shared_ptr<Action> removeFromParent;
  //
  //static shared_ptr<Action> performSelector(SEL)selector onTarget(id)target;
  //
  //static shared_ptr<Action> runBlock(dispatch_block_t)block;
  //static shared_ptr<Action> runBlock(dispatch_block_t)block queue(dispatch_queue_t)queue;
  //
  //static shared_ptr<Action> runAction(Action *)action onChildWithName(string*)name;
  //
  static std::shared_ptr<Action> custom(F1t sec, std::function<void(Action* action, Node* node, F1t completion)>);

  // SCROLL NODE

  static std::shared_ptr<Action> scrollToPoint(P2t point , F1t sec);
  static std::shared_ptr<Action> scrollToChild(Node* child , F1t sec);

  // LOOK

  static std::shared_ptr<Action> panTolookAtNode(Node* target, F1t sec);
  static std::shared_ptr<Action> snapLookToNode(Node* target, F1t sec);

  // ORBIT

  static std::shared_ptr<Action> enterOrbit(V3 orbit, F1t sec, Node* target = nullptr, V3t offset = V3(0));
  static std::shared_ptr<Action> maintainOrbit(V3 orbit, F1t sec, Node * target = nullptr, V3t offset = V3(0));

  static std::shared_ptr<Action> typeText(std::string text,F1t sec);

#pragma mark - DEBUG

  static unsigned int createdObjects;
  static unsigned int destroyedObjects;
  static unsigned int liveObjects(){return createdObjects - destroyedObjects;};


};

#endif

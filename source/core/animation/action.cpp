//
//  ofxBlockAnimationHandler.cpp
//  example-ofxTableView
//
//  Created by Chroma Developer on 2/11/14.
//
//

#include "action.h"
#include "node.h"
#include "label.h"

unsigned int Action::createdObjects {0};
unsigned int Action::destroyedObjects {0};

using namespace std;

Action::Action(F1t duration, CompletionBlock completionBlock) :
  completionBlock(completionBlock),
  _duration(duration)
{
  _numFrames = _duration * 60;
  createdObjects++;
}

#pragma mark - INIT / REMOVE / GROUP

void Action::stop(){
    for (auto& a : queue) {
        a->stop();
    }
    queue.clear();
    children.clear();

    _repeats = 0;
    completionBlock = nullptr;
    actionBlock = nullptr;
}

void Action::removeAction(Action* action){
    if (queue.size()) {
        _::eraseSmartFromRaw(queue, action);
    }
}

void Action::sharedReset() {
    frameCount = 0;
    _currentSubdivision = 0;

    if (children.size()) {
        queue.clear();
        queue = children;
        for (auto& c : queue) {
            c->_repeats = c->_initialRepeats;
            c->sharedReset();
        }
    }
}

void Action::runCompletion(CompletionBlock block){
    if (parentAction) {
        parentAction->runCompletion(block);
    }
    else if (parentNode) {
        parentNode->runCompletionBlock(block);
    }
}

bool Action::completeWithTimeSinceLast(F1t dt, Node *node){
  if (_repeats == 0) { // COMPLETE
    runCompletion(completionBlock);
    return 1;
  }
  else { // REPEAT
    if (_repeats > 0){
      _repeats -= 1;
    }
    sharedReset();
    return 0;
  }
}

#pragma mark - Grouping


shared_ptr<Action> Action::group(vector<shared_ptr<Action> > actions){

    auto newAction = make_shared<Action>(0);

    newAction->children = actions;

    for (auto& a : newAction->children) {
        a->parentAction = newAction.get();
    }

    return newAction;

}

shared_ptr<Action> Action::sequence(vector<shared_ptr<Action> > actions){

    auto newAction = Action::group(actions);
    newAction->serial = true;
    return newAction;

}

#pragma mark - TIMING

shared_ptr<Action> Action::delay(F1t sec){
    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [](Action *action, Node* node, F1t completion){
    };
    return newAction;
}

shared_ptr<Action> Action::block(CompletionBlock&& actionBlock){
  return Action::custom(0, [actionBlock](Action *a, Node *n, F1t p){
    actionBlock();
  });
}

shared_ptr<Action> Action::repeat(U1t count){
    _repeats = _initialRepeats = count;
    return shared_from_this();
}

shared_ptr<Action> Action::repeatForever(){
    _repeats = _initialRepeats = -1;
    return shared_from_this();
}

shared_ptr<Action> Action::timingMode(ActionTimingMode mode){
    _timingMode = mode;
    return shared_from_this();
}

shared_ptr<Action> Action::setDuration(F1t duration){
    _duration = duration;
    _numFrames = _duration * 60;
    setSubdivide(_subDivide);

    return shared_from_this();
}

shared_ptr<Action> Action::setSubdivide(U1t subdivide){
    _subDivide = subdivide;
    if (_subDivide) {
        _nFrames = (_numFrames / _subDivide);
        if (_nFrames == 0) {
            _nFrames = 1;
        }
    }
    return shared_from_this();
}

#pragma mark - MOVE BY

shared_ptr<Action> Action::moveByX(F1t deltaX, F1t deltaY, F1t sec){

    return Action::moveByX(deltaX, deltaY, 0, sec);

}

shared_ptr<Action> Action::moveByX(F1t deltaX, F1t deltaY, F1t deltaZ, F1t sec){

    return Action::moveBy(V3(deltaX, deltaY, deltaZ), sec);

}

shared_ptr<Action> Action::moveBy(V3t delta, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [delta](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->startPos = node->position.get();
            V3t p = action->startPos;
            action->endPos = V3(p.x + delta.x, p.y+ delta.y, p.z + delta.z);

            //NSLog(@"action end %f %f %f",action->endPos.x,action->endPos.y,action->endPos.z);
        }

        V3t np = getTween(action->startPos, action->endPos, completion );
        // NSLog(@"action dst %f %f %f, comp: %f",np.x,np.y,np.z, completion);
        node->position.set(np);

    };

    return newAction;
}

#pragma mark - MOVE TO

shared_ptr<Action> Action::moveTo(V3t location, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [location](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->startPos = node->position.get();
            action->endPos = V3(location.x, location.y, location.z);

        }

        node->position.set(getTween(action->startPos, action->endPos, completion));

    };

    return newAction;

}

shared_ptr<Action> Action::moveTo(V2t location, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [location](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->startPos = node->position.get();
            action->endPos = V3(location.x, location.y, node->position.get().z);

        }

        node->position.set(getTween(action->startPos, action->endPos, completion ));

    };

    return newAction;
}

shared_ptr<Action> Action::moveToX(F1t x, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [x](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->startPos = node->position.get();
            V3t p = node->position.get();
            action->endPos = V3(x, p.y, p.z);

        }

        node->position.set(getTween(action->startPos, action->endPos, completion));

    };

    return newAction;
}

shared_ptr<Action> Action::moveToY(F1t y, F1t sec){
    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [y](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->startPos = node->position.get();
            V3t p = node->position.get();
            action->endPos = V3(p.x, y, p.z);


        }

        node->position.set(getTween(action->startPos, action->endPos, completion ));


    };

    return newAction;

}

shared_ptr<Action> Action::moveToFollowNode(Node *target, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [target](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->startPos = node->position.get();
            //action->endPos = V3(location.x, location.y, location.z);

        }

        node->position.set(getTween(action->startPos, target->globalPosition(), completion ));

    };

    return newAction;

}

shared_ptr<Action> Action::followNode(Node *target, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [target](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {

        }

        node->position.set(target->globalPosition());
        //node->setPosition(getTween(action->startPos, target->globalPosition(), completion ));
    };

    return newAction;

}

#pragma mark - ROTATE

shared_ptr<Action> Action::rotateByAngles(V3t angles, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [angles](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            Q4t start = node->orientation.get();
            //NKLogV4("start orientation", start);
            action->startOrientation = start;
            Q4t xRot = Q4FromAngleAndV3(angles.x, V3(1,0,0));
            Q4t yRot = Q4FromAngleAndV3(angles.y, V3(0,1,0));
            Q4t zRot = Q4FromAngleAndV3(angles.z, V3(0,0,1));
            action->endOrientation = QuatMul(xRot, QuatMul(yRot, QuatMul(zRot, start)));
        }

        node->orientation.set(QuatSlerp(action->startOrientation, action->endOrientation,completion));

    };

    return newAction;

}

shared_ptr<Action> Action::rotateToAngles(V3t angles, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [angles](Action *action, Node* node, F1t completion){

        if (action->shouldInit == 1) {
            Q4t start = node->orientation.get();
            action->startOrientation = start;
            Q4t xRot = Q4FromAngleAndV3(angles.x, V3(1,0,0));
            Q4t yRot = Q4FromAngleAndV3(angles.y, V3(0,1,0));
            Q4t zRot = Q4FromAngleAndV3(angles.z, V3(0,0,1));
            action->endOrientation = QuatMul(xRot, QuatMul(yRot, QuatMul(zRot, start)));
            action->shouldInit = 0;
        }

        node->orientation.set(QuatSlerp(action->startOrientation, action->endOrientation,completion));
    };

    return newAction;

}

shared_ptr<Action> Action::rotateXByAngle(F1t radians, F1t sec){

    return Action::rotateAxis(V3(1, 0, 0), radians, sec);

}

shared_ptr<Action> Action::rotateYByAngle(F1t radians, F1t sec){
    return Action::rotateAxis(V3(0, 1, 0), radians, sec);
}


shared_ptr<Action> Action::rotateZByAngle(F1t radians, F1t sec){
    return Action::rotateAxis(V3(0, 0, 1), radians, sec);
}

shared_ptr<Action> Action::rotateAxis(V3t axis, F1t radians, F1t sec) {
    if (radians > 180 || radians < -180) {
        return multiRotateAxis(axis, radians, sec);
    }
    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [axis, radians](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {
            Q4t start = node->orientation.get();
            action->startOrientation = start;
            Q4t rot = Q4FromAngleAndV3(radians, axis);
            action->endOrientation = QuatMul(start, rot);
        }
        node->orientation.set(QuatSlerp(action->startOrientation, action->endOrientation,completion));
    };
    return newAction;
}

shared_ptr<Action> Action::multiRotateAxis(V3t axis, F1t radians, F1t sec) {
    int numActions = fabs(radians / 180);
    double portion = radians / (double)numActions;
    vector<shared_ptr<Action>> sequence;
    for (int i = 0; i < numActions; i++){
        sequence.push_back(Action::rotateAxis(axis, portion, sec/(double)numActions));
    }
    return Action::sequence(sequence);
}

shared_ptr<Action> Action::rotateToAngle(F1t radians, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [radians](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {


            Q4t start = node->orientation.get();
            action->startOrientation = start;
            Q4t zRot = Q4FromAngleAndV3(radians, V3(0,0,1));
            action->endOrientation = zRot;

        }

        node->orientation.set(QuatSlerp(action->startOrientation, action->endOrientation,completion));

    };

    return newAction;

}

#pragma mark - GL LOOK AT

shared_ptr<Action> Action::enterOrbit(V3t orbit, F1t sec, Node* target, V3t offset){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [target, orbit, offset](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->startPos = node->position.get();
            node->setOrbit(V3(orbit));
        }

      if (target) {
        node->position.set(getTween(action->startPos, node->positionForOrbit() + target->globalPosition() + offset, completion));
      }
      else {
        node->position.set(getTween(action->startPos, node->positionForOrbit() + offset, completion));
      }

    };

    return newAction;

}

shared_ptr<Action> Action::maintainOrbit(V3 delta, F1t sec, Node * target, V3t offset){

  auto newAction = make_shared<Action>(sec);

  newAction->actionBlock = [target, delta, offset](Action *action, Node* node, F1t completion){
    if (action->frameCount == 0) {
      action->startPos = node->orbit();
      action->endPos = action->startPos + delta;
      node->setOrbit(action->endPos);
    }

    node->setOrbit(getTween(action->startPos, action->endPos, completion));
    if (target) {
      node->position.set(target->globalPosition() + node->positionForOrbit() + offset);
    }
    else {
      node->position.set(node->positionForOrbit() + offset);
    }
  };

  return newAction;
}

shared_ptr<Action> Action::panTolookAtNode(Node *target, F1t sec){

    auto newAction = make_shared<Action>(sec);
    newAction->target = target;

    newAction->actionBlock = [target](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {

        }

        action->startOrientation = Q4GetM16Rotate(node->globalTransform.get());
        action->endOrientation = Q4GetM16Rotate(node->getLookMatrix(target->globalPosition()));

        node->orientation.set(QuatSlerp(action->startOrientation, action->endOrientation,completion));

    };

    return newAction;

}

shared_ptr<Action> Action::snapLookToNode(Node *target, F1t sec){

    auto newAction = make_shared<Action>(sec);
    newAction->target = target;

    newAction->actionBlock = [target](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {

        }

        node->lookAtNode(target);

    };

    return newAction;

}


#pragma mark - SCALE

shared_ptr<Action> Action::scaleBy(F1t scale, F1t sec){
    auto newAction = make_shared<Action>(sec);
    newAction->actionBlock = [scale](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {

            action->startPos = node->scale.get();
            action->endPos = V3(action->startPos.x * scale, action->startPos.y * scale, action->startPos.z * scale);
        }
        node->scale.set(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

shared_ptr<Action> Action::scaleXBy(F1t xScale, F1t yScale, F1t sec){
    auto newAction = make_shared<Action>(sec);
    newAction->actionBlock = [xScale,yScale](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {

            action->startPos = node->scale.get();
            action->endPos = V3(action->startPos.x * xScale, action->startPos.y * yScale, action->startPos.z);
        }
        node->scale.set(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

shared_ptr<Action> Action::scaleTo(F1t scale, F1t sec){
    auto newAction = make_shared<Action>(sec);
    newAction->actionBlock = [scale](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {

            action->startPos = node->scale.get();
            action->endPos = V3(scale);
        }
        node->scale.set(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

shared_ptr<Action> Action::scaleXTo(F1t xScale, F1t yScale, F1t sec){
    auto newAction = make_shared<Action>(sec);
    newAction->actionBlock = [xScale,yScale](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {

            action->startPos = node->scale.get();
            action->endPos = V3(xScale, yScale, action->startPos.z);
        }
        node->scale.set(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;

}

shared_ptr<Action> Action::scaleXTo(F1t xScale, F1t sec){
    auto newAction = make_shared<Action>(sec);
    newAction->actionBlock = [xScale](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {

            action->startPos = node->scale.get();
            action->endPos = V3(xScale, action->startPos.y, action->startPos.z);
        }
        node->scale.set(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

shared_ptr<Action> Action::scaleYTo(F1t scale, F1t sec){
    auto newAction = make_shared<Action>(sec);
    newAction->actionBlock = [scale](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {

            action->startPos = node->scale.get();
            action->endPos = V3(action->startPos.x, scale, action->startPos.z);
        }
        node->scale.set(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

#pragma mark - SIZE

shared_ptr<Action> Action::resizeToWidth(F1t width, F1t height, F1t sec){
    auto newAction = make_shared<Action>(sec);
    newAction->actionBlock = [width,height](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {
            action->startPos = node->size.get();
            action->endPos = V3(width, height, node->size.get().z);
        }
        node->size.set(getTween(action->startPos, action->endPos, completion));
    };

    return newAction;
}

shared_ptr<Action> Action::resize(V3t newSize, F1t sec){
    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [newSize](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {
            action->startPos = node->size.get();
            action->endPos = newSize;
        }
        node->size.set(getTween(action->startPos, action->endPos, completion));
    };

    return newAction;
}

shared_ptr<Action> Action::fadeByEnvelopeWithWaitTime(F1t waitTime, F1t inTime, F1t holdTime, F1t outTime){

    auto wait = Action::custom(waitTime, [](Action *action, Node* node, F1t completion){
        if (action->frameCount == 0) {
            node->setAlpha(0);
        }
    });

    auto fadeIn = Action::fadeAlphaTo(1., inTime);
    auto hold = Action::delay(holdTime);
    auto fadeOut = Action::fadeAlphaTo(0., outTime);

    return Action::sequence({wait,fadeIn,hold,fadeOut});
}

#pragma mark - COLOR

shared_ptr<Action> Action::fadeBlendTo(F1t alpha, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [alpha](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {

            action->startFloat = node->colorBlendFactor();
            action->endFloat = alpha;
        }

        node->setColorBlendFactor(weightedAverage(action->startFloat, action->endFloat, completion));

    };

    return newAction;

}

shared_ptr<Action> Action::fadeColorTo(Color color, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [color](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->data.color[0] = node->color();
            action->data.color[1] = color;
        }

        node->setColor(getTween(action->data.color[1], action->data.color[0], completion));
    };

    return newAction;

}

#pragma mark - ALPHA

shared_ptr<Action> Action::strobeAlpha(U1t onFrames, U1t offFrames, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [onFrames, offFrames](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            action->flag = true;
            action->startFloat = 0;
        }

        if (action->flag) {
            if (action->startFloat > onFrames) {
                node->setAlpha(0);
                action->startFloat = 0; // frameCounter;
                action->flag = false;
            }
            else {
                node->setAlpha(1.0);
                //NSLog(@"on %d %d", onFrames, action->frameCount);
            }


        }
        else {
            if (action->startFloat > offFrames) {
                node->setAlpha(1.0);
                action->startFloat = 0; // frameCounter;
                action->flag = true;
            }
            else {
                node->setAlpha(0.0);
            }
        }

        action->startFloat++;

        if (completion == 1) {
            node->setAlpha(1.0);
        }

    };

    return newAction;
}

shared_ptr<Action> Action::fadeInWithDuration(F1t sec){
    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {
            node->setAlpha(0);
            action->startFloat = 0;
            action->endFloat = 1.;
        }

        node->setAlpha(weightedAverage(action->startFloat, action->endFloat, completion));

    };

    return newAction;
}

shared_ptr<Action> Action::fadeOutWithDuration(F1t sec){
    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {

            action->startFloat = node->alpha();
            action->endFloat = 0;
        }

        node->setAlpha(weightedAverage(action->startFloat, action->endFloat, completion));

        if (completion == 1) {
            node->removeFromParent();
        }
    };

    return newAction;
}

shared_ptr<Action> Action::fadeAlphaTo(F1t alpha, F1t sec){

    auto newAction = make_shared<Action>(sec);

    newAction->actionBlock = [alpha](Action *action, Node* node, F1t completion){

        if (action->frameCount == 0) {

            action->startFloat = node->alpha();
            action->endFloat = alpha;
        }

        node->setAlpha(weightedAverage(action->startFloat, action->endFloat, completion));

    };

    return newAction;


}

#pragma mark - SCROLL ACTIONS

shared_ptr<Action> Action::scrollToPoint(P2t point, F1t sec){

    auto newAction = make_shared<Action>(sec);

#warning DO scrollNode
    //    newAction->actionBlock = [point](Action *action, Node* node, F1t completion){
    //
    //        if (action->frameCount == 0) {
    //            P2t p2 = [(NKScrollNode*)node scrollPosition);
    //            action->startPos = V3(p2.x,p2.y,0);
    //            action->endPos = V3(point.x, point.y, 0);
    //
    //        }
    //
    //        V3t p = getTween(action->startPos, action->endPos, completion );
    //        [(NKScrollNode*)node setScrollPosition:V2(p.x,p.y));
    //
    //    };

    return newAction;

}

shared_ptr<Action> Action::scrollToChild(Node *child, F1t sec){

    auto newAction = make_shared<Action>(sec);

#warning DO scrollNode
    //    newAction->actionBlock = [](Action *action, Node* node, F1t completion){
    //
    //        if (action->frameCount == 0) {
    //            P2t p2 = [(NKScrollNode*)node scrollPosition);
    //            action->startPos = V3(p2.x,p2.y,0);
    //            action->endPos = V3(child.position.x, child.position.y, 0);
    //
    //        }
    //
    //        V3t p = getTween(action->startPos, action->endPos, completion );
    //        [(NKScrollNode*)node setScrollPosition:V2(p.x,p.y));
    //
    //    };

    return newAction;

}

#pragma mark - TEXT / LABEL

shared_ptr<Action> Action::typeText(string text, F1t sec){

    auto newAction = make_shared<Action>(sec);

    int framesPerLetter = (sec / (double)text.size()) * 60;

    newAction->actionBlock = [text, framesPerLetter](Action *action, Node* node, F1t completion){
        int numletters = action->frameCount / framesPerLetter;
        if (numletters > text.size()) return;
        string substr = text.substr(0,numletters);

        ((Label*)node)->setText(substr);
    };

    return newAction;

}

#pragma mark - CUSTOM ACTIONS

shared_ptr<Action> Action::custom(F1t sec, std::function<void(Action* action, Node* node, F1t completion)> block){
    auto newAction = make_shared<Action>(sec);
    newAction->actionBlock = block;
    return newAction;
}

#pragma mark - UPDATE

bool Action::updateWithTimeSinceLast(F1t dt, Node *node){
  if (frameCount == _numFrames) {
    actionBlock(this, node, 1);
    return 1;
  }
  else {

    F1t completion = (frameCount+1) / ((float)_numFrames+1);

    switch (_timingMode) {
      case ActionTimingEaseOut:
        completion = sin((completion *  M_PI_2));
        break;

      case ActionTimingEaseIn:
        completion = 1.0 - sin((completion *  M_PI_2) + M_PI_2);
        break;

      case ActionTimingEaseInEaseOut:
        completion = 1.0 - (sin((completion *  M_PI) + M_PI_2) * .5 + .5);
        break;
      default:
        break;
    }

    if (_subDivide == 0) {
        actionBlock(this, node, completion);
    }

    else if (frameCount % _nFrames == 0){
      if (_currentSubdivision < _subDivide) {
        actionBlock(this, node, completion);
        _currentSubdivision++;
      }
    }

    frameCount++;
    return 0;
  }
}

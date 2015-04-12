//
//  ofxBlockAnimationHandler.cpp
//  example-ofxTableView
//
//  Created by Chroma Developer on 2/11/14.
//
//

#include "NKAction.h"
#include "NKNode.h"


NKAction::NKAction(F1t duration_) {
    setDuration(duration_);
    _repeats = 0;
}

#pragma mark - INIT / REMOVE / GROUP

void NKAction::stop(){
    
    for (auto a : children) {
        a->stop();
    }
    
    children.clear();
    actions.clear();
    _repeats = 0;
    completionBlock = nullptr;
    actionBlock = nullptr;
    
}

void NKAction::removeAction(shared_ptr<NKAction> action){
    if (actions.size()) {
        erase_object(actions, action);
        
        //        if (!actions.size()) {
        //            completeOrRepeat();
        //        }
    }
}

void NKAction::sharedReset() {
    
    frameCount = 0;
    _currentSubdivision = 0;
    
    if (children.size()) {
        //printf("reset %lu actions \n", children.size());
        actions.clear();
        actions = children;
        // printf("actions %lu, children %lu", actions.size(), children.size());
        
        for (auto c : actions) {
            c->_repeats = c->_initialRepeats;
            c->sharedReset();
        }
    }
    
}

void NKAction::runCompletion(CompletionBlock block){
    if (parentAction) {
        parentAction->runCompletion(block);
    }
    else if (parentNode) {
        parentNode->runCompletionBlockForAction(shared_from_this());
    }
}

bool NKAction::completeOrRepeat() {
    if (_repeats == 0) {
        if (completionBlock) {
            runCompletion(completionBlock);
        }
        
        return 1;
    }
    
    else {
        if (_repeats > 0){
            _repeats -= 1;
        }
        sharedReset();
        return 0;
    }
}

bool NKAction::completeWithTimeSinceLast(F1t dt, NKNode *node){
    return completeOrRepeat();
}

#pragma mark - Grouping


shared_ptr<NKAction> NKAction::group(vector<shared_ptr<NKAction> > actions){
    
    auto newAction = make_shared<NKAction>(0);
    
    newAction->children = actions;
    
    for (auto a : newAction->children) {
        a->parentAction = newAction.get();
    }
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::sequence(vector<shared_ptr<NKAction> > actions){
    
    auto newAction = NKAction::group(actions);
    newAction->serial = true;
    return newAction;
    
}

shared_ptr<NKAction> NKAction::delay(F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            
        }
    };
    return newAction;
}

shared_ptr<NKAction> NKAction::repeat(U1t count){
    _repeats = _initialRepeats = count;
    return shared_from_this();
}

shared_ptr<NKAction> NKAction::repeatForever(){
    _repeats = _initialRepeats = -1;
    return shared_from_this();
}

shared_ptr<NKAction> NKAction::timingMode(NKActionTimingMode mode){
    _timingMode = mode;
    return shared_from_this();
}

#pragma mark - MOVE BY

shared_ptr<NKAction> NKAction::moveByX(F1t deltaX, F1t deltaY, F1t sec){
    
    return NKAction::moveByX(deltaX, deltaY, 0, sec);
    
}

shared_ptr<NKAction> NKAction::moveByX(F1t deltaX, F1t deltaY, F1t deltaZ, F1t sec){
    
    return NKAction::moveBy(V3Make(deltaX, deltaY, deltaZ), sec);
    
}

shared_ptr<NKAction> NKAction::moveBy(V3t delta, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [delta](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->startPos = node->position();
            V3t p = action->startPos;
            action->endPos = V3Make(p.x + delta.x, p.y+ delta.y, p.z + delta.z);
            
            //NSLog(@"action end %f %f %f",action->endPos.x,action->endPos.y,action->endPos.z);
        }
        
        V3t np = getTween(action->startPos, action->endPos, completion );
        // NSLog(@"action dst %f %f %f, comp: %f",np.x,np.y,np.z, completion);
        node->setPosition(np);
        
    };
    
    return newAction;
}

#pragma mark - MOVE TO

shared_ptr<NKAction> NKAction::moveTo(V3t location, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [location](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->startPos = node->position();
            action->endPos = V3Make(location.x, location.y, location.z);
            
        }
        
        node->setPosition(getTween(action->startPos, action->endPos, completion));
        
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::moveTo(V2t location, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [location](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->startPos = node->position();
            action->endPos = V3Make(location.x, location.y, node->position().z);
            
        }
        
        node->setPosition(getTween(action->startPos, action->endPos, completion ));
        
    };
    
    return newAction;
}

shared_ptr<NKAction> NKAction::moveToX(F1t x, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [x](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->startPos = node->position();
            V3t p = node->position();
            action->endPos = V3Make(x, p.y, p.z);
            
        }
        
        node->setPosition(getTween(action->startPos, action->endPos, completion));
        
    };
    
    return newAction;
}

shared_ptr<NKAction> NKAction::moveToY(F1t y, F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [y](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->startPos = node->position();
            V3t p = node->position();
            action->endPos = V3Make(p.x, y, p.z);
            
            
        }
        
        node->setPosition(getTween(action->startPos, action->endPos, completion ));
        
        
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::moveToFollowNode(NKNode *target, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [target](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->startPos = node->position();
            //action->endPos = V3Make(location.x, location.y, location.z);
            
        }
        
        node->setPosition(getTween(action->startPos, target->globalPosition(), completion ));
        
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::followNode(NKNode *target, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [target](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
        }
        
        node->setPosition(target->globalPosition());
        //node->setPosition(getTween(action->startPos, target->globalPosition(), completion ));
    };
    
    return newAction;
    
}

#pragma mark - ROTATE

shared_ptr<NKAction> NKAction::rotateByAngles(V3t angles, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [angles](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            Q4t start = node->orientation();
            //NKLogV4("start orientation", start);
            action->startOrientation = start;
            Q4t xRot = Q4FromAngleAndV3(angles.x, V3Make(1,0,0));
            Q4t yRot = Q4FromAngleAndV3(angles.y, V3Make(0,1,0));
            Q4t zRot = Q4FromAngleAndV3(angles.z, V3Make(0,0,1));
            action->endOrientation = QuatMul(xRot, QuatMul(yRot, QuatMul(zRot, start)));
        }
        
        node->setOrientation(QuatSlerp(action->startOrientation, action->endOrientation,completion));
        
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::rotateToAngles(V3t angles, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [angles](NKAction *action, NKNode* node, F1t completion){
        
        if (action->shouldInit == 1) {
            Q4t start = node->orientation();
            action->startOrientation = start;
            Q4t xRot = Q4FromAngleAndV3(angles.x, V3Make(1,0,0));
            Q4t yRot = Q4FromAngleAndV3(angles.y, V3Make(0,1,0));
            Q4t zRot = Q4FromAngleAndV3(angles.z, V3Make(0,0,1));
            action->endOrientation = QuatMul(xRot, QuatMul(yRot, QuatMul(zRot, start)));
            action->shouldInit = 0;
        }
        
        node->setOrientation(QuatSlerp(action->startOrientation, action->endOrientation,completion));
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::rotateXByAngle(F1t radians, F1t sec){
    
    return NKAction::rotateAxis(V3Make(1, 0, 0), radians, sec);
    
}

shared_ptr<NKAction> NKAction::rotateYByAngle(F1t radians, F1t sec){
    
    return NKAction::rotateAxis(V3Make(0, 1, 0), radians, sec);
    
}


shared_ptr<NKAction> NKAction::rotateZByAngle(F1t radians, F1t sec){
    
    return NKAction::rotateAxis(V3Make(0, 0, 1), radians, sec);
    
}

shared_ptr<NKAction> NKAction::rotateAxis(V3t axis, F1t radians, F1t sec) {
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [axis, radians](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            Q4t start = node->orientation();
            //NKLogV4("start orientation", start);
            action->startOrientation = start;
            Q4t rot = Q4FromAngleAndV3(radians, axis);
            action->endOrientation = QuatMul(start, rot);
        }
        
        node->setOrientation(QuatSlerp(action->startOrientation, action->endOrientation,completion));
        
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::rotateToAngle(F1t radians, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [radians](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            
            Q4t start = node->orientation();
            action->startOrientation = start;
            Q4t zRot = Q4FromAngleAndV3(radians, V3Make(0,0,1));
            action->endOrientation = zRot;
            
        }
        
        node->setOrientation(QuatSlerp(action->startOrientation, action->endOrientation,completion));
        
    };
    
    return newAction;
    
}

#pragma mark - GL LOOK AT

shared_ptr<NKAction> NKAction::enterOrbitForNode(NKNode *target, F1t longitude, F1t latitude, F1t radius, F1t sec, V3t offset){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [target, longitude, latitude, radius, offset](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            action->startPos = node->position();
            node->setOrbit(V3Make(longitude,latitude,radius));
        }
        
        node->setPosition(getTween(action->startPos, V3Add(V3Add(node->currentOrbit(),target->globalPosition()),offset), completion));
        
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::enterOrbitAtLongitude(F1t longitude, F1t latitude, F1t radius, F1t sec, V3t offset){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [longitude, latitude, radius, offset](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->startPos = node->position();
            node->setOrbit(V3Make(longitude,latitude,radius));
            action->endPos = V3Add(node->currentOrbit(),offset);
        }
        
        node->setPosition(getTween(action->startPos, action->endPos, completion));
        
    };
    
    return newAction;
}

shared_ptr<NKAction> NKAction::maintainOrbitDelta(F1t deltaLongitude, F1t deltaLatitude, F1t deltaRadius, F1t sec, V3t offset){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [deltaLongitude, deltaLatitude, deltaRadius, offset](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            action->startPos = V3Make(node->longitude, node->latitude, node->radius);
            action->endPos = V3Add(action->startPos, V3Make(deltaLongitude, deltaLatitude, deltaRadius));
            
            node->setOrbit(action->endPos);
        }
        
        node->setOrbit(getTween(action->startPos, action->endPos, completion));
        node->setPosition(V3Add(offset, node->currentOrbit()));
    };
    
    return newAction;
}

shared_ptr<NKAction> NKAction::maintainOrbitForNode(NKNode *target, F1t deltaLongitude, F1t deltaLatitude, F1t deltaRadius, F1t sec, V3t offset){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [target, deltaLongitude, deltaLatitude, deltaRadius, offset](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            
            action->startPos = V3Make(node->longitude, node->latitude, node->radius);
            action->endPos = V3Add(action->startPos, V3Make(deltaLongitude, deltaLatitude, deltaRadius));
            node->setOrbit(action->endPos);
        }
        node->setOrbit(getTween(action->startPos, action->endPos, completion));
        node->setPosition(V3Add(V3Add(target->globalPosition(), node->currentOrbit()),offset));
    };
    
    return newAction;
}

shared_ptr<NKAction> NKAction::panTolookAtNode(NKNode *target, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    newAction->target = target;
    
    newAction->actionBlock = [target](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
        }
        
        action->startOrientation = Q4GetM16Rotate(node->globalTransform());
        action->endOrientation = Q4GetM16Rotate(node->getLookMatrix(target->globalPosition()));
        
        node->setOrientation(QuatSlerp(action->startOrientation, action->endOrientation,completion));
        
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::snapLookToNode(NKNode *target, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    newAction->target = target;
    
    newAction->actionBlock = [target](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            
        }
        
        node->lookAtNode(target);
        
    };
    
    return newAction;
    
}


#pragma mark - SCALE

shared_ptr<NKAction> NKAction::scaleBy(F1t scale, F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    newAction->actionBlock = [scale](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            
            action->startPos = node->scale();
            action->endPos = V3Make(action->startPos.x * scale, action->startPos.y * scale, action->startPos.z * scale);
        }
        node->setScale(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

shared_ptr<NKAction> NKAction::scaleXBy(F1t xScale, F1t yScale, F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    newAction->actionBlock = [xScale,yScale](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            
            action->startPos = node->scale();
            action->endPos = V3Make(action->startPos.x * xScale, action->startPos.y * yScale, action->startPos.z);
        }
        node->setScale(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

shared_ptr<NKAction> NKAction::scaleTo(F1t scale, F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    newAction->actionBlock = [scale](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            
            action->startPos = node->scale();
            action->endPos = V3MakeF(scale);
        }
        node->setScale(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

shared_ptr<NKAction> NKAction::scaleXTo(F1t xScale, F1t yScale, F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    newAction->actionBlock = [xScale,yScale](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            
            action->startPos = node->scale();
            action->endPos = V3Make(xScale, yScale, action->startPos.z);
        }
        node->setScale(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
    
}

shared_ptr<NKAction> NKAction::scaleXTo(F1t xScale, F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    newAction->actionBlock = [xScale](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            
            action->startPos = node->scale();
            action->endPos = V3Make(xScale, action->startPos.y, action->startPos.z);
        }
        node->setScale(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

shared_ptr<NKAction> NKAction::scaleYTo(F1t scale, F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    newAction->actionBlock = [scale](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            
            action->startPos = node->scale();
            action->endPos = V3Make(action->startPos.x, scale, action->startPos.z);
        }
        node->setScale(getTween(action->startPos, action->endPos, completion));
    };
    return newAction;
}

#pragma mark - SIZE

shared_ptr<NKAction> NKAction::resizeToWidth(F1t width, F1t height, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [width,height](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            action->startPos = node->size();
            action->endPos = V3Make(width, height, node->size().z);
        }
        
        
        node->setSize(getTween(action->startPos, action->endPos, completion));
    };
    
    return newAction;
    
    
}

shared_ptr<NKAction> NKAction::resize(V3t newSize, F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [newSize](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            action->startPos = node->size();
            action->endPos = newSize;
        }
        
        
        node->setSize(getTween(action->startPos, action->endPos, completion));
    };
    
    return newAction;
}

shared_ptr<NKAction> NKAction::fadeByEnvelopeWithWaitTime(F1t waitTime, F1t inTime, F1t holdTime, F1t outTime){
    
    auto wait = NKAction::customActionWithDuration(waitTime, [](NKAction *action, NKNode* node, F1t completion){
        if (action->frameCount == 0) {
            node->setAlpha(0);
        }
    });
    
    auto fadeIn = NKAction::fadeAlphaTo(1., inTime);
    auto hold = NKAction::delay(holdTime);
    auto fadeOut = NKAction::fadeAlphaTo(0., outTime);
    
    return NKAction::sequence({wait,fadeIn,hold,fadeOut});
}

#pragma mark - COLOR

shared_ptr<NKAction> NKAction::fadeBlendTo(F1t alpha, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [alpha](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            action->startFloat = node->colorBlendFactor();
            action->endFloat = alpha;
        }
        
        node->setColorBlendFactor(weightedAverage(action->startFloat, action->endFloat, completion));
        
    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::fadeColorTo(NKColor color, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [color](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->data.startColor = node->color().data;
            action->data.endColor = color.data;
        }
        
        node->color().data = (getTween(action->data.startColor, action->data.endColor, completion));
        
    };
    
    return newAction;
    
}

#pragma mark - ALPHA

shared_ptr<NKAction> NKAction::strobeAlpha(U1t onFrames, U1t offFrames, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [onFrames, offFrames](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            action->flag = true;
            action->startFloat = 0;
        }
        
        if (action->flag) {
            if (action->startFloat > onFrames) {
                node->color().setAlpha(0);
                action->startFloat = 0; // frameCounter;
                action->flag = false;
            }
            else {
                
                node->color().setAlpha(1.0);
                //NSLog(@"on %d %d", onFrames, action->frameCount);
            }
            
            
        }
        else {
            if (action->startFloat > offFrames) {
                node->color().setAlpha(1.0);
                action->startFloat = 0; // frameCounter;
                action->flag = true;
            }
            else {
                node->color().setAlpha(0.0);
            }
        }
        
        action->startFloat++;
        
        if (completion == 1) {
            node->color().setAlpha(1.0);
        }
        
    };
    
    return newAction;
}

shared_ptr<NKAction> NKAction::fadeInWithDuration(F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            node->setAlpha(0);
            action->startFloat = 0;
            action->endFloat = 1.;
        }
        
        node->setAlpha(weightedAverage(action->startFloat, action->endFloat, completion));
        
    };
    
    return newAction;
}

shared_ptr<NKAction> NKAction::fadeOutWithDuration(F1t sec){
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [](NKAction *action, NKNode* node, F1t completion){
        
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

shared_ptr<NKAction> NKAction::fadeAlphaTo(F1t alpha, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = [alpha](NKAction *action, NKNode* node, F1t completion){
        
        if (action->frameCount == 0) {
            
            action->startFloat = node->alpha();
            action->endFloat = alpha;
        }
        
        node->setAlpha(weightedAverage(action->startFloat, action->endFloat, completion));
        
    };
    
    return newAction;
    
    
}

#pragma mark - SCROLL ACTIONS

shared_ptr<NKAction> NKAction::scrollToPoint(P2t point, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
#warning DO scrollNode
    //    newAction->actionBlock = [point](NKAction *action, NKNode* node, F1t completion){
    //
    //        if (action->frameCount == 0) {
    //            P2t p2 = [(NKScrollNode*)node scrollPosition);
    //            action->startPos = V3Make(p2.x,p2.y,0);
    //            action->endPos = V3Make(point.x, point.y, 0);
    //
    //        }
    //
    //        V3t p = getTween(action->startPos, action->endPos, completion );
    //        [(NKScrollNode*)node setScrollPosition:V2Make(p.x,p.y));
    //
    //    };
    
    return newAction;
    
}

shared_ptr<NKAction> NKAction::scrollToChild(NKNode *child, F1t sec){
    
    auto newAction = make_shared<NKAction>(sec);
    
#warning DO scrollNode
    //    newAction->actionBlock = [](NKAction *action, NKNode* node, F1t completion){
    //
    //        if (action->frameCount == 0) {
    //            P2t p2 = [(NKScrollNode*)node scrollPosition);
    //            action->startPos = V3Make(p2.x,p2.y,0);
    //            action->endPos = V3Make(child.position.x, child.position.y, 0);
    //
    //        }
    //
    //        V3t p = getTween(action->startPos, action->endPos, completion );
    //        [(NKScrollNode*)node setScrollPosition:V2Make(p.x,p.y));
    //
    //    };
    
    return newAction;
    
}

#pragma mark - CUSTOM ACTIONS

shared_ptr<NKAction> NKAction::customActionWithDuration(F1t sec, ActionBlock block){
    
    auto newAction = make_shared<NKAction>(sec);
    
    newAction->actionBlock = block;
    
    return newAction;
}

#pragma mark - UPDATE

void NKAction::setDuration(F1t duration){
    _duration = duration;
    _numFrames = _duration * 60;
    setSubdivide(_subDivide);
}

void NKAction::setSubdivide(U1t subdivide){
    _subDivide = subdivide;
    if (_subDivide) {
        _nFrames = (_numFrames / _subDivide);
        if (_nFrames == 0) {
            _nFrames = 1;
        }
        //NSLog(@"total frames %d / %d = every %d frames", numFrames, _subDivide, nFrames);
    }
}

bool NKAction::updateWithTimeSinceLast(F1t dt, NKNode *node){
    
    F1t completion = (frameCount+1) / ((float)_numFrames+1);
    
    switch (_timingMode) {
        case NKActionTimingEaseOut:
            completion = sin((completion *  M_PI_2));
            break;
            
        case NKActionTimingEaseIn:
            completion = 1.0 - sin((completion *  M_PI_2) + M_PI_2);
            break;
            
        case NKActionTimingEaseInEaseOut:
            completion = 1.0 - (sin((completion *  M_PI) + M_PI_2) * .5 + .5);
            break;
        default:
            break;
    }
    
    if (frameCount == _numFrames) {
        if (actionBlock){
            actionBlock(this, node, 1.0);
        }
        return 1;
    }
    else {
        if (_subDivide == 0) {
            if (actionBlock){
                actionBlock(this, node, completion);
            }
        }
        
        else if (frameCount % _nFrames == 0){
            if (_currentSubdivision < _subDivide) {
                if (actionBlock){
                    actionBlock(this, node, completion);
                }
                _currentSubdivision++;
            }
        }
        
        frameCount++;
        
        return 0;
    }
}

//NKAnimationHandler* NKAction::handler(){
//    if (parentAction){
//        return parentAction->handler();
//    }
//    return _handler;
//}
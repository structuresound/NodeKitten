//
//  Text3DExample.cpp
//  Pods
//
//  Created by Leif Shackelford on 5/2/15.
//
//

#include "Text3DExample.h"

void Text3DExample::moveToView() {
    
    setSize(_size.xy);

    
    auto cameraTrack = Node::node();
    addChild(cameraTrack);
    cameraTrack->addChild(camera);

    camera->setPosition({.25,.5,1});
    
    auto sky = Mesh::nodeWithPrimitive(NKPrimitiveSphere, Texture::textureWithImageFile("imperial_skybox_by_vest.jpg"), Color(1.0), V3(100.0));
    sky->setCullFace(GLCullFaceBack);
    addChild(sky);
    
    sky->cullable = false;
    
    auto ground = Mesh::nodeWithPrimitive(NKPrimitivePlane, nullptr, Color(1.0, .5), V3(50, 50, 1));
    ground->setPosition(V3t{0,0,0});
    ground->setOrientationEuler(V3(-90, 0, 0));
    ground->setCullFace(GLCullFaceNone);
    addChild(ground);
    
    ground->cullable = false;
    
    auto label = make_shared<Label>("Vera.ttf", 0.2);
    label->setText("NodeKitten X");
    addChild(label);
    
    label->setCullFace(GLCullFaceBack);
    label->setTextAlignment(NKTextAlignmentLeft);
    
    label->cullable = false;
    label->runAction(Action::sequence({
        Action::customActionWithDuration(4.0, [label](Action *action, Node *node, F1t completion){
            if (action->frameCount == 0){
                string str = String::randomQuote();
                nkLog("text: %s", str);
                node->runAction(Action::typeText(str, .1 * str.length()));
                action->setDuration(.1 * str.length() + 1.0);
                //label->setColor(Color::randomColor());
            }
        })
    })->repeatForever());
    
    cameraTrack->runAction(Action::customActionWithDuration(1.0, [this, label, cameraTrack](Action *action, Node* node, F1t completion){
        V6t bbox = label->boundingBox();
        V3t end = V3(bbox.x.v[1], bbox.y.v[1], bbox.z.v[1]);
        
        cameraTrack->removeActionForKey("chase");
        cameraTrack->runAction(Action::moveTo(end, 2.0), "chase");
        
        camera->target->removeActionForKey("chase");
        camera->target->runAction(Action::moveTo(end, 1.0), "chase");
    })->repeatForever()->setSubdivide(10));
                      
    auto enterOrbit1 = Action::enterOrbitAtLongitude(rand() % 119, rand() % 119 , .5, 1.0, V3(0, .2, 1.5));
    
    enterOrbit1->completionBlock = [this]{
        camera->runAction(Action::customActionWithDuration(1.0, [](Action *action, Node* node, F1t completion){
            if (node->hasActions() == 1){
                float time = rand()%100 * .1 + 2;
                
                int dir = rand()%100 > 50 ? 15 : -15;
                
                node->runAction(Action::maintainOrbitDelta(dir*time,rand() % 119, 0, time, V3(0, .2, 1.5))->timingMode(ActionTimingEaseInEaseOut));
            }
        })->repeatForever());
    };
    
    camera->runAction(enterOrbit1);
    
}
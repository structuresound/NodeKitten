//
//  AnimationExample.cpp
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/26/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#include "AnimationExample.h"

shared_ptr<Texture> moonTex;
shared_ptr<Texture> skyTex;
shared_ptr<Texture> skyTex2;

void AnimationExample::moveToView() {
  
  nkLog("size of node: %lu", sizeof(Node));
  
  addChild(Node::node());
  
  auto forgroundLayer = Node::node();
  addChild(forgroundLayer); // for explicit depth ordering
  
  auto backgroundLayer = Node::node();
  addChild(backgroundLayer); // for explicit depth ordering
  
  nkLog("move to view: example scene \n");
  
  if (!moonTex) {
    moonTex = Texture::textureWithImageFile("moon.png");
    skyTex = Texture::textureWithImageFile("outdoors.jpg");
    skyTex2 = Texture::textureWithImageFile("imperial_skybox_by_vest.jpg");
  }
  
#if NK_LOG_GL
  char* glExtensions = (char*)glGetString(GL_EXTENSIONS);
  printf("GL EXTENSIONS : %s \n", glExtensions);
#endif
  
  setBackgroundColor(0);
  
  auto cameraTrack = Node::node();
  addChild(cameraTrack);
  
  auto cameraArm = Node::node();
  cameraTrack->addChild(cameraArm);
  
  cameraArm->addChild(camera->base);
  camera->base->position.set(V3(1.,1.,2.0));
  
  auto sky = Mesh::nodeWithPrimitive(NKPrimitiveSphere, skyTex, Color("lightblue"), V3(10.0));
  sky->shader = Program::shaderNamed("sky", NKS_COLOR_MODE_UNIFORM, 1, 0);
  sky->setCullFace(GLCullFaceBack);
  
  backgroundLayer->addChild(sky);
  
  auto ground = Mesh::nodeWithPrimitive(NKPrimitivePlane, nullptr, Color(.5,.5,.7,.7), V3(20, 20, 1));
  
  ground->position.set(V3t{0,-.5,0});
  ground->setOrientationEuler(V3(-90, 0, 0));
  
  sky->addChild(ground);
  
#if !NK_USE_GLES
  ground->shader = Program::shaderNamed("water", {ShaderModule::lightModule(true, 0), ShaderModule::swirl(.5, V2t{20,20}), ShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, 0)});
#endif
  
  auto sun = Mesh::nodeWithPrimitive(NKPrimitiveSphere, nullptr, Color("lightYellow"), V3(.25));
  sun->position.set(V3(2, 0, 0));
  
  forgroundLayer->addChild(sun);
  
  auto sphereLayer = MeshBatcher::nodeWithPrimitive(NKPrimitiveSphere, moonTex, Color(1), V3(1));
  auto cubeLayer = MeshBatcher::nodeWithPrimitive(NKPrimitiveCube, nullptr, Color(1.0), V3(1));
  
  forgroundLayer->addChild(sphereLayer);
  forgroundLayer->addChild(cubeLayer);
  
  int sph = 256;
  
  auto light = Light::light(this);
#if NK_USE_GLES
  sph = 50;
#else
  auto light2 = Light::light(this);
  auto light3 = Light::light(this);
#endif
  for (int i = 0; i < sph; i++) {
    auto cube = Mesh::nodeWithPrimitive(NKPrimitiveSphere, moonTex, Color(1.0), V3(rand() % 50 * .00025 + .002));
    sphereLayer->addChild(cube);
    
    auto prepare = Action::enterOrbit(V3(rand() % 230, rand() % 219, .1), 1.0);
    
    auto dance = Action::group({
      Action::sequence({
        Action::delay(2),
        Action::sequence({
          Action::delay(.5),
          Action::moveTo(V3(0), 1.0)->timingMode(ActionTimingEaseIn),
        }),
        Action::delay(1.5),
        Action::custom(1.0, [](Action *action, Node* node, F1t progress){
          if (action->frameCount == 0) {
            node->runAction(Action::enterOrbit(V3(rand() % 23, rand() % 121, rand() % 50 * .11), .6)->timingMode(ActionTimingEaseOut));
          }
        }),
        Action::delay(.5),
        Action::custom(1.0, [](Action *action, Node* node, F1t progress){
          if (action->frameCount == 0) {
            node->runAction(Action::maintainOrbit(V3(rand() % 303, rand() % 301, 0), 1.0)->timingMode(ActionTimingEaseInEaseOut));
          }})
      }),
      Action::sequence({
        Action::delay(.01),
        Action::rotateByAngles(V3(270,0, rand() %30 + 20), rand() % 2 + .1)->timingMode(ActionTimingEaseOut),
        Action::delay(.07),
        Action::rotateByAngles(V3(0,230,0), .3)->timingMode(ActionTimingEaseOut),
        Action::custom(1.0, [](Action *action, Node* node, F1t progress){
          if (action->frameCount == 0) {
            node->runAction(Action::resize(V3(rand() % 20 * .01 + .001),.3)->timingMode(ActionTimingEaseInEaseOut));
          }
        })
      })->repeatForever()
    })->repeat(2);
    
    dance->then([cube,cubeLayer]{
      auto cbounce = Action::custom(2.0,
                                    [](Action *a, Node* n, F1t c)
                                    {
                                      if (n->hasActions() == 1) {
                                        float rate = rand()%20 * .1 + .1;
                                        auto bounce = Action::sequence({
                                          Action::moveToY(-1, rate)->timingMode(ActionTimingEaseIn),
                                          Action::moveToY(n->position.get().y, rate)->timingMode(ActionTimingEaseOut)
                                        });
                                        n->runAction(bounce);
                                        n->runAction(Action::strobeAlpha(rand()%4, rand()%10, rate));
                                      }
                                    });
      if (random()%100 > 20){
        auto resize = Action::resize(V3(.03), 4.)->timingMode(ActionTimingEaseIn);
        cube->runAction(resize, [cube, cbounce]{
          cube->runAction(cbounce->repeatForever());
        });
      }
      else {
        auto clone = Mesh::nodeWithPrimitive(NKPrimitiveCube, nullptr, Color(1.0,.8), V3(.3));
        clone->setOrientationEuler(V3(45, 0, 0));
        clone->position.set(cube->position.get());
        cubeLayer->addChild(clone);
        clone->size.set(cube->size.get()*1.1);
        clone->runAction(Action::group({
          Action::rotateXByAngle(45, 20.0),
          Action::rotateYByAngle(90, 1.0)->repeatForever(),
          Action::fadeColorTo(Color(random()%100*.01, random()%100*.01, random()%100*.01, .6),1.0)
        }));
      }
    });
    
    prepare->then([cube, cubeLayer, dance]
                  {
                    cube->runAction(dance);
                  });
    
    cube->runAction(prepare);
  }
  
  camera->base->runAction(Action::moveTo(V3(0), 10))->then
  ([this, sky, cubeLayer, sphereLayer,light](){
    auto spin = Action::custom(10.0, [](Action *action, Node* node, F1t completion)
                               {
                                 action->startFloat *= 1.02;
                                 node->setOrientationEuler(V3(0,action->startFloat,0));
                               });
    spin->startFloat = 1.0;
    sky->runAction(spin);
    
    sky->runAction(Action::fadeAlphaTo(0, 10.0))->then
    ([this, sky, cubeLayer, sphereLayer]{
      sky->setTexture(skyTex2);
      sky->runAction(Action::sequence({
        Action::delay(20.0),
        Action::fadeAlphaTo(1.0, 10.0)
      }))->then
      ([this, sky, cubeLayer, sphereLayer](){
        cubeLayer->runAction(Action::moveToY(30, 55)->timingMode(ActionTimingEaseIn));
        camera->base->removeAllActions();
        camera->base->runAction(Action::moveToY(30, 60));
        
        sphereLayer->runAction(Action::delay(40),[sphereLayer]{
          sphereLayer->runAction(Action::moveToY(30, 10));
        });
        
        auto fadeAndRestart = Action::sequence({
          Action::delay(10),
          Action::custom(40,
                         [this, sky](Action *a, Node* n, F1t c)
                         {
                           if (a->frameCount % 3 == 0) {
                             sky->setAlpha(rand()%100*.01 * (1.0-c) + (1.0-c)*.5);
                           }
                         }),
          Action::delay(5)
        })->then(
                 [this](){
                   Dispatch::uiQueue.push_back([this]{
                     unload();
                   });
                 });
        sky->runAction(fadeAndRestart);
      });
    });
    
    auto enterOrbit2 = Action::enterOrbit(V3(rand() % 130, rand() % 119, 1.5), 1.0);
    
    enterOrbit2->completionBlock = [light]{
      
      auto glow = Action::custom(10.0, [](Action *action, Node* node, F1t completion)
                                 {
                                   ((Light*)node)->properties.ambient = .15 - completion*.1;
                                 });
      light->runAction(glow);
      light->runAction(Action::resize(V3(.01), 10.));
    };
    
    light->runAction(enterOrbit2);
  });
  
  
  
  auto enterOrbit1 = Action::enterOrbit(V3(rand() % 119, 0 , 3), 1.0, nullptr, V3(1, 0, 0));
  
  enterOrbit1->completionBlock = [cameraTrack, cameraArm]{
    cameraTrack->runAction(Action::custom(1.0, [cameraArm](Action *action, Node* node, F1t completion){
      if (node->hasActions() == 1){
        float time = rand()%100 * .1 + 2;
        int dir = rand()%100 > 50 ? 15 : -15;
        node->runAction(Action::maintainOrbit(V3(dir*time,0, 0), time, nullptr, V3(1, 0, 0))->timingMode(ActionTimingEaseInEaseOut));
      }
    })->repeatForever());
  };
  
  cameraTrack->runAction(enterOrbit1);
  //
  auto enterOrbit = Action::enterOrbit(V3(rand() % 130, rand() % 119, .1), 1.0);
  enterOrbit->completionBlock = [this]{
    camera->target->runAction(Action::custom(1.0, [](Action *action, Node* node, F1t completion){
      if (node->hasActions() == 1){
        if (rand()%100 > 90) {
          node->runAction(Action::maintainOrbit(V3(rand() % 303, rand() % 301, 0), rand()%4*.1 + 1));
        }
        if (rand()%100 == 5) {
          node->runAction(Action::maintainOrbit(V3(rand() % 303, rand() % 301, 0), rand()%20*.1 + 1));
        }
        
      }
    })->repeatForever());
  };
  
  camera->target->runAction(enterOrbit);
  //
  light->setColor(Color(.8,.8,1.0,1.0));
  
  light->properties.linearAttenuation = .1;
  light->properties.ambient = .3;
  
  light->size.set(V3(.2));
  light->position.set(V3(0, 10, 0));
  
  backgroundLayer->addChild(light);
  
#if !NK_USE_GLES
  light2->setColor(Color(1.0, 0, 0, 1));
  
  light2->properties.linearAttenuation = .2;
  light2->properties.ambient = .2;
  
  light2->size.set(V3(.2));
  light2->position.set(V3(0, 5, 0));
  
  light3->setColor(Color(0, 0, 1.0, 1));
  light3->properties.linearAttenuation = .3;
  light3->properties.color = V3(0, 0, 1.0);
  light3->properties.ambient = .2;
  
  light3->size.set(V3(.2));
  light3->position.set(V3(5, 0, 0));
  
  backgroundLayer->addChild(light2);
  backgroundLayer->addChild(light3);
#endif
  
  drawLights = true;
}

void AnimationExample::handleUXEvent(shared_ptr<UXEvent> event){
}
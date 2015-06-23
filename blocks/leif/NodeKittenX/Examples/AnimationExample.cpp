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
    
    auto forground = Node::node();
    auto background = Node::node();
    
    addChild(forground);
    addChild(background); // for explicit depth ordering
    
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

    backgroundColor = Color(0);

    auto cameraTrack = Node::node();
    auto cameraArm = Node::node();
    addChild(cameraTrack);
    cameraTrack->addChild(cameraArm);
    cameraArm->addChild(camera);

    camera->setPosition(V3(1.,1.,2.0));

    auto sky = Mesh::nodeWithPrimitive(NKPrimitiveSphere, skyTex, Color("lightblue"), V3(10.0));
    sky->shader = Shader::shaderNamed("sky", NKS_COLOR_MODE_UNIFORM, 1, 0);
    sky->setCullFace(GLCullFaceBack);

    background->addChild(sky);
    
    auto ground = Mesh::nodeWithPrimitive(NKPrimitivePlane, nullptr, Color(.5,.5,.7,.7), V3(20, 20, 1));

    ground->setPosition(V3t{0,-.5,0});
    ground->setOrientationEuler(V3(-90, 0, 0));
    
    sky->addChild(ground);

#if !NK_USE_GLES
    ground->shader = Shader::shaderNamed("water", {ShaderModule::lightModule(true, 0), ShaderModule::swirl(.5, V2t{20,20}), ShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, 0)});
#endif

    auto sun = Mesh::nodeWithPrimitive(NKPrimitiveSphere, nullptr, Color("lightYellow"), V3(.25));
    sun->setPosition(V3(2, 0, 0));
    forground->addChild(sun);

    auto sphereLayer = MeshBatcher::nodeWithPrimitive(NKPrimitiveSphere, moonTex, Color(1), V3(1));
    //auto sphereLayer = Node::node();
    
    auto cubeLayer = MeshBatcher::nodeWithPrimitive(NKPrimitiveCube, nullptr, Color(1.0), V3(1));
    //auto cubeLayer = Node::node();
    
    forground->addChild(sphereLayer);
    forground->addChild(cubeLayer);
    
    //cubeLayer->setBlendMode(GLBlendModeAdd);
    
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

        auto start =  Action::enterOrbitAtLongitude(rand() % 230, rand() % 219, .1, 1.0);

        start->completionBlock = [cube, sphereLayer, cubeLayer, sky, light]
        {
            auto pan = Action::group({
                                               Action::sequence({
                                                                          Action::delay(2),
                                                                          Action::sequence({
                                                                                                     Action::delay(.5),
                                                                                                     Action::moveTo(V3(0), 1.0)->timingMode(ActionTimingEaseIn),
                                                                                             }),
                                                                          Action::delay(.5),
                                                                          Action::customActionWithDuration(1.0, [](Action *action, Node* node, F1t completion){
                                                                              if (action->frameCount == 0) {
                                                                                  node->runAction(Action::enterOrbitAtLongitude(rand() % 23, rand() % 121, rand() % 50 * .11, .6)->timingMode(ActionTimingEaseOut));
                                                                              }
                                                                          }),
                                                                          Action::delay(.5),
                                                                          Action::customActionWithDuration(1.0, [](Action *action, Node* node, F1t completion){
                                                                              if (action->frameCount == 0) {
                                                                                  node->runAction(Action::maintainOrbitDelta(rand() % 303, rand() % 301, 0, 1.0)->timingMode(ActionTimingEaseInEaseOut));
                                                                              }})
                                                                  }),
                                               Action::sequence({
                                                                          Action::delay(.01),
                                                                          Action::rotateByAngles(V3(270,0, rand() %30 + 20), rand() % 2 + .1)->timingMode(ActionTimingEaseOut),
                                                                          Action::delay(.07),
                                                                          Action::rotateByAngles(V3(0,230,0), .3)->timingMode(ActionTimingEaseOut),
                                                                          Action::customActionWithDuration(1.0, [](Action *action, Node* node, F1t completion){
                                                                              if (action->frameCount == 0) {
                                                                                  node->runAction(Action::resize(V3(rand() % 20 * .01 + .001),.3)->timingMode(ActionTimingEaseInEaseOut));
                                                                              }
                                                                          })
                                                                  })->repeatForever()
                                       })->repeat(2);

            pan->completionBlock = [cube, sphereLayer, cubeLayer, sky, light]{
                if (random()%100 > 20){
                    cube->runAction(Action::resize(V3(.03), 4.)->timingMode(ActionTimingEaseIn), [sphereLayer, cube, sky]{
                        auto cbounce = Action::customActionWithDuration(2.0, [](Action *action, Node* node, F1t completion)
                        {
                            if (node->hasActions() == 1) {
                                float rate = rand()%20 * .1 + .1;

                                node->runAction(Action::sequence(
                                        {
                                                Action::moveToY(-1, rate)->timingMode(ActionTimingEaseIn),
                                                Action::moveToY(node->position().y, rate)->timingMode(ActionTimingEaseOut)
                                        }
                                ));

                                node->runAction(Action::strobeAlpha(rand()%4, rand()%10, rate));
                            }
                        });
                        cube->runAction(cbounce->repeatForever());

                    });
                }
                else {
                    auto clone = Mesh::nodeWithPrimitive(NKPrimitiveCube, nullptr, Color(1.0,.6), V3(.3));
                    clone->setOrientationEuler(V3(45, 0, 0));
                    clone->runAction(Action::followNode(cube.get(), 1.0)->repeatForever());
                    cubeLayer->addChild(clone);
                    clone->setSize(cube->size()*1.1);
                    clone->runAction(Action::rotateYByAngle(90, 1.0)->repeatForever());
                    cube->runAction(Action::fadeColorTo(Color(random()%100*.01, random()%100*.01, random()%100*.01, .6),1.0));
                }


            };

            cube->runAction(pan);
        };

        cube->runAction(start);

        sphereLayer->addChild(cube);
    }

    camera->runAction(Action::moveTo(V3(0), 10), [sky, light, sphereLayer, cubeLayer, this](){

        auto spin = Action::customActionWithDuration(10.0, [](Action *action, Node* node, F1t completion)
        {
            action->startFloat *= 1.02;
            node->setOrientationEuler(V3(0,action->startFloat,0));
        });
        spin->startFloat = 1.0;

        sky->runAction(spin);

        sky->runAction(Action::fadeAlphaTo(0, 10.0), [sphereLayer, sky, cubeLayer, this]{

            //cubeEmitter->setTexture(nullptr);
            sky->setTexture(skyTex2);

            sky->runAction(Action::sequence({Action::delay(20.0),
                                               Action::fadeAlphaTo(1.0, 10.0)}
            ), [sphereLayer,cubeLayer,sky, this]()
                           {
                               cubeLayer->runAction(Action::moveToY(30, 55)->timingMode(ActionTimingEaseIn));
                               camera->removeAllActions();
                               camera->runAction(Action::moveToY(30, 60));
                               sphereLayer->runAction(Action::delay(40),[sphereLayer]{
                                   sphereLayer->runAction(Action::moveToY(30, 10));
                               });
                               sky->runAction(Action::delay(10), [sphereLayer,sky,this]()
                               {
                                   sky->runAction(Action::customActionWithDuration(40,[sky,this](Action *action, Node* node, F1t completion)
                                   {
                                       if (action->frameCount % 3 == 0) {
                                           sky->setAlpha(rand()%100*.01 * (1.0-completion) + (1.0-completion)*.5);
                                       }
                                   }
                                   ), [this]()
                                                  {
                                                      this->runAction(Action::delay(5),[this](){
                                                          unload();
                                                      }
                                                      );});
                                   //cubeEmitter->setTexture(skyTex);
                               });
                           });
        });

        auto enterOrbit2 = Action::enterOrbitAtLongitude(rand() % 130, rand() % 119, 1.5, 1.0);

        enterOrbit2->completionBlock = [light]{

            auto glow = Action::customActionWithDuration(10.0, [](Action *action, Node* node, F1t completion)
            {
                ((Light*)node)->properties.ambient = .15 - completion*.1;
            });
            light->runAction(glow);
            light->runAction(Action::resize(V3(.01), 10.));
        };

        light->runAction(enterOrbit2);
    });



    auto enterOrbit1 = Action::enterOrbitAtLongitude(rand() % 119, 0 , 3, 1.0, V3(1, 0, 0));

    enterOrbit1->completionBlock = [cameraTrack, cameraArm]{
        cameraTrack->runAction(Action::customActionWithDuration(1.0, [cameraArm](Action *action, Node* node, F1t completion){
            if (node->hasActions() == 1){
                float time = rand()%100 * .1 + 2;

                int dir = rand()%100 > 50 ? 15 : -15;

                node->runAction(Action::maintainOrbitDelta(dir*time,0, 0, time, V3(1, 0, 0))->timingMode(ActionTimingEaseInEaseOut));


            }
        })->repeatForever());
    };

    cameraTrack->runAction(enterOrbit1);
//
    auto enterOrbit = Action::enterOrbitAtLongitude(rand() % 130, rand() % 119, .1, 1.0);
    enterOrbit->completionBlock = [this]{
        camera->target->runAction(Action::customActionWithDuration(1.0, [](Action *action, Node* node, F1t completion){
            if (node->hasActions() == 1){
                if (rand()%100 > 90) {
                    node->runAction(Action::maintainOrbitDelta(rand() % 303, rand() % 301, 0, rand()%4*.1 + 1));
                }
                if (rand()%100 == 5) {
                    node->runAction(Action::maintainOrbitDelta(rand() % 303, rand() % 301, 0, rand()%20*.1 + 1));
                }

            }
        })->repeatForever());
    };

    camera->target->runAction(enterOrbit);
//
    light->setColor(Color(.8,.8,1.0,1.0));
    
    light->properties.linearAttenuation = .1;
    light->properties.ambient = .3;

    light->setSize(V3(.2));
    light->setPosition(V3(0, 10, 0));

    background->addChild(light);
    
#if !NK_USE_GLES
    light2->setColor(Color(1.0, 0, 0, 1));
    
    light2->properties.linearAttenuation = .2;
    light2->properties.ambient = .2;
    
    light2->setSize(V3(.2));
    light2->setPosition(V3(0, 5, 0));
    
    light3->setColor(Color(0, 0, 1.0, 1));
    light3->properties.linearAttenuation = .3;
    light3->properties.color = V3(0, 0, 1.0);
    light3->properties.ambient = .2;
    
    light3->setSize(V3(.2));
    light3->setPosition(V3(5, 0, 0));
    
    background->addChild(light2);
    background->addChild(light3);
#endif

    drawLights = true;
}

void AnimationExample::handleUXEvent(shared_ptr<UXEvent> event){
}
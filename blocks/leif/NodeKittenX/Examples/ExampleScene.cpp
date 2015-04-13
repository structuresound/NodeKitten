//
//  ExampleScene.cpp
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/26/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#include "ExampleScene.h"

shared_ptr<NKTexture> moonTex;
shared_ptr<NKTexture> skyTex;
shared_ptr<NKTexture> skyTex2;

int onView = 0;

void ExampleScene::moveToView() {

    if (onView) {
        // moved to view twice?
        assert(0);
    }
    onView = 1;
    
    nkLog("move to view: example scene \n");
    
    if (!moonTex) {
        moonTex = NKTexture::textureWithImageFile("moon.png");
        skyTex = NKTexture::textureWithImageFile("outdoors.jpg");
        skyTex2 = NKTexture::textureWithImageFile("imperial_skybox_by_vest.jpg");
    }

#if NK_LOG_GL
    char* glExtensions = (char*)glGetString(GL_EXTENSIONS);
    printf("GL EXTENSIONS : %s \n", glExtensions);
#endif
    //
    //    return;

#if TARGET_EMSCRIPTEN
    auto cameraTrack = NKNode::node();
    addChild(cameraTrack);
    cameraTrack->addChild(camera);
    //camera->setPosition(V3Make(1.,1.,2.0));

    auto sky = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveSphere, skyTex, NKColor(1.0), V3MakeF(10.0));
    //sky->setShader(NKShaderProgram::shaderNamed("sky", NKS_COLOR_MODE_UNIFORM, 1, 0));
    sky->setCullFace(NKCullFaceBack);
    addChild(sky);

//    auto axes = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveAxes, nullptr, NKColor(1.0, 0,0,1.0), V3MakeF(1));
//
//    axes->setShader(NKShaderProgram::shaderNamed("vertexColor",NKS_COLOR_MODE_VERTEX,0,0,0));
//
//    //axes->runAction(NKAction::rotateZByAngle(20, 1.0)->repeatForever());
//    //axes->drawBoundingBox = true;
//
//    addChild(axes);

//    auto cubeShader = NKShaderProgram::shaderNamed("cube",NKS_COLOR_MODE_UNIFORM,1,0,0);
//
//    for (int i = 0; i < 100; i++) {
//        auto cube = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveCube, moonTex, NKColor(1.0,.6), V3MakeF(rand() % 50 * .00025 + .002));
//
//        cube->setShader(cubeShader);
//
//        auto start =  NKAction::enterOrbitAtLongitude(rand() % 230, rand() % 219, .1, 1.0);
//
//        start->completionBlock = [cube]{
//            cube->runAction(NKAction::group({
//                NKAction::sequence({
//                    NKAction::delay(2),
//                    NKAction::sequence({
//                        NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
//                            if (action->frameCount == 0) {
//                                node->runAction(NKAction::maintainOrbitDelta(rand() % 303, rand() % 301, 0, 1.0)->timingMode(NKActionTimingEaseInEaseOut));
//                            }}),
//                        NKAction::delay(.5),
//                        NKAction::moveTo(V3MakeF(0), 1.0)->timingMode(NKActionTimingEaseIn),
//                    }),
//                    NKAction::delay(.5),
//                    NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
//                        if (action->frameCount == 0) {
//                            node->runAction(NKAction::enterOrbitAtLongitude(rand() % 23, rand() % 121, rand() % 50 * .11, .6)->timingMode(NKActionTimingEaseOut));
//                        }
//                    })
//                }),
//                NKAction::sequence({
//                    NKAction::delay(.01),
//                    NKAction::rotateByAngles(V3Make(270,0, rand() %30 + 20), rand() % 2 + .1)->timingMode(NKActionTimingEaseOut),
//                    NKAction::delay(.07),
//                    NKAction::rotateByAngles(V3Make(0,230,0), .3)->timingMode(NKActionTimingEaseOut),
//                    NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
//                        if (action->frameCount == 0) {
//                            node->runAction(NKAction::resize(V3MakeF(rand() % 20 * .01 + .001),.3)->timingMode(NKActionTimingEaseInEaseOut));
//                        }
//                    })
//                })->repeatForever()
//            })->repeatForever());
//        };
//
//        cube->runAction(start);
//
//        addChild(cube);
//    }


    //    //  NKTexture::textureWithImageFile("outdoors.png")
    //    auto sky = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveSphere,nullptr, NKColor(1.0,.5,.5,1.0), V3MakeF(10.0));
    //    //sky->setShader(NKShaderProgram::shaderNamed("sky", NKS_COLOR_MODE_UNIFORM, 1, 0));
    //    sky->setCullFace(NKCullFaceBack);
    //    addChild(sky);

//    auto enterOrbit = NKAction::enterOrbitAtLongitude(rand() % 130, rand() % 119, 3, 1.0);
//
//    enterOrbit->completionBlock = [this]{
//        camera->runAction(NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
//            if (node->hasActions() == 1){
//                node->runAction(NKAction::maintainOrbitDelta(rand() % 303, rand() % 301, 0, rand()%5 + 5.0)->timingMode(NKActionTimingEaseInEaseOut));
//            }
//        })->repeatForever());
//    };

    //    camera->runAction(NKAction::sequence({
    //    }
    //                      ));
    //    cameraTrack->runAction(enterOrbit);

    //    auto light = NKLightNode::lightNode();
    //
    //    light->setSize(V3MakeF(.05));
    //    auto enterOrbit2 = NKAction::enterOrbitAtLongitude(rand() % 130, rand() % 119, .5, .01);
    //    enterOrbit2->completionBlock = [light]{
    //        light->runAction(NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
    //            if (node->hasActions() == 1){
    //                node->runAction(NKAction::maintainOrbitDelta(rand() % 303, rand() % 301, 0, rand()%5 + 1.0)->timingMode(NKActionTimingEaseInEaseOut));
    //            }
    //        })->repeatForever());
    //    };
    //
    //    light->runAction(enterOrbit2);
    //    addChild(light);
    //
    //    drawLights = true;

#else

    backgroundColor = NKColor(0);

    auto cameraTrack = NKNode::node();
    auto cameraArm = NKNode::node();
    addChild(cameraTrack);
    cameraTrack->addChild(cameraArm);
    cameraArm->addChild(camera);

    camera->setPosition(V3Make(1.,1.,2.0));

    auto sky = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveSphere, skyTex, NKColor("lightblue"), V3MakeF(10.0));
    sky->setShader(NKShaderProgram::shaderNamed("sky", NKS_COLOR_MODE_UNIFORM, 1, 0));
    sky->setCullFace(NKCullFaceBack);

    addChild(sky);

    auto ground = NKMeshNode::meshNodeWithPrimitive(NKPrimitivePlane, nullptr, NKColor(1.0, .5), V3Make(20, 20, 1));

    //auto ground = NKMeshNode::meshNodeWithVbo(NKVertexBuffer::planeWithSubdivisions(4, 4), 0, nullptr, NKColor(1,.5), V3Make(20, 20, 1));
    ground->setPosition(V3t{0,-1,0});
    ground->setOrientationEuler(V3Make(-90, 0, 0));

    //ground->setShader(NKShaderProgram::shaderNamed("vertexColor",NKS_COLOR_MODE_VERTEX,0,0,0));
    //ground->setDrawMode(GL_LINES);
    //ground->drawBoundingBox = true;

    //ground->setDrawMode(GL_LINES);

    //ground->setShader(NKShaderProgram::shaderNamed("water", {NKShaderModule::lightModule(true, 0), NKShaderModule::swirl(.5, V2t{20,20}), NKShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, 0)}));

    sky->addChild(ground);

    auto axes = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveAxes, nullptr, NKColor(1.0, 0,0,1.0), V3MakeF(1));

    axes->setShader(NKShaderProgram::shaderNamed("vertexColor",NKS_COLOR_MODE_VERTEX,0,0,0));

    //axes->runAction(NKAction::rotateZByAngle(20, 1.0)->repeatForever());
    axes->shouldDrawBoundingBox = true;

    addChild(axes);

    auto sun = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveSphere, nullptr, NKColor("lightsalmon"), V3MakeF(.25));
    sun->setPosition(V3Make(2, 0, 0));
    addChild(sun);

    sun->shouldDrawBoundingBox = true;

    sun->setShader(NKShaderProgram::shaderNamed("water2", {NKShaderModule::lightModule(true, 0), NKShaderModule::swirl(.5, V2t{5.0,5.0}), NKShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, 0)}));

    auto cubeEmitter = NKBatchNode::meshNodeWithPrimitive(NKPrimitiveSphere, moonTex, NKColor(1), V3MakeF(1));

    //cubeEmitter->setShader(NKShaderProgram::shaderNamed("water3", {NKShaderModule::lightModule(true, NK_BATCH_SIZE), NKShaderModule::swirl(.5, V2t{5.0,5.0}), NKShaderModule::textureModule(1),NKShaderModule::colorModule(NKS_COLOR_MODE_UNIFORM, NK_BATCH_SIZE)}, NK_BATCH_SIZE));

    auto glowBalls = NKBatchNode::meshNodeWithPrimitive(NKPrimitiveCube, nullptr, NKColor(1), V3MakeF(1));


    addChild(cubeEmitter);
    addChild(glowBalls);
    glowBalls->setBlendMode(NKBlendModeAdd);

    int sph = 256;

    auto light = NKLightNode::lightNode();
    auto light2 = NKLightNode::lightNode();
    auto light3 = NKLightNode::lightNode();

#if NK_USE_GLES
    sph = 50;
#endif
    for (int i = 0; i < sph; i++) {
        auto cube = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveCube, nullptr,NKColor(1.0), V3MakeF(rand() % 50 * .00025 + .002));

        auto start =  NKAction::enterOrbitAtLongitude(rand() % 230, rand() % 219, .1, 1.0);

        start->completionBlock = [cube, cubeEmitter, glowBalls, sky, light]
        {
            auto pan = NKAction::group({
                                               NKAction::sequence({
                                                                          NKAction::delay(2),
                                                                          NKAction::sequence({
                                                                                                     NKAction::delay(.5),
                                                                                                     NKAction::moveTo(V3MakeF(0), 1.0)->timingMode(NKActionTimingEaseIn),
                                                                                             }),
                                                                          NKAction::delay(.5),
                                                                          NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
                                                                              if (action->frameCount == 0) {
                                                                                  node->runAction(NKAction::enterOrbitAtLongitude(rand() % 23, rand() % 121, rand() % 50 * .11, .6)->timingMode(NKActionTimingEaseOut));
                                                                              }
                                                                          }),
                                                                          NKAction::delay(.5),
                                                                          NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
                                                                              if (action->frameCount == 0) {
                                                                                  node->runAction(NKAction::maintainOrbitDelta(rand() % 303, rand() % 301, 0, 1.0)->timingMode(NKActionTimingEaseInEaseOut));
                                                                              }})
                                                                  }),
                                               NKAction::sequence({
                                                                          NKAction::delay(.01),
                                                                          NKAction::rotateByAngles(V3Make(270,0, rand() %30 + 20), rand() % 2 + .1)->timingMode(NKActionTimingEaseOut),
                                                                          NKAction::delay(.07),
                                                                          NKAction::rotateByAngles(V3Make(0,230,0), .3)->timingMode(NKActionTimingEaseOut),
                                                                          NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
                                                                              if (action->frameCount == 0) {
                                                                                  node->runAction(NKAction::resize(V3MakeF(rand() % 20 * .01 + .001),.3)->timingMode(NKActionTimingEaseInEaseOut));
                                                                              }
                                                                          })
                                                                  })->repeatForever()
                                       })->repeat(2);

            pan->completionBlock = [cube, cubeEmitter, glowBalls, sky, light]{
                if (random()%100 > 10){
                    cube->runAction(NKAction::resize(V3MakeF(.03), 4.)->timingMode(NKActionTimingEaseIn), [cubeEmitter, cube, sky]{
                        auto cbounce = NKAction::customActionWithDuration(2.0, [](NKAction *action, NKNode* node, F1t completion)
                        {
                            if (node->hasActions() == 1) {
                                float rate = rand()%20 * .1 + .1;

                                node->runAction(NKAction::sequence(
                                        {
                                                NKAction::moveToY(-1, rate)->timingMode(NKActionTimingEaseIn),
                                                NKAction::moveToY(node->position().y, rate)->timingMode(NKActionTimingEaseOut)
                                        }
                                ));

                                node->runAction(NKAction::strobeAlpha(rand()%4, rand()%10, rate));
                            }
                        });
                        cube->runAction(cbounce->repeatForever());

                    });
                }
                else {
                    auto clone = NKMeshNode::meshNodeWithPrimitive(NKPrimitiveCube, nullptr, NKColor(1.0,.6), V3MakeF(.3));
                    clone->setOrientationEuler(V3Make(45, 0, 0));
                    clone->runAction(NKAction::followNode(cube.get(), 1.0)->repeatForever());
                    glowBalls->addChild(clone);
                    clone->setSize(cube->size()*1.1);
                    clone->runAction(NKAction::rotateYByAngle(90, 1.0)->repeatForever());
                    cube->runAction(NKAction::fadeColorTo(NKColor(random()%100*.01, random()%100*.01, random()%100*.01, .6),1.0));
                }


            };

            cube->runAction(pan);
        };

        cube->runAction(start);

        cubeEmitter->addChild(cube);
    }

    camera->runAction(NKAction::moveTo(V3MakeF(0), 10), [sky, light, cubeEmitter, glowBalls, this](){

        auto spin = NKAction::customActionWithDuration(10.0, [](NKAction *action, NKNode* node, F1t completion)
        {
            action->startFloat *= 1.02;
            node->setOrientationEuler(V3Make(0,action->startFloat,0));
        });
        spin->startFloat = 1.0;

        sky->runAction(spin);

        sky->runAction(NKAction::fadeAlphaTo(0, 10.0), [cubeEmitter, sky, glowBalls, this]{

            cubeEmitter->setTexture(nullptr);
            sky->setTexture(skyTex2);

            sky->runAction(NKAction::sequence({NKAction::delay(20.0),
                                               NKAction::fadeAlphaTo(1.0, 10.0)}
            ), [cubeEmitter,glowBalls,sky, this]()
                           {
                               glowBalls->runAction(NKAction::moveToY(30, 55)->timingMode(NKActionTimingEaseIn));
                               camera->removeAllActions();
                               camera->runAction(NKAction::moveToY(30, 60));
                               cubeEmitter->runAction(NKAction::delay(40),[cubeEmitter]{
                                   cubeEmitter->runAction(NKAction::moveToY(30, 10));
                               });
                               sky->runAction(NKAction::delay(10), [cubeEmitter,sky,this]()
                               {
                                   sky->runAction(NKAction::customActionWithDuration(40,[sky,this](NKAction *action, NKNode* node, F1t completion)
                                   {
                                       if (action->frameCount % 3 == 0) {
                                           sky->setAlpha(rand()%100*.01 * (1.0-completion) + (1.0-completion)*.5);
                                       }
                                   }
                                   ), [this]()
                                                  {
                                                      this->runAction(NKAction::delay(5),[this](){
                                                          this->camera->removeAllActions();
                                                          this->camera->removeFromParent();
                                                          this->removeAllChildren();
                                                          this->moveToView();
                                                      }
                                                      );});
                                   cubeEmitter->setTexture(skyTex);
                               });
                           });
        });

        auto enterOrbit2 = NKAction::enterOrbitAtLongitude(rand() % 130, rand() % 119, 1.5, 1.0);

        enterOrbit2->completionBlock = [light]{
            light->runAction(NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
                if (node->hasActions() == 1){
                    node->runAction(NKAction::maintainOrbitDelta(rand() % 303, rand() % 301, 0, .5)->timingMode(NKActionTimingEaseInEaseOut));
                }
            })->repeatForever());

            auto glow = NKAction::customActionWithDuration(10.0, [](NKAction *action, NKNode* node, F1t completion)
            {
                ((NKLightNode*)node)->properties.ambient = .15 - completion*.1;
            });
            light->runAction(glow);
            light->runAction(NKAction::resize(V3MakeF(.01), 10.));
        };

        light->runAction(enterOrbit2);



    });



    auto enterOrbit1 = NKAction::enterOrbitAtLongitude(rand() % 119, 0 , 3, 1.0, V3Make(1, 0, 0));

    enterOrbit1->completionBlock = [cameraTrack, cameraArm]{
        cameraTrack->runAction(NKAction::customActionWithDuration(1.0, [cameraArm](NKAction *action, NKNode* node, F1t completion){
            if (node->hasActions() == 1){
                float time = rand()%100 * .1 + 2;

                int dir = rand()%100 > 50 ? 15 : -15;

                node->runAction(NKAction::maintainOrbitDelta(dir*time,0, 0, time, V3Make(1, 0, 0))->timingMode(NKActionTimingEaseInEaseOut));


//                cameraArm->runAction(NKAction::customActionWithDuration(time, [cameraArm, time](NKAction *action, NKNode* node, F1t completion){
//                    if (node->hasActions() == 1){
//
//                        float size = rand()%100 * .01;
//
//                        int tm = time*.5;
//                        float q = (rand()%8 * .1 * tm) + (tm*.2);
//                        node->runAction(NKAction::sequence({
//                            NKAction::moveBy({0,size,0}, q)->timingMode(NKActionTimingEaseIn),
//                            NKAction::moveBy({0,-size,0}, tm-q)->timingMode(NKActionTimingEaseOut)
//                        }));
//                    }
//                }));

            }
        })->repeatForever());
    };

    cameraTrack->runAction(enterOrbit1);
//
//    auto enterOrbit = NKAction::enterOrbitAtLongitude(rand() % 130, rand() % 119, .1, 1.0);
//    enterOrbit->completionBlock = [this]{
//        camera->target.runAction(NKAction::customActionWithDuration(1.0, [](NKAction *action, NKNode* node, F1t completion){
//            if (node->hasActions() == 1){
//                if (rand()%100 > 90) {
//                    node->runAction(NKAction::maintainOrbitDelta(rand() % 303, rand() % 301, 0, rand()%4*.1 + 1));
//                }
//                if (rand()%100 == 5) {
//                    node->runAction(NKAction::maintainOrbitDelta(rand() % 303, rand() % 301, 0, rand()%20*.1 + 1));
//                }
//
//            }
//        })->repeatForever());
//    };
//
//    camera->target.runAction(enterOrbit);
//
    light->properties.linearAttenuation = .1;
    light->properties.color = V3Make(.8, .8, 1.0);
    light->properties.ambient = .1;

    light->setSize(V3MakeF(.2));
    light->setPosition(V3Make(0, 10, 0));



    light2->properties.linearAttenuation = .2;
    light2->properties.color = V3Make(1.0, 0, 0);
    light2->properties.ambient = .1;

    light2->setSize(V3MakeF(.2));
    light2->setPosition(V3Make(0, 5, 0));

    light3->properties.linearAttenuation = .3;
    light3->properties.color = V3Make(0, 0, 1.0);
    light3->properties.ambient = .1;

    light3->setSize(V3MakeF(.2));
    light3->setPosition(V3Make(5, 0, 0));

    addChild(light);
//    addChild(light2);
//    addChild(light3);


    drawLights = true;
#endif
}

void ExampleScene::handleEvent(NKEvent &event){

}
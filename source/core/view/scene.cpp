//
//  Scene.m
//  example-ofxTableView
//
//  Created by Chroma Developer on 2/17/14.
//
//

#include "scene.h"
#include "label.h"
#include "../gl/frameBuffer.h"
#include "../gl/texture.h"
#include "../gl/vertexBuffer.h"
#include "../gl/shaderProgram.h"
#include "../platform/common/sceneController.h"
#include "../platform/common/uxEvent.h"
#include "../node/camera.h"
#include "../node/light.h"
#include "../util/time.h"
#include "../animation/action.h"
#include "collectionView.h"


using namespace std;
using namespace Shader;

#if NK_LOG_METRICS
unsigned int Scene::draws {0};
unsigned int Scene::fps {0};
unsigned int Scene::mspf {0};
#endif

//Scene* Scene::activeScene = nullptr;

Scene::Scene(S2t size) : View(size){

#if NK_USE_MIDI
  [[NKMidiManager sharedInstance] setDelegate:self];
  _midiReceivedBlocks = [[NSMutableDictionary alloc]init];
#endif

#if NK_GL_DEBUG
  // Obtain iOS version
  int OSVersion_ = 0;

#if (TARGET_IOS || TARGET_OSX)
#if TARGET_IOS
  NSString *OSVer = [[UIDevice currentDevice] systemVersion];
#elif TARGET_OSX
  SInt32 versionMajor, versionMinor, versionBugFix;
  Gestalt(gestaltSystemVersionMajor, &versionMajor);
  Gestalt(gestaltSystemVersionMinor, &versionMinor);
  Gestalt(gestaltSystemVersionBugFix, &versionBugFix);

  NSString *OSVer = [NSString stringWithFormat:@"%d.%d.%d", versionMajor, versionMinor, versionBugFix];
#endif
  NSArray *arr = [OSVer componentsSeparatedByString:@"."];
  int idx = 0x01000000;
  for( NSString *str in arr ) {
    int value = [str intValue];
    OSVersion_ += value * idx;
    idx = idx >> 8;
  }
  NSLog(@"OS version: %@ (0x%08x)", OSVer, OSVersion_);
  //char* glExtensions = (char*)glGetString(GL_EXTENSIONS);
  //NSLog(@"GL EXT: %@",[NSString stringWithCString:glExtensions encoding: NSASCIIStringEncoding]);
#endif
  nkLog("GL_VENDOR:   %s", glGetString(GL_VENDOR) );
  nkLog("GL_RENDERER: %s", glGetString ( GL_RENDERER   ) );
  nkLog("GL_VERSION:  %s", glGetString ( GL_VERSION    ) );
#endif

  name = "scene";

  setBackgroundColor({.2, .2, .2, 1.0});
  _userInteractionEnabled = true;

  setBlendMode(GLBlendModeNone);
  setCullFace(GLCullFaceNone);
  setUsesDepth(false);
  _scene = this;


  camera = make_shared<Camera>(this);
  camera->base->position.set(V3(0,0,-1));

  useColorDetection = true;

  hitDetectBuffer = make_unique<FrameBuffer>(size.width, size.height);

  hitDetectShader = Program::shaderNamed("hitShaderSingle",NKS_COLOR_MODE_UNIFORM,0,0,0);

  printf("init scene with size, %f %f \n", size.width, size.height);

  GetGLError();

}

void Scene::logMetrics() {
#if NK_LOG_METRICS
  nkLog("fps %d : frametime: %1.3f nodes: %lu : textures: %lu : bBodies %lu : lights %lu", fps, (frameTime / frames), allChildren().size(), (unsigned long)666, (unsigned long)666, (unsigned long)lights.size());
#endif
}

void Scene::updateWithTimeSinceLast(F1t dt){

  currentFrameTime = Time::getCurrentTime();

  if (!loaded){
    loaded = true;
    moveToView();
  }
  frames++;

#warning  UPDATE SOUND GL ETC
  //    [NKSoundManager updateWithTimeSinceLast:dt];
  //    [CocoaGLManager updateWithTimeSinceLast:dt];
  //    [[NKBulletWorld sharedInstance] updateWithTimeSinceLast:dt];

  framesToRender += dt*frameRate;

  if (!framesToRender) {
    framesToRender = 1.0;
  }

  while (framesToRender >= 1.0) {
    camera->updateWithTimeSinceLast(dt/framesToRender);
    Node::updateWithTimeSinceLast(dt/framesToRender);
    framesToRender -= 1.0;
  }

  if (_updateBlock){
    _updateBlock(dt);
  }

  #if NK_LOG_METRICS

  if (!overlay){
    overlay = View::view();
    overlay->setLayoutMethod(Layout::Method::Horizontal);

    overlay->addChild(View::view());
    overlay->addChild(View::view());
    overlay->addChild(View::view());

    metrics = View::view();
    overlay->addChild(metrics);

    metrics->setLayoutMethod(Layout::Method::Vertical);
    //metrics->setScene(this);
    metrics->setBackgroundColor({1.0,0.5});
    metrics->setForceOrthographic();
    metrics->setIgnoresLights();

    for (int i = 0; i < 7; i++){
      auto label = Label::labelWithFontNamed("Roboto.ttf", 24.0, BLACK);
      label->setTextAlignment(NKTextAlignmentRight);
      metrics->addChild(label);
    }

    metrics->setUpdateBlock([this](float dt){
      static_pointer_cast<Label>(metrics->children()[0])->setText(nksf("%d fps", fps));
      static_pointer_cast<Label>(metrics->children()[1])->setText(nksf("%d ms", mspf));
      static_pointer_cast<Label>(metrics->children()[2])->setText(nksf("%d draws", draws));
      static_pointer_cast<Label>(metrics->children()[3])->setText(nksf("%d nodes", Node::liveObjects()));
      static_pointer_cast<Label>(metrics->children()[4])->setText(nksf("%d events", UXEvent::liveObjects()));
      static_pointer_cast<Label>(metrics->children()[5])->setText(nksf("%d fbos", FrameBuffer::liveObjects()));
      static_pointer_cast<Label>(metrics->children()[6])->setText(nksf("%d actions", Action::liveObjects()));
    });
  }

  metrics->updateWithTimeSinceLast(dt);

  timer += dt;
  if (timer > 1.0f) {
    mspf = ((frameTime * 1000) / frames);
    fps = (int)frames;
    frames = 0;
    timer -= 1.0f;
    frameTime = 0;
  }
  draws = 0;

#endif
}

void Scene::drawHitBuffer() {

  nkGetGLError();
  setBlendMode(GLBlendModeNone);
  glDisable(GL_BLEND);

  nkGetGLError();

  Node::drawWithHitShader();

  nkGetGLError();
}

void Scene::processHitBuffer() {

  hitDetectBuffer->bind();

  glViewport(0, 0, size.get().width, size.get().height);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  drawHitBuffer();

  while (hitQueue.size()) {
    hitQueue.front()();
    hitQueue.pop();
  }
}


//void Scene::bindMainFrameBuffer(Node *sender){
//
//    if (sender != this && framebuffer != nullptr) {
//        framebuffer->bind();
//    }
//    else {
//#if TARGET_IOS
//        view->framebuffer->bind();
//#elif (TARGET_OSX || NK_USE_WEBGL)
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//#endif
//    }
//    //NSLog(@"bind fb %@ %@", view, view.framebuffer);
//}

void Scene::draw() {
  clear();

  if (backgroundColor().visible()) {
    glClearColor(backgroundColor().r, backgroundColor().g, backgroundColor().b, backgroundColor().a);
  }

#if DRAW_HIT_BUFFER
  drawHitBuffer();
#else
  Node::draw();
#endif



#if NK_LOG_METRICS
  if (metrics){
    metrics->draw();
  }
#endif

  frameTime += Time::getCurrentTime() - currentFrameTime;
}

void Scene::customDraw() {
  if (drawAxes) {
    if (!axes) {
      axes = Mesh::nodeWithPrimitive(NKPrimitiveAxes, nullptr, Color(1.0, 0,0,1.0), V3(100));
      axes->shader = Program::shaderNamed("vertexColor",NKS_COLOR_MODE_VERTEX,0,0,0);
    }
//    if (!_::contains(children(), axes)) {
//      nkLog("add axes to scene");
//      addChild(axes);
//    }
  }
  if (drawCamera) {
    //camera->draw();
  }
}
//void Scene::drawStereo() {
//    glClearColor(0, 0, 0, 0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//    for(int eye = 0; eye < 2; eye++){  // 0 = left , 1 = right
//
//        camera->fovVertRadians = 120;
//        camera->aspect = size().width / size().height;
//
//
//        if(eye == 0){
//            glRotatef(-oculusRift.IPD*100.0, 0.0f, 1.0f, 0.0f);
//            //            glTranslatef(oculusRift.IPD, 0.0f, 0.0f);
//        }
//        else if (eye == 1){
//            glRotatef(oculusRift.IPD*100.0, 0.0f, 1.0f, 0.0f);
//            //            glTranslatef(-oculusRift.IPD, 0.0f, 0.0f);
//        }
//        glPushMatrix();
//
//        // apply headset orientation
//        glMultMatrixf(oculusRift.orientation);
//        // mouse rotation
//        glRotatef(mouseRotation.y, 1, 0, 0);
//        glRotatef(mouseRotation.x, 0, 1, 0);
//        // draw scene
//        //-------------------------
//        // keyboard translation
//        scene.render(walkPosition.x, walkPosition.y);  // place for custom scene
//        //-------------------------
//        glPopMatrix();
//        // unbind framebuffer, now render to screen
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//        glBindTexture(GL_TEXTURE_2D, textureId);
//        glGenerateMipmap(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, 0);
//
//        if(eye == 0)                        // left screen
//            glViewport (0, 0, w/2., h);
//        else if (eye == 1)                  // right screen
//            glViewport (w/2., 0, w/2., h);
//
//        glMatrixMode (GL_PROJECTION);
//        glLoadIdentity ();
//        [self glPerspective:90.0f Aspect:(GLfloat)(w/2.0f)/(GLfloat)(h) Near:.1f Far:10.0f];
//        glMatrixMode (GL_MODELVIEW);
//
//        glLoadIdentity();
//        glPushMatrix();
//
//        if(warping){
//            glUseProgram(_program);
//            // preset suggestions from http://www.mtbs3d.com/phpbb/viewtopic.php?f=140&t=17081
//            const float Scale[2] = {0.1469278, 0.2350845};
//            const float ScaleIn[2] = {2, 2.5};
//            const float HmdWarpParam[4] = {1, 0.22, 0.24, 0};
//            const float LeftLensCenter[2] = {0.2863248*2.0, 0.5};
//            const float LeftScreenCenter[2] = {0.55, 0.5};
//            const float RightLensCenter[2] = {(0.7136753-.5) * 2.0, 0.5};
//            const float RightScreenCenter[2] = {0.45, 0.5};
//            // apply shader uniforms
//            glUniform2fv(uniforms[3], 1, Scale);
//            glUniform2fv(uniforms[4], 1, ScaleIn);
//            glUniform4fv(uniforms[5], 1, HmdWarpParam);
//            if(eye == 0){
//                glUniform2fv(uniforms[1], 1, LeftLensCenter);
//                glUniform2fv(uniforms[2], 1, LeftScreenCenter);
//            }
//            else{
//                glUniform2fv(uniforms[1], 1, RightLensCenter);
//                glUniform2fv(uniforms[2], 1, RightScreenCenter);
//            }
//        }
//        else{  // no warp, closer to fill screen
//            glTranslatef(0, 0, -1.0);
//        }
//        // draw scene on a quad for each side
//        glBindTexture(GL_TEXTURE_2D, textureId);
//        [self drawQuad];
//        glBindTexture(GL_TEXTURE_2D, 0);
//
//        if(warping)
//            glUseProgram(0);
//
//        glPopMatrix();
//    }
//    glFlush();
//}

void Scene::clear() {
  GLState::setActiveShader(nullptr);

  if (boundTexture){
    boundTexture->unbind();
    boundTexture = nullptr;
  }
  if (VertexBuffer::activeVBO){
    VertexBuffer::activeVBO->unbind();
    VertexBuffer::activeVBO = nullptr;
  }
}

void Scene::bindLights() {
  GLState::activeShader()->uniformNamed(NKS_I1_NUM_LIGHTS).bindI1((int)lights.size());
  nkGetGLError();
  if (lights.size() != 0) {
    for (int i = 0; i < lights.size(); i++) {
      GLState::activeShader()->uniformNamed(NKS_LIGHT).bindLightProperties(lights[i]->properties, i);
      nkGetGLError();
    }
  }
}


void Scene::dispatchUXEvent(UXEvent& event) {
  //NSLog(@"dispatch event for location %f %f",location.x, location.y);

#warning dispatchUXEvent
  //
  //    if (useColorDetection) {
  //
  //        HitCallback callBack = [this, event]{
  //            UB4t hc;
  //            glReadPixels(event.screenLocation.x, event.screenLocation.y,
  //                         1, 1,
  //                         GL_RGBA, GL_UNSIGNED_BYTE, &hc);
  //#warning do Shader Manager
  //           Node *hit = [ShaderManager nodeForColor:hc];
  //
  //            if (!hit){
  //                hit = this;
  //            }
  //
  //            event.node = hit;
  //
  //            hit->handleUXEvent(event);
  //        };
  //
  //        hitQueue.push(callBack);
  //    }
  //
  //    else if (useBulletDetection) {
  //        // IMPLEMENT THIS
  //    }
}

void Scene::setView(SceneController* view_){
  view = view_;
}

Scene::~Scene() {
  if (loaded) {
    unload();
  }
}

void Scene::unload() {
  nkLog("tear down scene . . .");

  removeAllChildren();

  nkLog("cleared children");

  while (hitQueue.size()) {
    hitQueue.pop();
  }

#if NK_USE_MIDI
  [_midiReceivedBlocks removeAllObjects];
#endif

  camera->reset();

  nkLog("reset camera");

#warning bullet
  //   [NKBulletWorld reset];

  lights.clear();

  nkLog("cleared lights");

  hitDetectBuffer->down();

  glFinish();
  clear();

  nkLog("reset GL pipeline");

  loaded = false;

  nkLog("unloaded . . . ");

}

#if NK_USE_MIDI
-(void)handleMidiCommand:(MIKMIDICommand *)command {
  for (MidiReceivedBlock block in _midiReceivedBlocks.allValues){
    block(command);
  }
  //else {
  // NSLog(@"received midi, provide scene a MidiReceivedBlock, or instead, implement: -(void)handleMidiCommand:(MIKMIDICommand *)command in subclass");
  //}
}
#endif

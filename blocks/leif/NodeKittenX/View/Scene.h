/***********************************************************************
 
 Written by Leif Shackelford
 Copyright (c) 2014 Chroma.io
 All rights reserved. *
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met: *
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of CHROMA GAMES nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission. *
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE. *
 ***********************************************************************/

//#define SHOW_HIT_DETECTION


#ifndef __Scene_h_
#define __Scene_h_

#include "View.h"
#include "Camera.h"

#if USE_OVR
#include "NKOculusInterface.h"
#endif

//#import "NKAlertSprite.h"

//#if NK_USE_MIDI
//#import "MIKMIDI.h"
//typedef void (^MidiReceivedBlock)(MIKMIDICommand* command);
//#define newMidiReceivedBlock (MidiReceivedBlock)^(MIKMIDICommand* command)
//#endif

typedef std::function<void(void)> HitCallback;

class SceneController;
class Camera;
namespace Shader {
  class Program;
}
class VertexBuffer;
class NKAlertSprite;
class FrameBuffer;
class Texture;
class Light;
class CollectionView;

//class NKBulletWorld;

class Scene : public View
{
  unsigned long long frames {0};
  double currentFrameTime {0};
  double frameTime {0};
  double frameRate {60};
  double framesToRender {0};
  
protected:
  
  double timer {0};
  
public:
  //static Scene *activeScene;
  
#if NK_LOG_METRICS
  static unsigned int draws;
  static unsigned int fps;
  static unsigned int mspf;
#endif
  
  bool loaded {false};
  
#if USE_OVR
  NKOculusInterface oculus;
#endif
  
  Scene(S2t size);
  ~Scene();
  
  // NODE HIERARCHY
  virtual void moveToView(){
    //activeScene = this;
  };
  
  virtual void moveFromView(){
    unload();
  };
  
  SceneController *view = nullptr;
  
  //@property (nonatomic) void *view;
  
  Color borderColor;
  
  std::shared_ptr<Camera> camera;
  std::shared_ptr<Mesh> axes;
  
  std::shared_ptr<View> overlay;
  std::shared_ptr<View> metrics;
  
  M16Stack stack;
  // NKAlertSprite *alertSprite;
  
  // SCENE DEBUG TOOLS //
  bool drawLights {false};
  bool drawCamera {false};
  bool drawAxes {false};
  
  std::vector<Light*> lights;
  
  // HIT DETECTION
  
  void dispatchUXEvent(UXEvent* event);
  
  
  // COLOR BASED HIT DETECTION
  bool useColorDetection {false};
  std::shared_ptr<Shader::Program> hitDetectShader;
  std::unique_ptr<FrameBuffer> hitDetectBuffer;
  
  std::queue<HitCallback> hitQueue;
  
  // PHYSICS HIT DETECTION
  
  bool useBulletDetection;
  
  // GL STATE MACHINE CONTROL
  
  VertexBuffer *boundVertexBuffer = nullptr;
  Texture *boundTexture = nullptr;
  bool fill {false};
  
  // EXTERNAL MODULES
#warning midi
  //#if NK_USE_MIDI
  //    void handleMidiCommand:(MIKMIDICommand*)command;
  //    NSMutableDictionary* midiReceivedBlocks;//MidiReceivedBlock midiReceivedBlock;
  //#endif
  
  void setView(SceneController* view);
  
  void bindMainFrameBuffer(Node* sender);
  void clear();
  void unload();
  
  void pushMultiplyMatrix(M16t matrix);
  void pushScale(V3t scale);
  void popMatrix();
  
  void setUniformIdentity();
  
  // GEOMETRY
  M16t orthographicMatrix() {
    auto sz = size.get();
    return M16MakeOrtho(-sz.width*.5f, sz.width*.5f, -sz.height*.5f, sz.height*.5f, 65535, -65535);
  }
  
  // EVENTS
  
  void dispatchUXEvent(UXEvent& event);
  
  // UPDATE
  
  // NODE OVERRIDES
  
  
  void afterTransform() override {
    View::afterTransform();
    for (auto& child : children()){
      child->afterTransform();
    }
    auto theSize = size.get();
    camera->setAspectRatio(theSize.width / theSize.height);
#if NK_LOG_METRICS
    if (overlay){
      overlay->size.set(size.get());
    }
#endif
  }
  
  void afterResize() override {
    afterTransform();
  }
  
  void pushStyle() override {};
  void updateWithTimeSinceLast(F1t dt) override;
  
  void bindLights();
  // DRAW
  
  virtual void draw() override;
  virtual void customDraw() override;
  void drawStereo();
  
  // HIT BUFFER
  
  void processHitBuffer();
  void drawHitBuffer();
  
  void logMetrics();
  
  // ALERT
  
  void presentAlert(NKAlertSprite* alert, bool animated);
  void alertDidSelectOption(int option);
  void alertDidCancel();
  void dismissAlertAnimated(bool animated);
  
  
};

#endif



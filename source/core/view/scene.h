#pragma once

#include "view.h"
#include "../node/camera.h"

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
      overlay->size.set(theSize);
    }
#endif
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
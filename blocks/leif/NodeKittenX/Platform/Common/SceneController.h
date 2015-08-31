//*
//*  NODE KITTEN
//*

#pragma once

// NK VIEW IS AN ABSTRACT BASE CLASS
// SOME WILL BELONG TO NATIVE VIEWS
// OTHERS WILL REPERESENT THE BASE OF A SINGLE VIEW APP
// AND ENCOMPASS A MAIN() LIKE INFINITE LOOP FUNCTION


#include "Generic.h"

class FrameBuffer;
class Texture;
class Mesh;
class VertexBuffer;
class Scene;
class UXEvent;

namespace Shader {
  class Program;
}

class SceneController : public Sizeable<V2> {
  
  std::shared_ptr<Scene> _scene;
  
public:
  
  SceneController(S2t size);
  
  U1t drawHitEveryXFrames = 10;
  U1t framesSinceLastHit {0};
  U1t rotate {0};
  
  D1t lastTime = 0;
  
  std::unique_ptr<FrameBuffer> framebuffer;
  std::shared_ptr<Texture> texture;
  std::shared_ptr<Shader::Program> defaultShader;
  std::shared_ptr<VertexBuffer> vertexBuffer;
  std::shared_ptr<Mesh> rect;
  
  void afterResize() override;
  void draw();
  
  const std::shared_ptr<Scene>& scene() const {return _scene;}
  void setScene(std::shared_ptr<Scene> scene);
  
  void *nativeView;
  
  std::vector<std::shared_ptr<UXEvent>>events;
  
  void handleUXEvent(std::shared_ptr<UXEvent> event);
};


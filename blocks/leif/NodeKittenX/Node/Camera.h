
//  Camera.h
//  nike3dField
//
//  Created by Chroma Developer on 4/1/14.
//
//

#pragma once

#include "Mesh.h"

class SceneNode;

typedef enum NKProjectionMode {
  NKProjectionModePerspective,
  NKProjectionModeOrthographic
} NKProjectionMode;

class Camera {
  
  NKProjectionMode _projectionMode {NKProjectionModePerspective};
  
  Scene* _scene;
  
  F1t fovVertRadians;
  F1t aspect;
  F1t nearZ;
  F1t farZ;
  
public:
  Camera(Scene* scene);
  
  Reactive::Pullable<M16t> viewMatrix;
  Reactive::Pullable<M16t> projectionMatrix;
  Reactive::Pullable<M16t> viewProjectionMatrix;
  
  void reset();
#pragma mark - NODAL OVERRIDES
  std::shared_ptr<Mesh> target; // MESH FOR DEBUG DRAW
  std::shared_ptr<Mesh> base; // MESH FOR DEBUG DRAW
  
  void lookAtNode(Node *node); // auto-orient camera
  void attachTo(Node* parent); // auto-position camera
  
  void setAspectRatio(F1t aspectRatio){
    aspect = aspectRatio;
  }
  
#if USE_OVR
  void setupCamera(OVR::Util::Render::StereoEye eye);
#endif
  
  V3t eyeDirection();
  P2t screenToWorld(P2t p);
  
  void updateCameraWithTimeSinceLast(F1t dt);
  void updateWithTimeSinceLast(F1t dt);
  
  static void glInit();
  
  void setProjectionMode(NKProjectionMode mode){_projectionMode = mode;};
};

class FrustumG {
  
private:
  
  enum {
    TOP = 0, BOTTOM, LEFT,
    RIGHT, NEARP, FARP
  };
  
public:
  
  enum {OUTSIDE, INTERSECT, INSIDE};
  
  Plane pl[6];
  
  V3t ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
  float nearD, farD, ratio, angle,tang;
  float nw,nh,fw,fh;
  
  FrustumG();
  ~FrustumG();
  
  void setCamInternals(float angle, float ratio, float nearD, float farD);
  void setCamDef(V3t &p, V3t &l, V3t &u);
  int pointInFrustum(V3t &p);
  int sphereInFrustum(V3t &p, float raio);
  //int boxInFrustum(AABox &b);
  
};
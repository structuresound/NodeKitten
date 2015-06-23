
//  Camera.h
//  nike3dField
//
//  Created by Chroma Developer on 4/1/14.
//
//

#ifndef __Camera_h_
#define __Camera_h_
//test
#include "Mesh.h"

class SceneNode;

typedef enum NKProjectionMode {
    NKProjectionModePerspective,
    NKProjectionModeOrthographic
} NKProjectionMode;

class Camera : public Node {
    
    M16t _viewMatrix;
    M16t _projectionMatrix;
    M16t _viewProjectionMatrix;
    
    bool _vDirty = true;
    bool _pDirty = true;
    bool _vpDirty = true;
    
    NKProjectionMode _projectionMode {NKProjectionModePerspective};
    
    Scene* _scene;
    
public:
    
    Camera(Scene* scene);
    
    F1t fovVertRadians;
    F1t aspect;
    F1t nearZ;
    F1t farZ;

    nk_unique_ptr(Mesh) target;
    
    void setTarget(Node *node);
    
    M16t viewMatrix();
    M16t projectionMatrix();
    M16t viewProjectionMatrix();
    M16t orthographicMatrix();
    
#if USE_OVR
    void setupCamera(OVR::Util::Render::StereoEye eye);
#endif
    V3t eyeDirection();
    
    P2t screenToWorld(P2t p);
    
    void updateCameraWithTimeSinceLast(F1t dt);
    void updateWithTimeSinceLast(F1t dt);
    
    static void glInit();
    
    void setProjectionMode(NKProjectionMode mode){_projectionMode = mode;};
    
    // NODE OVERRIDES
    void lookAtNode(Node* node);
    void setDirty(bool dirty);
    
    void drawChildren() override;
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


#endif

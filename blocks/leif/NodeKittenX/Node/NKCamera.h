//
//  NKCamera.h
//  nike3dField
//
//  Created by Chroma Developer on 4/1/14.
//
//

#ifndef __NKCamera_h_
#define __NKCamera_h_
//test
#include "NKNode.h"

class SceneNode;

typedef enum NKProjectionMode {
    NKProjectionModePerspective,
    NKProjectionModeOrthographic
} NKProjectionMode;

class NKCamera : public NKNode {
    
    M16t _viewMatrix;
    M16t _projectionMatrix;
    M16t _viewProjectionMatrix;
    
    bool _vDirty = true;
    bool _pDirty = true;
    bool _vpDirty = true;
    
public:
    
    NKCamera(NKSceneNode* scene);
    NKProjectionMode projectionMode;
    
    F1t fovVertRadians;
    F1t aspect;
    F1t nearZ;
    F1t farZ;
    
    NKNode target;
    
    void setTarget(NKNode *node);
    
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
    
    void initGL();
    
    // NODE OVERRIDES
    void lookAtNode(NKNode* node);
    void setDirty(bool dirty);
    
};

class Vector;
class Line;
class Plane;
class Matrix;
class Transform;

class Line{
public:
    V3t o,d;
    Line(){
    }
    Line( const V3t &o,const V3t &d ):o(o),d(d){
    }
    Line operator+( const V3t &q )const{
        return Line( o+q,d );
    }
    Line operator-( const V3t &q )const{
        return Line( o-q,d );
    }
    V3t operator*( float q )const{
        return o+d*q;
    }
    V3t nearest( const V3t &q )const{
        return o+d*(d.dot(q-o)/d.dot(d));
    }
};

class Plane{
public:
    V3t n;
    float d;
    
    Plane():d(0){
    }
    //normal/offset form
    Plane( const V3t &n,float d ):n(n),d(d){
    }
    
    //point/normal form
    Plane( const V3t &p,const V3t &n ):n(n),d(-n.dot(p)){
    }
    
    //create plane from tri
    Plane( const V3t &v0,const V3t &v1,const V3t &v2 ){
        set(v0, v1, v2);
    }
    
    Plane operator-()const{
        return Plane( -n,-d );
    }
    
    void set( const V3t &v0,const V3t &v1,const V3t &v2 ){
        n=(v1-v0).cross(v2-v0).normalized();d=-n.dot(v0);
    }
    
    float t_intersect( const Line &q )const{
        return -distance(q.o)/n.dot(q.d);
    }
    V3t intersect( const Line &q )const{
        return q*t_intersect(q);
    }
    Line intersect( const Plane &q )const{
        V3t lv=n.cross( q.n ).normalized();
        return Line( q.intersect( Line( nearest( n*-d ),n.cross(lv) ) ),lv );
    }
    V3t nearest( const V3t &q )const{
        return q-n*distance(q);
    }
    void negate(){
        n=-n;d=-d;
    }
    float distance( const V3t &q )const{
        return n.dot(q)+d;
    }
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

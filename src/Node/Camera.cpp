//
//  Camera.m
//  nike3dField
//
//  Created by Chroma Developer on 4/1/14.
//
//

#include "Camera.h"
#include "FrameBuffer.h"
#include "Scene.h"
#include "Action.h"

using namespace std;

Camera::Camera(Scene* scene)
{
  _scene = scene;
  aspect = scene->size.get().width / scene->size.get().height; // Use screen bounds as default
  nearZ = .01f;
  farZ = 10000.0f;
  fovVertRadians = DEGREES_TO_RADIANS(54);
  
  base = Mesh::nodeWithPrimitive(NKPrimitiveSphere, nullptr,  Color(1.0), V3(.005));
  target = Mesh::nodeWithPrimitive(NKPrimitiveSphere, nullptr,  Color(1.0), V3(.005));
  
  viewMatrix.setPullFunction([this]{
    if (target)
      return M16MakeLookAt(base->globalPosition(), target->globalPosition(), base->upVector());
    else // manual
      return M16MakeLookAt(base->globalPosition(), V3MultiplyM16WithTranslation(base->globalTransform.get(), V3{0,0,-1}), base->upVector());
  });
  
  projectionMatrix.setPullFunction([this]{
    if (_projectionMode == NKProjectionModeOrthographic) {
      return _scene->orthographicMatrix();
    }
    else {
      return M16MakePerspective(fovVertRadians,
                                aspect,
                                nearZ,
                                farZ);
    }
  });
  
  viewProjectionMatrix.setPullFunction([this]{
    return M16Multiply(projectionMatrix.get(), viewMatrix.get());
  });
}

void Camera::lookAtNode(Node *node){
  if (node) {
    target->removeActionForKey("_follow");
    // An Example of 2 different ways this do this:
    
    // 1 first using 'then' / completion block, less clear, more concise ?
    target->runAction(Action::moveToFollowNode(node, 1)->then([this,node]{
      target->runAction(Action::followNode(node, 1)->repeatForever(), "_follow");
    }));
    
    // 2 use a sequence = more clear, but less concise ?
    //  auto sequence = Action::sequence({
    //    Action::moveToFollowNode(node, 1),
    //    Action::followNode(node, 1)->repeatForever()
    //  });
    //  target->runAction(sequence, "_follow");
  }
}

void Camera::attachTo(Node *node){
  base->removeActionForKey("_follow");
  base->runAction(Action::followNode(node, 1)->repeatForever(), "_follow");
}

//void Camera::drawChildren() {
//  Node::drawChildren();
//  if (_scene->drawCamera){
//    target->customDraw();
//  }
//}

void Camera::glInit() {
  
}


#pragma mark UTIL

////convert from screen to camera
//-(V3t)s2w:(P2t)ScreenXY {
//    V3t CameraXYZ;
//
//    CameraXYZ.x = ((ScreenXY.x * 2.) / self.scene.size.width) - 1.;
//    CameraXYZ.y = ((ScreenXY.y * 2.) / self.scene.size.height)- 1.;
//    CameraXYZ.z = self.position.z;
//
//    //CameraXYZ.z = ScreenXYZ.z;
//    NSLog(@"noralized screen coords %f %f %f", CameraXYZ.x, CameraXYZ.y, CameraXYZ.z);
//    //get inverse camera matrix
//    M16t inverseCamera = M16InvertColumnMajor([self projectionMatrix], NULL);
//
//    //convert camera to world
//    V3t p = V3MultiplyM16(inverseCamera, CameraXYZ);
//
//    NSLog(@"camera coords %f %f %f", p.x, p.y, p.z);
//    return p;
//}
//
//-(P2t)screenToWorld:(P2t)p {
//
//    V3t p2 = [self s2w:p];
//        p2.x *= 1000.;
//        p2.y *= 1000.;
//
//     //NSLog(@"world i: %f %f o:%f %f", p.x, p.y, p2.x, p2.y);
//    return V2(p2.x, p2.y);
//    //return V2(10000, 10000);
//}
//
//-(P2t)screenPoint:(P2t)p InNode:(Node*)node {
//
//    V3t CameraXYZ;
//    CameraXYZ.x = p.x / self.scene.size.width - 1.0f;
//    CameraXYZ.y = p.y / self.scene.size.height;
//    //CameraXYZ.z = ScreenXYZ.z;
//
//    //get inverse camera matrix
//    M16t inverseCamera = M16InvertColumnMajor([node globalTransform], NULL);
//
//    //convert camera to world
//
//    V3t p2 = V3MultiplyM16(inverseCamera, CameraXYZ);
//
//    p2.x *= 1820.;
//    p2.y *= 1820.;
//
//    //NSLog(@"world i: %f %f o:%f %f", p.x, p.y, p2.x, p2.y);
//    return V2(p2.x, p2.y);
//
//}

void Camera::updateWithTimeSinceLast(F1t dt){
  viewMatrix.setDirty();
  viewProjectionMatrix.setDirty();
  projectionMatrix.setDirty();

  _scene->afterTransform();
  
  if (!base->parent()){
    base->updateWithTimeSinceLast(dt);
  }
  if (!target->parent()){
    target->updateWithTimeSinceLast(dt);
  }
}

void Camera::reset(){
  base->removeAllActions();
  target->removeAllActions();
  base->removeFromParent();
  target->removeFromParent();
}

#pragma mark - Frustum

#define ANG2RAD 3.14159265358979323846/180.0

void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD) {
  
  // store the information
  this->ratio = ratio;
  this->angle = angle;
  this->nearD = nearD;
  this->farD = farD;
  
  // compute width and height of the near and far plane sections
  tang = (float)tan(ANG2RAD * angle * 0.5) ;
  nh = nearD * tang;
  nw = nh * ratio;
  fh = farD  * tang;
  fw = fh * ratio;
  
  
}

void FrustumG::setCamDef(V3t &p, V3t &l, V3t &u) {
  
  V3t dir,nc,fc,X,Y,Z;
  
  // compute the Z axis of camera
  // this axis points in the opposite direction from
  // the looking direction
  Z = p - l;
  Z.normalize();
  
  // X axis of camera with given "up" vector and Z axis
  X = u * Z;
  X.normalize();
  
  // the real "up" vector is the cross product of Z and X
  Y = Z * X;
  
  // compute the centers of the near and far planes
  nc = p - Z * nearD;
  fc = p - Z * farD;
  
  // compute the 4 corners of the frustum on the near plane
  ntl = nc + Y * nh - X * nw;
  ntr = nc + Y * nh + X * nw;
  nbl = nc - Y * nh - X * nw;
  nbr = nc - Y * nh + X * nw;
  
  // compute the 4 corners of the frustum on the far plane
  ftl = fc + Y * fh - X * fw;
  ftr = fc + Y * fh + X * fw;
  fbl = fc - Y * fh - X * fw;
  fbr = fc - Y * fh + X * fw;
  
  // compute the six planes
  // the function set3Points assumes that the points
  // are given in counter clockwise order
  pl[TOP].set(ntr,ntl,ftl);
  pl[BOTTOM].set(nbl,nbr,fbr);
  pl[LEFT].set(ntl,nbl,fbl);
  pl[RIGHT].set(nbr,ntr,fbr);
  pl[NEARP].set(ntl,ntr,nbr);
  pl[FARP].set(ftr,ftl,fbl);
}

//The function computed the eight corners of the frustum and the six planes according to the algorithm defined in the previous section.
//
//The following (more efficient) alternative may be used to replace the computation of the eight corners and the six planes in the function above.
//
//
//pl[NEARP].setNormalAndPoint(-Z,nc);
//pl[FARP].setNormalAndPoint(Z,fc);
//
//Vec3 aux,normal;
//
//aux = (nc + Y*nh) - p;
//aux.normalize();
//normal = aux * X;
//pl[TOP].setNormalAndPoint(normal,nc+Y*nh);
//
//aux = (nc - Y*nh) - p;
//aux.normalize();
//normal = X * aux;
//pl[BOTTOM].setNormalAndPoint(normal,nc-Y*nh);
//
//aux = (nc - X*nw) - p;
//aux.normalize();
//normal = aux * Y;
//pl[LEFT].setNormalAndPoint(normal,nc-X*nw);
//
//aux = (nc + X*nw) - p;
//aux.normalize();
//normal = Y * aux;
//pl[RIGHT].setNormalAndPoint(normal,nc+X*nw);

int FrustumG::pointInFrustum(V3t &p) {
  
  int result = INSIDE;
  for(int i=0; i < 6; i++) {
    
    if (pl[i].distance(p) < 0)
      return OUTSIDE;
  }
  return(result);
  
}

int FrustumG::sphereInFrustum(V3t &p, float radius) {
  
  float distance;
  int result = INSIDE;
  
  for(int i=0; i < 6; i++) {
    distance = pl[i].distance(p);
    if (distance < -radius)
      return OUTSIDE;
    else if (distance < radius)
      result =  INTERSECT;
  }
  return(result);
}

//int FrustumG::boxInFrustum(Box &b) {
//
//    int result = INSIDE, out,in;
//
//    // for each plane do ...
//    for(int i=0; i < 6; i++) {
//
//        // reset counters for corners in and out
//        out=0;in=0;
//        // for each corner of the box do ...
//        // get out of the cycle as soon as a box as corners
//        // both inside and out of the frustum
//        for (int k = 0; k < 8 && (in==0 || out==0); k++) {
//
//            // is the corner outside or inside
//            if (pl[i].distance(b.getVertex(k)) < 0)
//                out++;
//            else
//                in++;
//        }
//        //if all corners are out
//        if (!in)
//            return (OUTSIDE);
//        // if some corners are out and others are in
//        else if (out)
//            result = INTERSECT;
//    }
//    return(result);
//
//}

//int FrustumG::boxInFrustum(AABox &b) {
//
//    int result = INSIDE;
//    //for each plane do ...
//    for(int i=0; i < 6; i++) {
//
//        // is the positive vertex outside?
//        if (pl[i].distance(b.getVertexP(pl[i].normal)) < 0)
//            return OUTSIDE;
//        // is the negative vertex outside?
//        else if (pl[i].distance(b.getVertexN(pl[i].normal)) < 0)
//            result =  INTERSECT;
//    }
//    return(result);
//
//}

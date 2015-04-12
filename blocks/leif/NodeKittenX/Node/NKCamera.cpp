//
//  NKCamera.m
//  nike3dField
//
//  Created by Chroma Developer on 4/1/14.
//
//

#include "NKCamera.h"
#include "NKFrameBuffer.h"
#include "NKSceneNode.h"

NKCamera::NKCamera(NKSceneNode* scene){
    
    setScene(scene);
    setPosition(V3Make(scene->position().x, scene->position().y, scene->position().z - 1));
    name = make_shared<string>("CAMERA");
    
    _pDirty = true;
    
    aspect = scene->size().width / scene->size().height; // Use screen bounds as default
    nearZ = .01f;
    farZ = 10000.0f;
    fovVertRadians = DEGREES_TO_RADIANS(54);
    
    _upVector = V3Make(0, 1, 0);
    
    initGL();
}

void NKCamera::setTarget(NKNode *node){
    lookAtNode(node);
}

void NKCamera::lookAtNode(NKNode *node){
    target.removeAllActions();
    
#warning fix after ACTIONS PORTED
//    [_target runAction:[NKAction moveToFollowNode:node duration:1.] completion:^{
//     [_target repeatAction:[NKAction followNode:node duration:1.]];
//     }];
}

void NKCamera::setDirty(bool dirty) {
    NKNode::setDirty(dirty);
    _vpDirty = dirty;
    _vDirty = dirty;
    _pDirty = dirty;
}

M16t NKCamera::viewProjectionMatrix()
{
    if (_vpDirty) {
        _vpDirty = false;
        return _viewProjectionMatrix = M16Multiply(_projectionMatrix, _viewMatrix);
    }
    
    return _viewProjectionMatrix;
}

M16t NKCamera::viewMatrix() {
    if (_vDirty) {
        _viewMatrix = M16MakeLookAt(globalPosition(), target.globalPosition(), _upVector);
        _vDirty = false;
        return _viewMatrix;
    }
    return _viewMatrix;
}
//
//-(M16t)globalTransform {
//    return _localTransform;
//}

M16t NKCamera::projectionMatrix() {
    if (_pDirty) {
        _pDirty = false;
        if (projectionMode == NKProjectionModeOrthographic) {
            return _projectionMatrix = orthographicMatrix();
        }
        else {
            return _projectionMatrix = M16MakePerspective(fovVertRadians,
                                                         aspect,
                                                         nearZ,
                                                         farZ);
        }
    }
    return _projectionMatrix;
}

M16t NKCamera::orthographicMatrix() {
    return M16MakeOrtho(-scene()->size().width*.5, scene()->size().width*.5, -scene()->size().height*.5, scene()->size().height*.5, nearZ, farZ);
}

void NKCamera::initGL() {
    GetGLError();
    
    glEnable(GL_BLEND);
    
    GetGLError();
    
    scene()->setUsesDepth(true);
    
    glLineWidth(1.0f);
    
    GetGLError();
    
#if !(NK_USE_WEBGL || NK_USE_GLES)
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
    
#if !NK_USE_GL3
    glEnable(GL_MULTISAMPLE_ARB);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
#endif
    
    float gran;
    glGetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &gran);
    nkLog("smooth line gran: %f",gran);
#endif
    
    GetGLError();
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
//    return V2Make(p2.x, p2.y);
//    //return V2Make(10000, 10000);
//}
//
//-(P2t)screenPoint:(P2t)p InNode:(NKNode*)node {
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
//    return V2Make(p2.x, p2.y);
//    
//}

void NKCamera::updateCameraWithTimeSinceLast(F1t dt) {
    NKNode::updateWithTimeSinceLast(dt);
    target.updateWithTimeSinceLast(dt);
    //  NKLogV3(@"camera pos", self.globalPosition);
}

void NKCamera::updateWithTimeSinceLast(F1t dt){

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

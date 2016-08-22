//
//  SceneController.cpp
//  Pods
//
//  Created by Leif Shackelford on 5/31/15.
//
//

#include "view.h"
#include "../gl/gl.h"
#include "scene.h"

#pragma mark - DRAW

View::View(V2t size) : Node(V3t{size.x, size.y, 1}){
  _anchorPoint = V2(.5);

  modelViewMatrix.setPullFunction([this]{
    return M16Multiply(M16MakeLookAt({0,0,1}, {0,0,0}, {0,1,0}),M16ScaleWithV3(globalTransform.get(), this->size.get()));
  });
  modelViewProjectionMatrix.setPullFunction([this]{
    return M16Multiply(scene()->orthographicMatrix(), modelViewMatrix.get());
  });
}

void View::draw() {
  if (hidden()) return;

  if (_shouldRasterize){
    if (_framebuffer && !_framebuffer->dirty()) {
      drawFBO();
      return;
    }
  }

  if (_framebuffer) {
    pushFramebuffer();
  }
  else {
    if (_backgroundColor.visible()){
      Mesh::fillRect(scene(), getGlobalFrame(), _backgroundColor);
    }
  }

  customDraw();
  nkGetGLError();
  restoreGLStates();
  nkGetGLError();
  drawChildren();

  if (_framebuffer) {
    popFramebuffer();
    drawFBO();
  }
}

bool View::containsPoint(P2t location) const {

  // OLD METHOD
  // ADDING LOCAL TRANSFORMATION

  P2t p = location;
  //P2tp = [self transformedPoint:location];

  //NSLog(@"world coords: %f %f %f", p.x, p.y, p.z);

  R4t r = getGlobalFrame();

  //bool withinArea = false;
  if ( p.x > r.x && p.x < r.x + r.size.width && p.y > r.y && p.y < r.y + r.size.height)
  {
    // [self logCoords];
    return true;
  }
  return false;

  //    P2t p = [self inverseProjectedPoint:location];
  //
  //    V3t globalPos = [self globalPosition];
  //
  //    R4t r = R4Make(globalPos.x - size().x * _anchorPoint3d.x, globalPos.y - size().y *_anchorPoint3d.y, size().x, size().y);
  //
  //    //bool withinArea = false;
  //    if ( p.x > r.x && p.x < r.x + r.size.width && p.y > r.y && p.y < r.y + r.size.height)
  //    {
  //        // [self logCoords];
  //        return true;
  //    }
  //    return false;

}

//
//  SceneController.cpp
//  Pods
//
//  Created by Leif Shackelford on 5/31/15.
//
//

#include "View.h"
#include "ShaderProgram.h"
#include "Mesh.h"

void View::layoutSubviews() {
}

#pragma mark - DRAW

void View::draw() {
  if (hidden()) return;
  
  if (_shouldRasterize){
    if (framebuffer && !framebuffer->dirty) {
      drawFBO();
      return;
    }
  }
  
  if (framebuffer) {
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
  layoutSubviews();
  drawChildren();
  
  if (framebuffer) {
    popFramebuffer();
    drawFBO();
  }
}

void View::drawFBO() const {
  Mesh::fillRect(scene(), getGlobalFrame(), WHITE, framebuffer->renderTexture);
}

R4t View::getGlobalFrame() const {
  V3t g = globalPosition();
  return R4(g.x - size.get().x * _anchorPoint.x, g.y - size.get().y *_anchorPoint.y, size.get().x, size.get().y);
}

R4t View::getDrawFrame() const {
  return R4(-size.get().x * _anchorPoint.x, -size.get().y *_anchorPoint.y, size.get().x, size.get().y);
}

R4t View::calculateAccumulatedFrame() const {
  
  R4t rect = getDrawFrame();
  
  for (auto& child : children()) {
    
    auto view = (View*)child.get();
    
    R4t childFrame = view->getDrawFrame();
    
    if (childFrame.x < rect.x) {
      rect.x = childFrame.x;
    }
    
    if (childFrame.x + childFrame.size.width > rect.x + rect.size.width) {
      rect.size.width = rect.x + childFrame.x + childFrame.size.width;
    }
    
    if (childFrame.y < rect.y) {
      rect.y = childFrame.y;
    }
    
    
    if (childFrame.y + childFrame.size.height > rect.y + rect.size.height) {
      rect.size.height = rect.y + childFrame.y + childFrame.size.height;
    }
    
  }
  
  return rect;
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
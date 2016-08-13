//
//  View.h
//  Pods
//
//  Created by Leif Shackelford on 5/31/15.
//
//

#ifndef __View__
#define __View__

#include "Texture.h"
#include "Node.h"
#include "Mesh.h"

class View :
public Node, public Layout::Manager<View> {
  
  bool _shouldRasterize {false};
  V2t _anchorPoint;
  Color _backgroundColor;
  std::unique_ptr<FrameBuffer> _framebuffer;

public:
  // Convenience Constructors
  static std::shared_ptr<View> viewWithSize(V2t size){
    std::shared_ptr<View> ret (new View(size));
    return std::move(ret);
  }
  
  static std::shared_ptr<View> view(){
    std::shared_ptr<View> ret (new View(V2{1,1}));
    return std::move(ret);
  }
  
  View(V2t size);
  ~View(){};
  
  const std::unique_ptr<FrameBuffer>& framebuffer() const {return _framebuffer;}
  void setFramebuffer(std::unique_ptr<FrameBuffer> framebuffer) {_framebuffer = std::move(framebuffer);}

#pragma mark - Layout
  
  void afterResize() override {
    if (_framebuffer) {
      _framebuffer->size.set(size.get().xy);
    }
    afterTransform();
  }
  
  void afterTransform() override {
    globalTransform.setDirty();
    modelViewMatrix.setDirty();
    modelViewProjectionMatrix.setDirty();
    layout(this);
  }
  
  void afterChildrenChanged() override{
    if (_framebuffer) {
      _framebuffer->setDirty();
    }
  }
  
  void draw() override;
  
  bool containsPoint(P2t location) const;
  
#pragma mark - SETTERS / GETTERS
  
  View* setBackgroundColor(Color backgroundColor){
    _backgroundColor = backgroundColor;
    return this;
  }
  Color backgroundColor(){
    return _backgroundColor;
  }
  
  View* setShouldRasterize(bool rasterize){
    _shouldRasterize = rasterize;
    
    if (_shouldRasterize && !_framebuffer){
      _framebuffer.reset(new FrameBuffer(size.get().xy));
    }
    
    return this;
  }
  
#pragma mark - FRAME METHODS
  
  void drawFBO() const {
    Mesh::fillRect(scene(), getGlobalFrame(), WHITE, _framebuffer->renderTexture);
  }
  
  R4t getGlobalFrame() const {
    V3t g = globalPosition();
    return R4(g.x - size.get().x * _anchorPoint.x, g.y - size.get().y *_anchorPoint.y, size.get().x, size.get().y);
  }
  
  R4t getDrawFrame() const {
    return R4(-size.get().x * _anchorPoint.x, -size.get().y *_anchorPoint.y, size.get().x, size.get().y);
  }
  
  R4t calculateAccumulatedFrame() const {
    
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

  
#pragma mark - FRAMEBUFFER
  
  void pushFramebuffer() {
    _framebuffer->bind();
    if (_backgroundColor.visible()){
      _framebuffer->clear(_backgroundColor);
    }
    else {
      _framebuffer->clear();
    }
  }
  
  void popFramebuffer() {
    if (_shouldRasterize) {
      _framebuffer->setDirty(false);
    }
    _framebuffer->unbind();
  }
  
#pragma mark - ANCHOR METHODS
  
  void setAnchorPoint(V2t anchorPoint){
    _anchorPoint.x = anchorPoint.x;
    _anchorPoint.y = anchorPoint.y;
  }
  void setAnchorPoint(F1t x, F1t y){
    _anchorPoint.x = x;
    _anchorPoint.y = y;
  }
  
  V2t anchorPoint(){
    return _anchorPoint;
  }
  
};

#endif /* defined(__Pods__SceneController__) */
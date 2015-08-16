//
//  SceneController.h
//  Pods
//
//  Created by Leif Shackelford on 5/31/15.
//
//

#ifndef __View__
#define __View__

#include "Texture.h"
#include "Node.h"

class View : public Layout<Node> {
  
  bool _shouldRasterize {false};
  V2t _anchorPoint;
  Color _backgroundColor;

public:
  // Convenience Constructors
  static std::shared_ptr<View> viewWithSize(V2t size){
    std::shared_ptr<View> ret (new View(size));
    return ret;
  }
  
  View(V2t size) : Layout(V3t{size.x, size.y, 1}){
    _anchorPoint = V2(.5);
  }
  ~View(){};
  
  // Public Properties
  std::unique_ptr<FrameBuffer> framebuffer;
  
#pragma mark - Layout
  
  void afterTransform() override{
    Node::afterTransform();
    if (framebuffer) {
      framebuffer.reset();
      framebuffer = std::make_unique<FrameBuffer>(size.get().xy);
    }
  }
  
  void afterChildrenChanged() override{
    if (framebuffer) {
      framebuffer->dirty = true;
    }
  }
  
  void draw() override;
  void drawFBO() const;
  virtual void layoutSubviews();
  
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
    
    if (_shouldRasterize && !framebuffer){
      framebuffer = std::make_unique<FrameBuffer>(size.get().xy);
    }

    return this;
  }
  
#pragma mark - FRAME METHODS
  
  R4t getDrawFrame() const;
  R4t getGlobalFrame() const;
  R4t calculateAccumulatedFrame() const;
  
#pragma mark - FRAMEBUFFER
  
  void pushFramebuffer() {
    framebuffer->bind();
    if (_backgroundColor.visible()){
      framebuffer->clear(_backgroundColor);
    }
    else {
      framebuffer->clear();
    }
  }
  
  void popFramebuffer() {
      if (_shouldRasterize) {
        framebuffer->dirty = false;
      }
      framebuffer->unbind();
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
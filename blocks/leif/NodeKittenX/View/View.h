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

class View :
public Node {
  
  bool _shouldRasterize {false};
  V2t _anchorPoint;
  Color _backgroundColor;
  std::unique_ptr<Layout::Manager<View>> _layoutManager;
  std::unique_ptr<FrameBuffer> _framebuffer;

public:
  // Convenience Constructors
  static std::shared_ptr<View> viewWithSize(V2t size){
    std::shared_ptr<View> ret (new View(size));
    return ret;
  }
  
  static std::shared_ptr<View> view(){
    std::shared_ptr<View> ret (new View(V2{1,1}));
    return ret;
  }
  
  View(V2t size);
  ~View(){};
  
  // Public Properties
  const std::unique_ptr<Layout::Manager<View>>& layoutManager() const {return _layoutManager;}
  
  Layout::Manager<View>* setLayoutMethod(Layout::Method layoutMethod) {
    if (!_layoutManager){
      _layoutManager.reset(new Layout::Manager<View>());
    }
    return _layoutManager->setLayoutMethod(layoutMethod);
  }
  
  const std::unique_ptr<FrameBuffer>& framebuffer() const {return _framebuffer;}
  void setFramebuffer(std::unique_ptr<FrameBuffer> framebuffer) {_framebuffer = std::move(framebuffer);}
  
#pragma mark - Layout
  
  void afterTransform() override {
    globalTransform.setDirty();
    modelViewMatrix.setDirty();
    modelViewProjectionMatrix.setDirty();
    
    if (_framebuffer) {
      _framebuffer.reset(new FrameBuffer(size.get().xy));
    }
    if (_layoutManager){
//      Node* rec = this;
//      while (rec->parent()){
//        printf("- ");
//        rec = rec->parent();
//      }
//      printf("layout %p \n", this);
      
      _layoutManager->layout(this);
    }
  }
  
  void afterChildrenChanged() override{
    if (_framebuffer) {
      _framebuffer->setDirty();
    }
  }
  
  void draw() override;
  void drawFBO() const;
  
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
  
  R4t getDrawFrame() const;
  R4t getGlobalFrame() const;
  R4t calculateAccumulatedFrame() const;
  
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
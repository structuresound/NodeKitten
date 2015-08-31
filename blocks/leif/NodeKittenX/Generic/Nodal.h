#pragma once

template <class T>
class Nodal {
  std::vector<std::shared_ptr<T>> _children;
  T* _parent = nullptr;
  int _siblingPosition;
  
public:
  
  virtual T* parent() const {return _parent;}
  
  const std::vector<std::shared_ptr<T>>& children() const {
    return _children;
  }
  
  const std::shared_ptr<T>& child() const {
    return _children[0];
  }
  
  template<class C>
  C* addChild(std::shared_ptr<C>& child) {
    addChild(std::move(child));
    return child.get();
  }
  
  template<class C>
  C* addChild(std::shared_ptr<C>&& child) {
    child->_siblingPosition = (int)_children.size();
    _children.push_back(child);
    child->setParent(static_cast<T*>(this));
    afterChildrenChanged();
    return static_cast<C*>(child.get());
  }
  
  template<class C>
  C* insertChild(std::shared_ptr<C>& child, int atIndex){
    if (!_children.size()) {
      addChild(child);
    }
    else {
      child->_siblingPosition = atIndex;
      child->setParent(this);
      _children.insert(_children.begin() + atIndex, child);
      afterChildrenChanged();
    }
    return child.get();
  }
  
  int siblingPosition() const {return _siblingPosition;};
  
  T* childAtSiblingPosition(U1t position) const {
    if (_children.size() > position){
      return _children[position];
    }
    return nullptr;
  }
  
  T* nextSibling() const {
    if (_parent){
      auto next = _parent->childAtSiblingPosition(_siblingPosition+1);
      if (next) return next;
      return _parent->_children.front();
    }
    nkError("asking for next sibling without parent");
    return this;
  };
  
  T* previousSibling() const {
    if (_parent){
      if (_siblingPosition-1 > 0){
        auto next = _parent->childAtSiblingPosition(_siblingPosition-1);
        if (next) return next;
      }
      return _parent->_children.back();
    }
    nkError("asking for next sibling without parent");
    return this;
  };
  
  virtual void afterChildrenChanged(){
  }
  
  
  virtual T* setParent(T *parent){
    if (_parent) {
      beforeParentChanged();
      _parent->removeChild(static_cast<T*>(this));
      _parent = parent;
      afterParentChanged();
    }
    else {
      _parent = parent;
    }
    return static_cast<T*>(this);
  }
  
  virtual void beforeParentChanged(){};
  virtual void afterParentChanged(){};
  virtual void beforeRemove(){
    removeAllChildren();
  }
  virtual void afterRemove(){};
  
  virtual void removeFromParent(){
    if (_parent){
      beforeRemove();
      _parent->removeChild(static_cast<T*>(this));
      afterRemove();
    }
  }
  
  void removeChildren(std::vector<std::unique_ptr<T>> children){
    _::subtract(_children, children);
    afterChildrenChanged();
  }
  
  void sortChildren(std::function<bool(const std::shared_ptr<T>&,const std::shared_ptr<T>&)>&& func){
    sort(_children.begin(), _children.end(),func);
  }
  
  void removeAllChildren(){
    for (auto child : _children) {
      child->beforeRemove();
      child->afterRemove();
    }
    _children.clear();
  }
  
  void removeChild(T* child){
    _::eraseSmartFromRaw(_children, child);
    afterChildrenChanged();
  }
  
  void removeChildNamed(std::string& name){
    childNamed(name)->removeFromParent();
  }
  
  T* childNamed(std::string& name){
    for (auto& n : _children) {
      if (n->name == name) {
        return n.get();
      }
    }
    return nullptr;
  }
  
  T* randomChild(){
    if (!_children.size()) {
      return this;
    }
    return _children[random() % _children.size()].get();
  }
  
  // THIS GETS EXPENSIVE WITH A LOT OF NODES

  std::vector<T*> allChildren(){
    std::vector<T*> allChildren;
    
    for (auto& child : _children){
      allChildren.push_back(child.get());
      _::append(allChildren, child->allChildren());
    }
    
    return allChildren;
  }
  
  T* randomLeaf(){
    if (_children.size()) {
      return randomChild()->randomLeaf();
    }
    return this;
  }
  
};
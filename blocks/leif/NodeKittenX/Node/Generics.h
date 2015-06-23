//
//  Generics.h
//  Pods
//
//  Created by Leif Shackelford on 6/22/15.
//
//

#ifndef __Pods__Generics__
#define __Pods__Generics__

#include "UXEvent.h"
#include "GLState.h"

template <typename T>
class SizeProperty {
protected:
    T _size;
public:
    void setSize(const T size){_size = size; afterSetSize();};
    T size() const {return _size;};
    virtual void afterSetSize(){};
};

template <class T>
class Nodal {
    vector<shared_ptr<T>> _children;
    T* _parent = nullptr;
    
public:
    
    T* parent();
    
    vector<shared_ptr<T>> children() {
        return _children;
    }
    
    shared_ptr<T> child() {
        return _children[0];
    }
    
    virtual void addChild(shared_ptr<T> child) {
        child->siblingPosition = (int)_children.size();
        child->setParent(this);
        _children.push_back(child);
        afterChildrenChanged();
    }
    
    void insertChild(shared_ptr<T> child, int atIndex){
        if (!_children.size()) {
            addChild(child);
        }
        else {
            child->siblingPosition = atIndex;
            child->setParent(this);
            _children.insert(_children.begin() + atIndex, child);
            afterChildrenChanged();
        }
    }
    
    virtual void afterChildrenChanged(){}
    
    void setParent(T *parent){
        if (_parent) {
            beforeChangeParent();
            V3t p = globalPosition();
            _parent->removeChild(shared_from_this());
            _parent = parent;
            afterChangeParent();
            setGlobalPosition(p);
        }
        else {
            _parent = parent;
        }
        
        if (_userInteractionEnabled && _parent) {
            _parent->setUserInteractionEnabled(true);
        }
        
        setDirty();
    }
    
    virtual void beforeRemove(){};
    virtual void afterRemove(){};
    
    virtual void beforeChangeParent(){};
    
    void removeFromParent(){
        beforeRemove();
        removeAllChildren();
        
        if (_parent){
            _parent->removeChild(this);
        }
        afterRemove();
    }
    
    void removeChildrenInArray(vector<shared_ptr<T>> children){
        _c::subtract(_children, children);
    }
    
    void removeAllChildren(){
        auto copy = _children;
        
        for (auto child : copy) {
            child->removeFromParent();
        }
        
        _children.clear();
    }
    
    void removeChild(T* child){
        _c::erase(_children, child);
    }
    
    void removeChildNamed(string& name){
        childNamed(name)->removeFromParent();
    }
    
    T* childNamed(string& name){
        for (auto n : _children) {
            if (n->name == name) {
                return n;
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
    
    vector<T*> allChildren(){
        vector<T*> allChildren;
        
        for (auto child : _children){
            allChildren.push_back(child.get());
            for (auto rec : child->allChildren()){
                allChildren.push_back(rec.get());
            }
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

#endif /* defined(__Pods__Generics__) */

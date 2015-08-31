#pragma once

#include "CommonTypes.h"

namespace Reactive {
  
  using std::function;
  
  template<typename T>
  class Pullable {
  protected:
    mutable bool _dirty;
    mutable T _cache;
    std::function<T()> pull;
  public:
    Pullable() : _dirty{true} { }
    
    Pullable(const T &initial) : _cache(initial), _dirty{false} { }
    
    virtual ~Pullable() {
    }
    
    void setPullFunction(std::function<T(void)> &&func) {
      pull = func;
    }
    
    T get() const {
      if (!_dirty) return _cache;
      Pullable::set(pull());
      return _cache;
    };
    
    T ref() const {
      return _cache;
    }
    
    virtual void set(const T &value) const {
      _cache = value;
      _dirty = false;
    };
    
    void setDirty(bool dirty = true) {
      _dirty = dirty;
    };
  };
  
  
  template<typename T>
  class Variable : public Pullable<T> {
    std::vector<function<void(void)>> afterFuncs;
  public:
    void afterSet(function<void(void)> &&callback) {
      afterFuncs.push_back(callback);
    }
    
    void set(const T &value) const override {
      Pullable<T>::set(value);
      for (auto &callback : afterFuncs) {
        callback();
      }
    }
    
    virtual ~Variable() {
    }
  };
  
  
  template<typename T>
  class ObserverHandle;
  
  template<typename T>
  class Observable : public Pullable<T> {
    using Pullable<T>::Pullable;
    std::vector<ObserverHandle<T> *> observers;
  public:
    void set(const T &value) const override;
    
    void addObserver(ObserverHandle<T> *observer) {
      observers.push_back(observer);
    }
    
    void removeObserver(ObserverHandle<T> *observer) {
      _::erase(observers, observer);
    }
  };
  
  template<typename T>
  class PropertyObserver {
    std::vector <std::unique_ptr<ObserverHandle<T>>> observers;
  public:
    void addObserver(const Observable<T> &property, function<void(T)> &&observeCallback) {
      observers.emplace_back(property, observeCallback);
    }
  };
  
  template<typename T>
  class ObserverHandle {
    Observable<T> &_property;
    function<void(T)> _observeCallback;
  public:
    ObserverHandle(const Observable<T> &property, function<void(T)> observeCallback) :
    _property(property),
    _observeCallback(observeCallback) {
      _property->addObserver(this);
    }
    
    ~ObserverHandle() {
      _property->removeObserver(this);
    }
    
    void observe(T changedValue) {
      _observeCallback(changedValue);
    }
  };
  
  template<typename T>
  void Observable<T>::set(const T &value) const {
    Pullable<T>::set(value);
    for (auto &p : observers) {
      p->observe(Pullable<T>::get());
    }
  }
  
  template<typename T>
  class Vector : public Variable<T> {
  public:
    void setX(F1t elem) {
      T ex = Pullable<T>::_cache;
      ex.v[0] = elem;
      Pullable<T>::set(ex);
    };
    
    void setY(F1t elem) {
      T ex = Pullable<T>::_cache;
      ex.v[1] = elem;
      Pullable<T>::set(ex);
    };
    
    void setZ(F1t elem) {
      T ex = Pullable<T>::_cache;
      ex.v[2] = elem;
      Pullable<T>::set(ex);
    };
  };
  
  template<>
  void Vector<F1t>::setY(float position) = delete;
  
  template<>
  void Vector<F1t>::setZ(float position) = delete;
  
  template<>
  void Vector<V2>::setZ(float position) = delete;
  
}

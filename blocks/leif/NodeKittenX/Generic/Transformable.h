#pragma once

template <typename T>
class Sizeable {
protected:
public:
  Reactive::Variable<T> size;
  virtual void afterResize() = 0;
  Sizeable(){
    size.afterSet([this]{
      afterResize();
    });
  }
  virtual ~Sizeable(){};
};

//template <typename T>
//class Positionable {
//public:
//    ReactiveVariable<T> position;
//    virtual void afterReposition() = 0;
//    Positionable(){
//        position.afterSet([this]{
//            afterReposition();
//        });
//    }
//    virtual ~Positionable(){};
//};

template <class Derived, typename T>
class Transformable {
  
public:
  Reactive::Variable<T> position;
  Reactive::Variable<T> scale;
  Reactive::Variable<Q4> orientation;
  Reactive::Variable<M16> transform;
  V3t _upVector;
  
  Transformable(){
    transform.setPullFunction([this]{
      M16 ret = M16Multiply(M16MakeScale(scale.ref()), M16MakeRotate(orientation.ref()));
      M16SetV3Translation(&ret, position.ref());
      return ret;
    });
    transform.afterSet([this]{
      scale.setDirty();
      position.setDirty();
      afterTransform();
    });
    position.setPullFunction([this]{
      return transform.get().translation;
    });
    position.afterSet([this]{
      transform.setDirty();
      afterTransform();
    });
    scale.setPullFunction([this]{
      return V3GetM16Scale(transform.get());
    });
    scale.afterSet([this]{
      transform.setDirty();
      afterTransform();
    });
    orientation.setPullFunction([this]{
      return Q4FromMatrix(transform.get());
    });
    orientation.afterSet([this]{
      transform.setDirty();
      afterTransform();
    });
  }
  virtual ~Transformable(){};
  virtual void afterTransform() = 0;
  
#pragma mark - ORIENTATION METHODS
  
  void setOrientationEuler(const V3t eulerAngles){
    orientation.set(Q4FromV3(eulerAngles));
  }
  
  F1t getYOrientation() {
    V3t raw = V3FromQ4(Q4GetM16Rotate(transform.get()));
    return raw.x < 0 ? -raw.y : raw.y;
  }
  
  void setZRotation(F1t rotation){
    orientation.set(Q4FromAngleAndV3(rotation, V3(0,0,1)));
  }
  
  void rotateMatrix(M16t matrix){
    M16t m = M16Multiply(M16MakeScale(scale.get()),matrix);
    M16SetV3Translation(&m, position.get());
    transform.set(m);
  }
  
  M16t getLookMatrix(V3t lookAtPosition){
    return M16MakeLookAt(position.get(), lookAtPosition, upVector());
  }
  
  void lookAtPoint(V3t point){
    rotateMatrix(getLookMatrix(point));
  }
  
  V3t upVector() const {return _upVector;};
  
private:
  Derived& self() { return *static_cast<Derived*>(this); }
  const Derived& self() const { return *static_cast<const Derived*>(this); }
  
};

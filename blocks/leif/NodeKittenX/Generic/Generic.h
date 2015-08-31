#pragma once

#include "Reactive.h"
#include "Transformable.h"
#include "Drawable.h"
#include "Nodal.h"
#include "Layout.h"

template <class T>
class CountedClass
{
  static unsigned int createdObjects;
  static unsigned int destroyedObjects;
public:
  static unsigned int liveObjects(){return createdObjects - destroyedObjects;};
  CountedClass<T>(){
    createdObjects++;
  }
  virtual ~CountedClass<T>(){
    createdObjects--;
  }
};

template<typename T>
unsigned int CountedClass<T>::createdObjects = 0;

template<typename T>
unsigned int CountedClass<T>::destroyedObjects = 0;
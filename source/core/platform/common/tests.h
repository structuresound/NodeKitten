#include "../../core.h"

template <typename T>
class DynamicProperty {
  T _specializedMember;
public:
  T specializedMember(){
    return _specializedMember;
  }
};

class Base {
  bool _dataMember;
public:
  virtual void log(){
    nkLog("hello derived");
  }
};

class DerivedA : public Base, public DynamicProperty<int> {
public:
  void log() override {
    nkLog("hello derived A, property: %d", specializedMember());
  }
};

class DerivedB : public Base, public DynamicProperty<string>{
public:
  void log() override {
    nkLog("hello derived B, property: %s", specializedMember());
  }
};

class Tests {
  vector<unique_ptr<Base>> items;
  
public:
  Tests(){
    nkLog("*** COMPILER TESTS ***");
    testInheritance();
    
    nkLog("*** FRAMEWORK TESTS ***");
    testKiwi();
    logClassSizes();
  }
  
  static Tests* tests;
  
  template<typename T>
  void add(T&& item){
    items.push_back(move(item));
  };
  
  void testInheritance();
  void logClassSizes();
  void testKiwi();
};
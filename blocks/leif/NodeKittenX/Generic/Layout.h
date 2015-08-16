#ifndef __Layout__
#define __Layout__

#include "Reactive.h"
#include "kiwi.h"

// non-raw literal operator
constexpr double operator"" _percent( long double val) { return val / 100 ; }

//namespace underscore {
//    template<class Container, class Function>
//    void each(Container container, Function fn)
//    {
//      auto first = cbegin(container);
//      auto last = cend(container);
//
//      while (first!=last) {
//        fn (*first);
//        ++first;
//      }
//    }
//}

typedef enum : U1t {
  LayoutManual,
  LayoutContainer,
  LayoutRow,
  LayoutColumn,
  LayoutCollection
} LayoutMethod;

enum class ConstraintType {size, margin};

class LayoutUnit : public Reactive::Observable<F1t> {
  using Reactive::Observable<F1t>::Observable;
};

class LayoutUnitPixel : public LayoutUnit {
};

class LayoutUnitPercent : public LayoutUnit {
public:
  class DoubleIsPercent{}; // a tag
  explicit LayoutUnitPercent(DoubleIsPercent, double pct) : LayoutUnit(pct/100.0) {}
};

static LayoutUnitPercent operator "" _pct( long double pct )
{
  return LayoutUnitPercent{LayoutUnitPercent::DoubleIsPercent{}, static_cast<double>(pct)};
}

class Constraint {
  ConstraintType _type;
  std::vector<LayoutUnit> constraints;

public:
  const ConstraintType type() const {return _type;}
  void setType(ConstraintType type) {_type = type;}

  F1t solve(){

  }


};



class Solver {
  std::vector<Constraint> constraints;
  kiwi::Solver kSolver;
  kiwi::Variable x {"x"};
  kiwi::Variable y {"y"};

public:

  Solver(){

    // x = 20
    kSolver.addConstraint(x == 20);

    // x + 2 == y + 10
    kSolver.addConstraint(x + 2 == y + 20);
    // these all have strength 'required

    kSolver.updateVariables();

    nkLog("x + %f + y + %f", x.value(), y.value());

// x == 20
// y == 12

  };
  ~Solver(){};

  F1t solve(){

  }
};

template<typename T>
class Layout : public T {

  bool horizontalMajor {false};

  T* _selectedElement;
  T* _element;

  V4 anchor;

  V2 Offset;
  LayoutMethod _layoutMethod {LayoutContainer};

  Constraint _constraint;

public:
  using T::T;

  void layout(){
//    _::each(this->children(), [](auto& child){
//        nkLog(child->);
//    });
  };

  const T* selectedElement() const {return _selectedElement;}
  void setSelectedElement(T* selectedElement) {_selectedElement = selectedElement;}

  const LayoutMethod layoutMethod() const {return _layoutMethod;}
  void setLayoutMethod(LayoutMethod layoutMethod) {_layoutMethod = layoutMethod;}

  // LAYOUT
  virtual V2 sizeForViewAtIndex(IndexPath indexPath){

  }
  virtual V2 positionForViewAtIndexPath(IndexPath indexPath, T* view){

  }
  virtual V2 spacingForViewAtIndexPath(IndexPath indexPath, T* view){

  }

  // SCROLLING
  virtual V2 targetPositionForEstimatedPosition(V2 position){

  }

};

#endif
#ifndef __Layout__
#define __Layout__

#include "Reactive.h"
#include "kiwi.h"

// non-raw literal operator
constexpr double operator"" _pct( long double val) { return val / 100.0f ; }

namespace Layout {
  
  enum class Method {
    None,
    Horizontal,
    Vertical,
    Container,
    Row,
    Column,
    Collection
  };
  
  enum class Unit {
    none,
    Pixels,
    Percent,
    ConstrainPixels,
    ConstrainPct
  };
  
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
  class Delegate {
    
  };
  
  template<typename T>
  class Manager {
    std::vector<T*> _selectedElements;
    
    Delegate<T>* _delegate;
    
    bool _descending {true};
    V2 _anchorPoint {0,0};
    V2 _offset {0,0};
    V4 _spacing {0,0,0,0};
    
    Method _method {Method::None};
    Unit _unit {Unit::Percent};
    Constraint _constraint;
    
  public:
    const Method layoutMethod() const {return _method;}
    Manager<T>* setLayoutMethod(Method layoutMethod) {_method = layoutMethod; return this;}
    
    const V2 anchorPoint() const {return _anchorPoint;}
    Manager<T>* setAnchorPoint(V2 anchorPoint) {_anchorPoint = anchorPoint; return this;}
    
    const V2 spacing() const {return _spacing;}
    Manager<T>* setSpacing(V2 spacing) {_spacing = spacing; return this;}
    
    // LAYOUT
    virtual V2 sizeForViewAtIndex(IndexPath indexPath){
      return {0,0};
    }
          
    virtual V2 positionForViewAtIndexPath(IndexPath indexPath, T* view){
      return {0,0};
    }
    virtual V2 spacingForViewAtIndexPath(IndexPath indexPath, T* view){
      return {0,0};
    }
    
    virtual int numberOfSections(){
      return 1;
    }
    
    virtual int numberOfChildrenInSection(const unsigned int section){
      return 0;
    }
    
    // SCROLLING
    virtual V2 targetPositionForEstimatedPosition(V2 position){
      return {0,0};
    }
    
    void staticLayout(T* element, bool vertical, bool descending){
      auto invert = descending ? -1.0f : 1.0f;
      
      auto spacing = V2{_spacing.left + _spacing.right, _spacing.top + _spacing.bottom};
      
      auto origin = element->size.get().xy * (_anchorPoint - (V2{.5,.5} * invert));
      
      V2 csize;
      
      if (vertical) {
        csize = (element->size.get().xy * V2{1.0f, 1.0f / element->children().size()}) - spacing;
        origin += (csize * .5) * invert;
        if (descending) origin.y -= _spacing.top;
        else origin.y += _spacing.bottom;
        origin.x -= _spacing.right;
      }
      else {
        csize = (element->size.get().xy * V2{1.0f / element->children().size(), 1.0f}) - spacing;
        origin += (csize * .5) * invert;
        if (descending) origin.x -= _spacing.right;
        else origin.x += _spacing.left;
        origin.y += _spacing.bottom;
      }
      
      auto step = (csize + spacing) * invert;
      
      if (vertical) step.x = 0;
      else step.y = 0;

      for (auto& child : element->children()){
        child->position.set(origin);
        child->size.set(csize);
        origin += step;
      }
    }
    
    virtual void layout(T* element) {
      switch (_method){
        case Method::None: default:
          for (auto& child : element->children()){
            child->afterTransform();
          }
        case Method::Vertical:
          staticLayout(element, true, true);
          break;
        case Method::Horizontal:
          staticLayout(element, false, false);
          break;
      }
    };
    
  };
  
}

#endif
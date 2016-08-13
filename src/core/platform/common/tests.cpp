//
//  Testing.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/25/15.
//
//

#include "tests.h"

Tests* Tests::tests;

void Tests::testInheritance(){

  nkLog("template inheritance: ");
  auto lvalue = make_unique<Base>();

  add(lvalue);
  add(make_unique<DerivedA>());
  add(make_unique<DerivedB>());

  for (auto& item : items){
    item->log();
  }
}

void Tests::testKiwi(){
  nkLog("*** KIWI + CONSTRAINTS ***");
  {
    kiwi::Solver solver;
    kiwi::Variable x {"x"};
    kiwi::Variable y {"y"};

    kiwi::Variable width {"width"};

    // x = 20
    solver.addConstraint(x == 20);
    // x + 2 == y + 10
    solver.addConstraint(x + 2 == y + 10);

    // y should be 12
    nkAssert((y == 12), "should be 12");

    solver.updateVariables();

    nkLog("x = %f, y = %f", x.value(), y.value());
  }
  // 2
  {
    // 0|-------------| x1 ----- xm ----- x2 |-----------------------|100
    auto x1 = kiwi::Variable{"x1"};
    auto x2 = kiwi::Variable{"x2"};
    auto xm = kiwi::Variable{"xm"};

    auto constraints = vector<kiwi::Constraint>{
      x1 >= 0,
      x2 <= 100,
      x2 >= x1 + 10,
      xm == (x1 + x2) / 2,
    }; // these all have strength 'required'

    kiwi::Solver solver;
    for (auto& cn : constraints){
      solver.addConstraint(cn);
    }

    solver.addEditVariable(xm, 1.0);

    for (auto val : {-20, 0, 20, 50, 80, 100, 140}){
      solver.suggestValue(xm, val);
      solver.updateVariables();
      nkLog("x1: %f", x1.value());
      nkLog("x2: %f", x2.value());
      nkLog("xm: %f", xm.value());
      nkLog("suggested xm: %d", val);
    }

  }
}

void Tests::logClassSizes() {
  nkLog("*** STRUCT + CLASS SIZES ***");
  nkLog("... Types            _____");
  nkLog("bool                 %d", sizeof(bool));
  nkLog("U1                   %d", sizeof(U1t));
  nkLog("F1                   %d", sizeof(F1t));
  nkLog("V3                   %d", sizeof(V3t));
  nkLog("M16                  %d", sizeof(M16t));
  nkLog("function M16t(void)  %d", sizeof(function<M16t()>));
  nkLog("vector<M16t>         %d", sizeof(vector<M16t>));
  nkLog("... Components       _____");
  nkLog("Reactive::Pullable<M16>%d", sizeof(Reactive::Pullable<M16>));
  nkLog("Reactive::Variable<M16>%d", sizeof(Reactive::Variable<M16>));
  nkLog("Nodal<Node>          %d", sizeof(Nodal<Node>));
  nkLog("Transformable<V3>    %d", sizeof(Transformable<Node,V3>));
  nkLog("Sizeable<V3>:        %d", sizeof(Sizeable<V3>));
  nkLog("CountedClass<Node>   %d", sizeof(CountedClass<Node>));
  nkLog("... Classes          _____");
  nkLog("shared_ptr<Node>     %d", sizeof(shared_ptr<Node>));
  nkLog("Node:                %d", sizeof(Node));
  nkLog("View:                %d", sizeof(View));
  nkLog("Texture:             %d", sizeof(Texture));
  nkLog("Shader:              %d", sizeof(Shader::Program));
  nkLog("FrameBuffer:         %d", sizeof(FrameBuffer));
  nkLog("Mesh:                %d", sizeof(Mesh));
};

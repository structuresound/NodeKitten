//
//  LayoutExample.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/19/15.
//
//

#include "LayoutExample.h"

void LayoutExample::moveToView() {
  setLayoutMethod(Layout::Method::Vertical);
  
  addChild(View::view())->setBackgroundColor(Color("red"));
  addChild(View::view())->setBackgroundColor(Color("orange"));
  addChild(View::view())->setBackgroundColor(Color("yellow"));
  addChild(View::view())->setBackgroundColor(Color("green"));
  addChild(View::view())->setBackgroundColor(Color("blue"));
  auto row = addChild(View::view())->setBackgroundColor(Color("violet"));
  
  row->setLayoutMethod(Layout::Method::Horizontal);
  row->addChild(View::view())->setBackgroundColor(Color("red"));
  row->addChild(View::view())->setBackgroundColor(Color("orange"));
  row->addChild(View::view())->setBackgroundColor(Color("yellow"));
  row->addChild(View::view())->setBackgroundColor(Color("green"));
  row->addChild(View::view())->setBackgroundColor(Color("blue"));
  auto col = row->addChild(View::view())->setBackgroundColor(Color("violet"));
  
  col->setLayoutMethod(Layout::Method::Vertical);
  col->addChild(View::view())->setBackgroundColor(Color("red"));
  col->addChild(View::view())->setBackgroundColor(Color("orange"));
  col->addChild(View::view())->setBackgroundColor(Color("yellow"));
  col->addChild(View::view())->setBackgroundColor(Color("green"));
  col->addChild(View::view())->setBackgroundColor(Color("blue"));
  col->addChild(View::view())->setBackgroundColor(Color("violet"));
}

void LayoutExample::handleUXEvent(shared_ptr<UXEvent> event){
  
}
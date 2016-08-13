//
//  ContainerView.h
//  Pods
//
//  Created by Leif Shackelford on 8/10/15.
//
//

#ifndef __Pods__ContainerView__
#define __Pods__ContainerView__

#include "ScrollView.h"

class ContainerView : public ScrollView {
  
public:
  ContainerView(V2 size) : ScrollView(1) {}
  ~ContainerView(){};
  
  static std::shared_ptr<ContainerView> viewWithSize(V2t size) {
    std::shared_ptr<ContainerView> ret (new ContainerView(size));
    return ret;
  }
  
  ContainerView* addRow(V2 vunits){
    return addLayout(vunits, Layout::Method::Row);
  }

  ContainerView* addColumn(V2 vunits){
    return addLayout(vunits, Layout::Method::Column);
  }

  ContainerView* addLayout(V2 vunits, Layout::Method l){
    auto child = std::make_shared<ContainerView>(vunits);
    child->setLayoutMethod(l);
    return static_cast<ContainerView*>(addChild(child));
  }
  
};
#endif /* defined(__Pods__ContainerView__) */

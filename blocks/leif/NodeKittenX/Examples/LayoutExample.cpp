//
//  LayoutExample.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/19/15.
//
//


#include "LayoutExample.h"

void LayoutExample::moveToView() {
    auto container = ContainerView::viewWithSize(size.get().xy);
    container
            ->addRow(V2{.5, .5})
            ->addColumn(V2{.5, .5});
    container->addRow(V2{.5, .5})->setBackgroundColor(Color{.3, .3, .3});
    addChild(container);
}

void LayoutExample::handleUXEvent(shared_ptr<UXEvent> event){

}
//
//  CollectionView.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/16/15.
//
//

#include "CollectionView.h"

shared_ptr<CollectionView> CollectionView::viewWithSize(V2 size){
    return make_shared<CollectionView>(size);
}

void CollectionView::updateWithTimeSinceLast(F1t dt){
    if (_dirty) {
        layout();
    }
    View::updateWithTimeSinceLast(dt);
}

void CollectionView::scrollToItemAtIndex(U1 index){}
void CollectionView::scrollToView(View *view){}

void CollectionView::didScroll(U1 index){}
void CollectionView::didFinishScrolling(){}

// GENERATION

View* CollectionView::createViewForItemAtIndex(U1 index){return nullptr;}

// LAYOUT

void CollectionView::animateChanges(){}

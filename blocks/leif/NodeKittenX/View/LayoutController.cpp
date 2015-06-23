//
//  LayoutController.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/16/15.
//
//

#include "LayoutController.h"

// OPTIONAL
V2 LayoutController::sizeForViewAtIndex(IndexPath indexPath){return V2{1};}
V2 LayoutController::targetPositionForEstimatedPosition(V2 position){return V2{1};}
V2 LayoutController::positionForViewAtIndexPath(IndexPath indexPath, View* view){return V2{1};}
V2 LayoutController::spacingForViewAtIndexPath(IndexPath indexPath, View* view){return V2{1};}

void layout(){
    
}
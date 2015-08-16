//
//  ScrollView.h
//  Pods
//
//  Created by Leif Shackelford on 6/22/15.
//
//

#ifndef __Pods__ScrollView__
#define __Pods__ScrollView__

#include "View.h"
#include "Layout.h"

class ScrollView : public View {
public:
    ScrollView(V2 size) : View(size){};
    ~ScrollView(){};
    
    virtual void didScroll(U1 index);
    virtual void didFinishScrolling();
};

#endif /* defined(__Pods__ScrollView__) */

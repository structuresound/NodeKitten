#pragma once

#include "view.h"

class ScrollView : public View {
public:
    ScrollView(V2 size) : View(size){};
    ~ScrollView(){};

    virtual void didScroll(U1 index);
    virtual void didFinishScrolling();
};
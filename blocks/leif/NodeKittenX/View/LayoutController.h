#ifndef __LayoutController__
#define __LayoutController__

#include "View.h"

class Layout {
public:
    virtual void layout(){};
    virtual Layout* nextLayout() {return this;};
    virtual Layout* previousLayout() {return this;};
};

class Column : Layout {
public:
    vector<View*> views;
};

class Row : Layout {
public:
    vector<Column> columns;
};

class ReactiveLayout : Layout {
    vector<Row> rows;
};

class LayoutController : public Layout {
public:
    LayoutController(View* view){_view = view;}
    ~LayoutController(){};
    
    View* selectedView;
    V2 Offset;
    
    bool horizontalMajor {false};
    
    // LAYOUT
    V2 sizeForViewAtIndex(IndexPath indexPath);
    V2 positionForViewAtIndexPath(IndexPath indexPath, View* view);
    V2 spacingForViewAtIndexPath(IndexPath indexPath, View* view);
    
    // SCROLLING
    V2 targetPositionForEstimatedPosition(V2 position);
    
private:
    View* _view;
};

#endif
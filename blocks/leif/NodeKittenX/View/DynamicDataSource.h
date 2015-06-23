//
//  DynamicDataSource.h
//  Pods
//
//  Created by Leif Shackelford on 6/22/15.
//
//

#ifndef __Pods__DynamicDataSource__
#define __Pods__DynamicDataSource__

#include "CommonTypes.h"

class DynamicDataSourceDelegate {
public:
    void insertItemsAtIndexPaths(vector<U1> selectedIndexes);
    void deleteItemsAtIndexPaths(vector<U1> selectedIndexes);
    void reloadItemsAtIndexPaths(vector<U1> selectedIndexes);
    void moveItemFromTo(U1t from, U1t to);
};

class DynamicDataSource {
    JSON _json;
public:
    virtual void setData(JSON, bool shouldAnimate = true);
    JSON data(){return _json;};
    
    DynamicDataSourceDelegate *delegate;
};

#endif /* defined(__Pods__DynamicDataSource__) */

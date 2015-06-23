//
//  DynamicDataSource.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/22/15.
//
//

#include "DynamicDataSource.h"

// DELEGATE

void DynamicDataSourceDelegate::insertItemsAtIndexPaths(vector<U1> selectedIndexes){}
void DynamicDataSourceDelegate::deleteItemsAtIndexPaths(vector<U1> selectedIndexes){}
void DynamicDataSourceDelegate::reloadItemsAtIndexPaths(vector<U1> selectedIndexes){}
void DynamicDataSourceDelegate::moveItemFromTo(U1t from, U1t to){}

// DATA SOURCE

void DynamicDataSource::setData(JSON json, bool shouldAnimate){
    _json = json;
}
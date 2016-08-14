//
//  DynamicDataSource.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/22/15.
//
//

#include "dynamicDataSource.h"

using namespace std;
// DELEGATE

void DynamicDataSourceDelegate::insertItemsAtIndexPaths(vector<U1> selectedIndexes){}
void DynamicDataSourceDelegate::deleteItemsAtIndexPaths(vector<U1> selectedIndexes){}
void DynamicDataSourceDelegate::reloadItemsAtIndexPaths(vector<U1> selectedIndexes){}
void DynamicDataSourceDelegate::moveItemFromTo(U1t from, U1t to){}

// DATA SOURCE

//
//  DynamicDataSource.h
//  Pods
//
//  Created by Leif Shackelford on 6/22/15.
//
//

#ifndef __Pods__DynamicDataSource__
#define __Pods__DynamicDataSource__

#include "commonTypes.h"

class DynamicDataSourceDelegate {
public:
  void insertItemsAtIndexPaths(std::vector<U1> selectedIndexes);
  void deleteItemsAtIndexPaths(std::vector<U1> selectedIndexes);
  void reloadItemsAtIndexPaths(std::vector<U1> selectedIndexes);
  void moveItemFromTo(U1t from, U1t to);
};

class DynamicDataSource : public DynamicDataSourceDelegate{
  JSON _data;
  DynamicDataSourceDelegate *_delegate {this};
public:
  const JSON data() const {return _data;}
  void setData(JSON data, bool shouldAnimate = true){
    _data = data;
  }
};

#endif /* defined(__Pods__DynamicDataSource__) */

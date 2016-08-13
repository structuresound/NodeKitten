//
//  Dispatch.h
//  Pods
//
//  Created by Leif Shackelford on 6/30/15.
//
//

#ifndef __Pods__Dispatch__
#define __Pods__Dispatch__

#include "CommonTypes.h"

class Dispatch {
public:
  static std::deque<std::function<void()>> uiQueue;
};

#endif /* defined(__Pods__Dispatch__) */

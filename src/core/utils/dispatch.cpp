//
//  Dispatch.cpp
//  Pods
//
//  Created by Leif Shackelford on 6/30/15.
//
//

#include "dispatch.h"

using namespace std;

deque<function<void()>> Dispatch::uiQueue;

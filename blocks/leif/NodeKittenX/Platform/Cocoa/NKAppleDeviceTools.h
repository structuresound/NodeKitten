//
//  NKAppleDeviceTools.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/18/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef NodeKittenX_NKAppleDeviceTools_h
#define NodeKittenX_NKAppleDeviceTools_h

#include "NKTypes.h"

namespace NKApple {

inline string NSToCppString(void *s);
    
string pathForResource(string name, string ext);

string appDirectory();
    
}

#endif


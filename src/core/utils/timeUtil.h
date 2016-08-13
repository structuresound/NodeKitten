//
//  TimeUtil.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/24/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __NodeKittenX__Time__
#define __NodeKittenX__Time__

#include "commonTypes.h"

class Time {
    static double launchTime;
public:
    static double getCurrentTime();
    static double getElapsedTime();
};

#endif /* defined(__NodeKittenX__Time__) */

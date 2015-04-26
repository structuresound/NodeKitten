//
//  NKTime.cpp
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/24/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#include "NKTime.h"

#if (TARGET_IOS || TARGET_OSX)
#include <CoreFoundation/CoreFoundation.h>
#elif TARGET_EMSCRIPTEN
#endif

namespace NKTime {

    static double launchTime = getCurrentTime();

    double getElapsedTime() {
        return getCurrentTime() - launchTime;
    }

    double getCurrentTime() {
#if (TARGET_IOS || TARGET_OSX)
        return CFAbsoluteTimeGetCurrent();
#elif TARGET_EMSCRIPTEN
        return emscripten_get_now();
#else
    return SDL_GetTicks() * .001;
#endif
        assert(0);
        return 0.0;
    }
    
    
}
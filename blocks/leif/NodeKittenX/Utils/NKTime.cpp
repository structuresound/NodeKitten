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

namespace Time {
    
    double getCurrentTime() {
#if (TARGET_IOS || TARGET_OSX)
        return CFAbsoluteTimeGetCurrent();
#elif TARGET_EMSCRIPTEN
        return emscripten_get_now();
        //return SDL_GetTicks() * .001;
#endif
        assert(0);
        return 0.0;
    }
    
    
}
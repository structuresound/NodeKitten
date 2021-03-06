#include "time.h"

#if (TARGET_IOS || TARGET_OSX)
#include <CoreFoundation/CoreFoundation.h>
#elif TARGET_EMSCRIPTEN
#endif

double Time::launchTime = Time::getCurrentTime();

double Time::getElapsedTime() {
    return getCurrentTime() - launchTime;
}

double Time::getCurrentTime() {
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

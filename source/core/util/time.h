#pragma once

#include "../type.h"

class Time {
    static double launchTime;
public:
    static double getCurrentTime();
    static double getElapsedTime();
};

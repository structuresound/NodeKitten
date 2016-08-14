#pragma once

#include "../type.h"

class Dispatch {
public:
  static std::deque<std::function<void()>> uiQueue;
};

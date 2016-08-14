#include "dispatch.h"

using namespace std;

deque<function<void()>> Dispatch::uiQueue;

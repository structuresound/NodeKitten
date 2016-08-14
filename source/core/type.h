#pragma once

// CPP STL
// core: ---------------------------
#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>  //for ostringsream
#include <iomanip>  //for setprecision
#include <fstream>
#include <algorithm>
#include <cfloat>
#include <map>
#include <stack>
#include <unordered_map>
#include <memory>
#include <queue>
#include <assert.h>

#if _LIBCPP_STD_VER < 14
//Excerpt From: "Effective Modern C++."
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
  return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
#endif

template <typename R, typename E>
std::unique_ptr<R> unique_ptr_cast(E& ptr){
  return std::make_unique<R>(std::move(ptr));
}

#include "../ext/json/json.h"

typedef Json::Value JSON;

#include "macro.h"
#include "vector/vector.h"

typedef std::function<void(void)> CompletionBlock;

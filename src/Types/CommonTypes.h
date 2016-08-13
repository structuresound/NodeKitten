//
//  CommonTypes.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/17/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#pragma once


#include "Macro.h"

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

#include "Functional.h"
#include "format.h"

#if defined(RELEASE) || defined(NK_RELEASE)
    #define nkLog(args...) ;
    #define nkAssert(x,args...) ;
#else
#if NK_SUPPRESS_LOGS
       #define nkLog(args...) ;
    #else
#define nkLog(args...) fmt::printf(args); printf("\n");
#endif
#define nkAssert(x,args...) if (!x){printf("ASSERT!: "); nkLog(args); assert(0);}
#endif

#define nkError(args...) printf("nk_error: "); fmt::printf(args); printf("\n");

// ext libs

#include "json.h"

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

#include "VectorTypes.h"
#include "ScalarStack.h"

typedef Json::Value JSON;
typedef std::function<void(void)> CompletionBlock;
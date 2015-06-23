//
//  CommonTypes.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/17/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __Types_h_
#define __Types_h_

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

using namespace std;

#ifdef NK_SUPPORTS_UNIQUE_PTR
#define nk_unique_ptr(T) unique_ptr<T>
#define nk_make_unique(T) make_unique<T>
#else
#define nk_unique_ptr(T) shared_ptr<T>
#define nk_make_unique(T) make_shared<T>
#endif

#include "VectorTypes.h"
#include "VectorUtil.h"
#include "TemplateFunctions.h"
#include "StringUtil.h"
#include "Platform.h"
#include "json.h"

typedef Json::Value JSON;

typedef std::function<void(void)> CompletionBlock;

#endif

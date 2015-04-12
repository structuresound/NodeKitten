//
//  NKTypes.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/17/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __NKTypes_h_
#define __NKTypes_h_

#include "NKMacro.h"

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

using namespace std;

#include "NKMath.h"
#include "NKTemplateFunctions.h"
#include "NKColor.h"
#include "format.h"

//static inline void nkLog(...){
//    va_list args;
//    va_start(args, fmt);
//    string format {va_arg(args, char*)};
//    format+="\n";
//    printf( format.c_str(), args );
//}

//#define nkLog fmt::printf

#define nkLog(args...) fmt::printf(args); printf("\n");
//#define nkLog(x,args...) printf((string(x)+"\n").c_str(), args)
#define nkAssert(x,args...) if (!x){nkLog(args); assert(0);}

//#define eprintf(format, args...) fprintf (stderr, format , ##args)

//static inline void nkAssert(bool check, std::string format, ...){
//    if (!check) {
//        format+="\n";
//        va_list args;
//        printf( format.c_str(), args );
//        assert(0);
//    }
//    return;
//}

template <typename T> class NKVector : public vector<T> {
};

template <typename T, unsigned int S> class NKArray : public array<T, S> {
};

typedef map<string, string> NKDictionary;

typedef std::function<void(void)> CompletionBlock;

template <typename T> void printVector(vector<T>& vec) {
    
    int inc = sizeof(T) / sizeof(F1t);
    for (int i = 0; i < vec.size(); i++){
        for (int e = 0; e < inc; e++){
            //nkLog("{0}", vec[i*inc + e]);
            printf("%1.2f ", (double)vec[i*inc + e]);
        }
        printf("\n");
    }
}

#endif

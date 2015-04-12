//
//  NKColor.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 2/2/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __NodeKittenX__NKColor__
#define __NodeKittenX__NKColor__

#include "NKTypes.h"

#define div255 *.003921568
static inline float cblend(F1t col, F1t bl){
    return ((col * bl) + (1. - bl));
}

class NKColor {
public:
    static std::map<std::string, C4t> namedColor;
    
    NKColor(std::string name) {data = NKColor::namedColor[name];};
    NKColor() : data {1,1,1,1} {};
    NKColor(F1t w, F1t a = 1.0) : data {w,w,w,a} {};
    NKColor(F1t r, F1t g, F1t b, F1t a) : data {r,g,b,a} {
    }
    
    C4t data;
    
    bool operator==(const NKColor&rhs){
        return V4Equal(data, rhs.data);
    }
    
    void setAlpha(F1t alpha){data.a = alpha;};
    
    C4t colorWithBlendFactor(F1t blendFactor) {
        
        if (blendFactor > 0.) {
            C4t col;
            
            col.r = cblend(data.r, blendFactor);
            col.g = cblend(data.g, blendFactor);
            col.b = cblend(data.b, blendFactor);
            col.a = data.a;
            
            return col;
        }
        else {
            return V4MakeF(1.0);
        }
        
    }
    
};

class NKByteColor {
public:
    
    static std::map<std::string, UB4t> namedColor;
    
    NKByteColor(std::string name) {data = NKByteColor::namedColor[name];};
    NKByteColor() : data {255,255,255,255} {};
    NKByteColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : data {r,g,b,a} {}
    UB4t data;
    C4t c4Color(){return C4Make(data.r div255, data.r div255, data.r div255, data.r div255);};
    
    bool operator==(const NKByteColor&rhs){
        return UB4Equal(data, rhs.data);
    }
};




#endif /* defined(__NodeKittenX__NKColor__) */

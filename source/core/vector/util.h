//
//  NKMath.h
//  NKNikeField
//
//  Created by Leif Shackelford on 5/7/14.
//  Copyright (c) 2014 Chroma Developer. All rights reserved.
//

#pragma once

#ifdef __cplusplus
#define NK_EXPORT extern "C" __attribute__((visibility ("default")))
#else
#define NK_EXPORT extern __attribute__((visibility ("default")))
#endif
#define NK_AVAILABLE __OSX_AVAILABLE_STARTING

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#ifndef PI
#define PI       3.14159265358979323846
#endif

#ifndef TWO_PI
#define TWO_PI   6.28318530717958647693
#endif

#ifndef M_TWO_PI
#define M_TWO_PI   6.28318530717958647693
#endif

#ifndef FOUR_PI
#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
#define HALF_PI  1.57079632679489661923
#endif

#ifndef DEG_TO_RAD
// (PI/180.0)
#define DEG_TO_RAD(x) ((x) * 0.01745329251994)
#endif

#ifndef RAD_TO_DEG
//(180.0/PI)
#define RAD_TO_DEG(x) ((x) * 57.29577951308232)
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
#define CLAMP(val,min,max) ((val) < (min) ? (min) : ((val > max) ? (max) : (val)))
#endif

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

static const UB4t NKRainbow[] = {
    {255,0,0,255},
    {255,200,0,255},
    {255,255,0,255},
    {0,255,0,255},
    {0,0,255,255},
    {255,0,255,255},
};

static inline F1t sgn(F1t val)
{
    return (val > 0.0f) ? 1.0f : ((val < 0.0f) ? -1.0f : 0.0f);
}


static inline F1t clampf(F1t n, F1t min, F1t max)
{
    return std::min(std::max(n,min),max);
}

inline F1t logAverage (F1t src, F1t dst, F1t d){

    return src == dst ? src : ((src * (1.- d*d) + dst * d * d));

}

template <typename T>
T Lerp( T& val1, T& val2, float u)
{
    return (1 - u) * val1 + u * val2;
}

template <typename T>
T random(T max)
{
    return rand() % max;
}

static inline F1t weightedAverage (F1t src, F1t dst, F1t d){
    return src == dst ? src : (src * (1.-d) + dst * d);
}

static inline F1t getTween (F1t src, F1t dst, F1t d){
    return weightedAverage(src,dst,d);
}

static inline V2t getTween(V2t src, V2t dst, F1t d){
    return V2t{weightedAverage(src.x, dst.x, d),
        weightedAverage(src.y, dst.y, d)};
}

static inline V3t getTween(V3t src, V3t dst, F1t d){
    return V3t{weightedAverage(src.x, dst.x, d),
        weightedAverage(src.y, dst.y, d),
        weightedAverage(src.z, dst.z, d)};
}

static inline V4t getTween(V4t src, V4t dst, F1t d){
    return V4t{weightedAverage(src.x, dst.x, d),
        weightedAverage(src.y, dst.y, d),
        weightedAverage(src.z, dst.z, d),
        weightedAverage(src.w, dst.w, d)};
}

static inline P2t polToCar(P2t pol) {

    P2t car;

    car.x = pol.x*cosf(pol.y);
    car.y = pol.x*sin(pol.y);

    return car;

}

static inline P2t carToPol(P2t car){

    P2t pol;

    pol.x = sqrt(car.x*car.x + car.y*car.y);
    pol.y = atan2( car.y, car.x );

    return pol;

}

static inline F1t FModulus(F1t a, F1t b)
{
    int result = (int)( a / b );
    return a - (F1t)( result ) * b;
}

template<typename T>
static inline T getCatmullInterpolate(
                                           T y0,T y1,
                                           T y2,T y3,
                                           D1t mu)
{
    T a0,a1,a2,a3;
    D1t mu2;

    mu2 = mu*mu;
    a0 = y0*-0.5 + y1*1.5 - y2*1.5 + y3*0.5;
    a1 = y0 - y1*2.5 + y2*2.0 - y3*0.5;
    a2 = y0*-0.5 + y2*0.5;
    a3 = y1;

    return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}

static inline double getCubicInterpolate(
                                         double y0,double y1,
                                         double y2,double y3,
                                         double mu)
{
    double a0,a1,a2,a3,mu2;

    mu2 = mu*mu;
    a0 = y3 - y2 - y0 + y1;
    a1 = y0 - y1 - a0;
    a2 = y2 - y0;
    a3 = y1;

    return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}

static inline F1t CGMap(F1t n, F1t minIn, F1t maxIn, F1t minOut, F1t maxOut)
{
    F1t inRange = maxIn - minIn;
    F1t outRange = maxOut - minOut;
    F1t scalarN = (n-minIn) / inRange;
    F1t ret = minOut + (outRange * scalarN);
    if(isinf(ret) || isnan(ret)){
        ret = maxOut;
    }else{
        ret = clampf(ret, minOut, maxOut);
    }
    return ret;
}

static inline F1t approxBezierLength(B12t b)
{
    // a p1 p2 b

    float length = (V3Length(V3Subtract(b.v[0], b.v[1])) +
                    V3Length(V3Subtract(b.v[1], b.v[2])) +
                    V3Length(V3Subtract(b.v[2], b.v[3])));

    return length;
}

static inline F1t B12Length(B12t b) {
    V3t p0 = V3Subtract(b.v[0], b.v[1]);
    V3t p1 = V3Subtract(b.v[2], b.v[1]);
    V3t p2;
    V3t p3 = V3Subtract(b.v[3], b.v[2]);


    // this is essentially normalize -- ls
    F1t l0 = V3Length(p0);
    F1t l1 = V3Length(p1);
    F1t l3 = V3Length(p3);
    if (l0 > 0.f) p0 = V3DivideScalar(p0, l0);
    if (l1 > 0.f) p1 = V3DivideScalar(p1, l1);
    if (l3 > 0.f) p3 = V3DivideScalar(p3, l3);

    //    p0 = V3FastNormalize(p0);
    //    p1 = V3FastNormalize(p1);
    //    p3 = V3FastNormalize(p3);

    p2 = V3Negate(p1);

    F1t a = fabsf(V3DotProduct(p0, p1)) + fabsf(V3DotProduct(p2, p3));

    if (a == 0) return 0;

    if ((a > 1.98f) || ((l0+l1+l3) < ((4.f - a)*8.f))) {
        return l0+l1+l3;
    }
    // else
    B12t bl,br;

    bl.v[0] = b.v[0];
    bl.v[1] = V3MultiplyScalar(V3Add(b.v[0], b.v[1]), .5);
    V3t mid = V3MultiplyScalar(V3Add(b.v[1], b.v[2]), .5);

    bl.v[2] = V3MultiplyScalar(V3Add(bl.v[1], mid), .5);
    br.v[3] = b.v[3];

    br.v[2] = V3MultiplyScalar(V3Add(b.v[2], mid), .5);
    br.v[1] = V3MultiplyScalar(V3Add(br.v[2], mid), .5);
    br.v[0] = V3MultiplyScalar(V3Add(br.v[1], bl.v[2]), .5);
    bl.v[3] = br.v[0];

    return B12Length(bl) + B12Length(br);
} // bezLength3

static inline unsigned int hash() {
    auto hash = "hash";
    return XXH32 (&hash, sizeof(hash), 0);
}

template <typename T> T randomScalar()
{
    T ret;
    F1t *start = ret.v;
    auto length = (sizeof(T) / sizeof(F1t));
    for (int i = 0; i < length; i++) {
        *(start + i) = rand() / (F1t)RAND_MAX;
    }
    return ret;
}

static inline V4t V4r()
{
    return randomScalar<V4t>();
}

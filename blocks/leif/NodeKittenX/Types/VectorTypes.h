//*
//*  NODE KITTEN
//*

#ifndef __NKVectorTypes__
#define __NKVectorTypes__


#if TARGET_OSX
#include <Accelerate/Accelerate.h>
#endif

#include "Macro.h"
#include "xxhash.h"

#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <float.h>
#include <string>


#pragma mark -
#pragma mark NK VECTOR TYPES
#pragma mark -

#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)
#define CONVERT_UV_U_TO_ST_S(u) ((2*u) / M_PI)
#define div255 *.003921568f

#define NK_INLINE static inline

const float SMALL=.000001f;		//small value
const float BIG=10000000.0f;	//big value

typedef uint32_t U1t;
typedef U1t U1;
typedef unsigned long ULt;
typedef ULt UL;

typedef int32_t I1t;

union _U2t {
    U1t v[2];
    
    struct { U1t section, item; };
    struct { U1t col, row; };
    struct { U1t x, y; };
    struct { U1t min, max; };
    struct { U1t s, t; };
    struct { U1t width, height; };
    
    _U2t(const U1t x,const U1t y){
        s = x; t = y;
    }
    _U2t(const U1t f){
        x = y = f;
    }
    _U2t(){
        x = y = 0;
    }
    
    operator U1t*(){
        return &x;
    }
    operator const U1t *(){
        return &x;
    }
    U1t &operator[]( U1t n ){
        return (&x)[n];
    }
    U1t operator[]( U1t n )const{
        return (&x)[n];
    }
    _U2t operator-()const{
        return _U2t{ -x,-y };
    }
    _U2t operator*( U1t scale )const{
        return _U2t{ x*scale,y*scale};
    }
    _U2t operator*( const _U2t &q )const{
        return _U2t{ x*q.x,y*q.y};
    }
    _U2t operator/( U1t scale )const{
        return _U2t{ x/scale,y/scale };
    }
    _U2t operator/( const _U2t &q )const{
        return _U2t{ x/q.x,y/q.y};
    }
    _U2t operator+( const _U2t &q )const{
        return _U2t{ x+q.x,y+q.y };
    }
    _U2t operator-( const _U2t &q )const{
        return _U2t{ x-q.x,y-q.y};
    }
    _U2t &operator*=( U1t scale ){
        x*=scale;y*=scale;return *this;
    }
    _U2t &operator*=( const _U2t &q ){
        x*=q.x;y*=q.y;return *this;
    }
    _U2t &operator/=( U1t scale ){
        x/=scale;y/=scale;return *this;
    }
    _U2t &operator/=( const _U2t &q ){
        x/=q.x;y/=q.y;return *this;
    }
    _U2t &operator+=( const _U2t &q ){
        x+=q.x;y+=q.y;return *this;
    }
    _U2t &operator-=( const _U2t &q ){
        x-=q.x;y-=q.y;return *this;
    }
    
    bool operator<( const _U2t &q )const{
        if( fabs(x-q.x)>0 ) return x<q.x ? true : false;
        return fabs(y-q.y)>0;
    }
    bool operator==( const _U2t &q )const{
        return fabs(x-q.x)<=0 && fabs(y-q.y)<=0;
    }
    bool operator!=( const _U2t &q )const{
        return fabs(x-q.x)>0 || fabs(y-q.y)>0;
    }
    float dot( const _U2t &q )const{
        return x*q.x+y*q.y;
    }
    void clear(){
        x=y=0;
    }
};

typedef union _U2t U2;
typedef U2 IndexPath;

union _UB4t
{
    GLubyte u[4];
    struct {GLubyte r,g,b,a;};
    struct {unsigned char x,y,z,w;};
    
    bool operator<( const _UB4t &q )const{
        return XXH32 (this, sizeof(_UB4t), 0) < XXH32 (&q, sizeof(_UB4t), 0);
    }
    bool operator!(){
        return !(r+g+b+a);
    }
};

typedef union _UB4t UB4t;
typedef union _UB4t UB4;
typedef UB4t C4t;

static std::map<std::string, UB4t> UB4Colors = {
    { "transparent", { 0, 0, 0, 0 } }, { "aliceblue", { 240, 248, 255, 255 } },
    { "antiquewhite", { 250, 235, 215, 255 } }, { "aqua", { 0, 255, 255, 255 } },
    { "aquamarine", { 127, 255, 212, 255 } }, { "azure", { 240, 255, 255, 255 } },
    { "beige", { 245, 245, 220, 255 } }, { "bisque", { 255, 228, 196, 255 } },
    { "black", { 0, 0, 0, 255 } }, { "blanchedalmond", { 255, 235, 205, 255 } },
    { "blue", { 0, 0, 255, 255 } }, { "blueviolet", { 138, 43, 226, 255 } },
    { "brown", { 165, 42, 42, 255 } }, { "burlywood", { 222, 184, 135, 255 } },
    { "cadetblue", { 95, 158, 160, 255 } }, { "chartreuse", { 127, 255, 0, 255 } },
    { "chocolate", { 210, 105, 30, 255 } }, { "coral", { 255, 127, 80, 255 } },
    { "cornflowerblue", { 100, 149, 237, 255 } }, { "cornsilk", { 255, 248, 220, 255 } },
    { "crimson", { 220, 20, 60, 255 } }, { "cyan", { 0, 255, 255, 255 } },
    { "darkblue", { 0, 0, 139, 255 } }, { "darkcyan", { 0, 139, 139, 255 } },
    { "darkgoldenrod", { 184, 134, 11, 255 } }, { "darkgray", { 169, 169, 169, 255 } },
    { "darkgreen", { 0, 100, 0, 255 } }, { "darkgrey", { 169, 169, 169, 255 } },
    { "darkkhaki", { 189, 183, 107, 255 } }, { "darkmagenta", { 139, 0, 139, 255 } },
    { "darkolivegreen", { 85, 107, 47, 255 } }, { "darkorange", { 255, 140, 0, 255 } },
    { "darkorchid", { 153, 50, 204, 255 } }, { "darkred", { 139, 0, 0, 255 } },
    { "darksalmon", { 233, 150, 122, 255 } }, { "darkseagreen", { 143, 188, 143, 255 } },
    { "darkslateblue", { 72, 61, 139, 255 } }, { "darkslategray", { 47, 79, 79, 255 } },
    { "darkslategrey", { 47, 79, 79, 255 } }, { "darkturquoise", { 0, 206, 209, 255 } },
    { "darkviolet", { 148, 0, 211, 255 } }, { "deeppink", { 255, 20, 147, 255 } },
    { "deepskyblue", { 0, 191, 255, 255 } }, { "dimgray", { 105, 105, 105, 255 } },
    { "dimgrey", { 105, 105, 105, 255 } }, { "dodgerblue", { 30, 144, 255, 255 } },
    { "firebrick", { 178, 34, 34, 255 } }, { "floralwhite", { 255, 250, 240, 255 } },
    { "forestgreen", { 34, 139, 34, 255 } }, { "fuchsia", { 255, 0, 255, 255 } },
    { "gainsboro", { 220, 220, 220, 255 } }, { "ghostwhite", { 248, 248, 255, 255 } },
    { "gold", { 255, 215, 0, 255 } }, { "goldenrod", { 218, 165, 32, 255 } },
    { "gray", { 128, 128, 128, 255 } }, { "green", { 0, 128, 0, 255 } },
    { "greenyellow", { 173, 255, 47, 255 } }, { "grey", { 128, 128, 128, 255 } },
    { "honeydew", { 240, 255, 240, 255 } }, { "hotpink", { 255, 105, 180, 255 } },
    { "indianred", { 205, 92, 92, 255 } }, { "indigo", { 75, 0, 130, 255 } },
    { "ivory", { 255, 255, 240, 255 } }, { "khaki", { 240, 230, 140, 255 } },
    { "lavender", { 230, 230, 250, 255 } }, { "lavenderblush", { 255, 240, 245, 255 } },
    { "lawngreen", { 124, 252, 0, 255 } }, { "lemonchiffon", { 255, 250, 205, 255 } },
    { "lightblue", { 173, 216, 230, 255 } }, { "lightcoral", { 240, 128, 128, 255 } },
    { "lightcyan", { 224, 255, 255, 255 } }, { "lightgoldenrodyellow", { 250, 250, 210, 255 } },
    { "lightgray", { 211, 211, 211, 255 } }, { "lightgreen", { 144, 238, 144, 255 } },
    { "lightgrey", { 211, 211, 211, 255 } }, { "lightpink", { 255, 182, 193, 255 } },
    { "lightsalmon", { 255, 160, 122, 255 } }, { "lightseagreen", { 32, 178, 170, 255 } },
    { "lightskyblue", { 135, 206, 250, 255 } }, { "lightslategray", { 119, 136, 153, 255 } },
    { "lightslategrey", { 119, 136, 153, 255 } }, { "lightsteelblue", { 176, 196, 222, 255 } },
    { "lightyellow", { 255, 255, 224, 255 } }, { "lime", { 0, 255, 0, 255 } },
    { "limegreen", { 50, 205, 50, 255 } }, { "linen", { 250, 240, 230, 255 } },
    { "magenta", { 255, 0, 255, 255 } }, { "maroon", { 128, 0, 0, 255 } },
    { "mediumaquamarine", { 102, 205, 170, 255 } }, { "mediumblue", { 0, 0, 205, 255 } },
    { "mediumorchid", { 186, 85, 211, 255 } }, { "mediumpurple", { 147, 112, 219, 255 } },
    { "mediumseagreen", { 60, 179, 113, 255 } }, { "mediumslateblue", { 123, 104, 238, 255 } },
    { "mediumspringgreen", { 0, 250, 154, 255 } }, { "mediumturquoise", { 72, 209, 204, 255 } },
    { "mediumvioletred", { 199, 21, 133, 255 } }, { "midnightblue", { 25, 25, 112, 255 } },
    { "mintcream", { 245, 255, 250, 255 } }, { "mistyrose", { 255, 228, 225, 255 } },
    { "moccasin", { 255, 228, 181, 255 } }, { "navajowhite", { 255, 222, 173, 255 } },
    { "navy", { 0, 0, 128, 255 } }, { "oldlace", { 253, 245, 230, 255 } },
    { "olive", { 128, 128, 0, 255 } }, { "olivedrab", { 107, 142, 35, 255 } },
    { "orange", { 255, 165, 0, 255 } }, { "orangered", { 255, 69, 0, 255 } },
    { "orchid", { 218, 112, 214, 255 } }, { "palegoldenrod", { 238, 232, 170, 255 } },
    { "palegreen", { 152, 251, 152, 255 } }, { "paleturquoise", { 175, 238, 238, 255 } },
    { "palevioletred", { 219, 112, 147, 255 } }, { "papayawhip", { 255, 239, 213, 255 } },
    { "peachpuff", { 255, 218, 185, 255 } }, { "peru", { 205, 133, 63, 255 } },
    { "pink", { 255, 192, 203, 255 } }, { "plum", { 221, 160, 221, 255 } },
    { "powderblue", { 176, 224, 230, 255 } }, { "purple", { 128, 0, 128, 255 } },
    { "red", { 255, 0, 0, 255 } }, { "rosybrown", { 188, 143, 143, 255 } },
    { "royalblue", { 65, 105, 225, 255 } }, { "saddlebrown", { 139, 69, 19, 255 } },
    { "salmon", { 250, 128, 114, 255 } }, { "sandybrown", { 244, 164, 96, 255 } },
    { "seagreen", { 46, 139, 87, 255 } }, { "seashell", { 255, 245, 238, 255 } },
    { "sienna", { 160, 82, 45, 255 } }, { "silver", { 192, 192, 192, 255 } },
    { "skyblue", { 135, 206, 235, 255 } }, { "slateblue", { 106, 90, 205, 255 } },
    { "slategray", { 112, 128, 144, 255 } }, { "slategrey", { 112, 128, 144, 255 } },
    { "snow", { 255, 250, 250, 255 } }, { "springgreen", { 0, 255, 127, 255 } },
    { "steelblue", { 70, 130, 180, 255 } }, { "tan", { 210, 180, 140, 255 } },
    { "teal", { 0, 128, 128, 255 } }, { "thistle", { 216, 191, 216, 255 } },
    { "tomato", { 255, 99, 71, 255 } }, { "turquoise", { 64, 224, 208, 255 } },
    { "violet", { 238, 130, 238, 255 } }, { "wheat", { 245, 222, 179, 255 } },
    { "white", { 255, 255, 255, 255 } }, { "whitesmoke", { 245, 245, 245, 255 } },
    { "yellow", { 255, 255, 0, 255 } }, { "yellowgreen", { 154, 205, 50, 255 } }
};

static inline UB4t UB4Color(const string& name){
    return UB4Colors[name];
}

typedef GLfloat F1t;
typedef double D1t;

union _V2t {
    F1t v[2];
    
    struct { F1t x, y; };
    struct { F1t min, max; };
    struct { F1t s, t; };
    struct { F1t width, height; };
    
    _V2t(const F1t x,const F1t y){
        s = x; t = y;
    }
    _V2t(const F1t f){
        x = y = f;
    }
    _V2t(){
        x = y = 0.0f;
    }
    
    operator float*(){
        return &x;
    }
    operator const float *(){
        return &x;
    }
    float &operator[]( int n ){
        return (&x)[n];
    }
    float operator[]( int n )const{
        return (&x)[n];
    }
    _V2t operator-()const{
        return _V2t{ -x,-y };
    }
    _V2t operator*( F1t scale )const{
        return _V2t{ x*scale,y*scale};
    }
    _V2t operator*( double scale )const{
        return _V2t{ x*(F1t)scale,y*(F1t)scale};
    }
    _V2t operator*( const _V2t &q )const{
        return _V2t{ x*q.x,y*q.y};
    }
    _V2t operator/( float scale )const{
        return _V2t{ x/scale,y/scale };
    }
    _V2t operator/( const _V2t &q )const{
        return _V2t{ x/q.x,y/q.y};
    }
    _V2t operator+( const _V2t &q )const{
        return _V2t{ x+q.x,y+q.y };
    }
    _V2t operator-( const _V2t &q )const{
        return _V2t{ x-q.x,y-q.y};
    }
    _V2t &operator*=( float scale ){
        x*=scale;y*=scale;return *this;
    }
    _V2t &operator*=( const _V2t &q ){
        x*=q.x;y*=q.y;return *this;
    }
    _V2t &operator/=( float scale ){
        x/=scale;y/=scale;return *this;
    }
    _V2t &operator/=( const _V2t &q ){
        x/=q.x;y/=q.y;return *this;
    }
    _V2t &operator+=( const _V2t &q ){
        x+=q.x;y+=q.y;return *this;
    }
    _V2t &operator-=( const _V2t &q ){
        x-=q.x;y-=q.y;return *this;
    }
    
    bool operator<( const _V2t &q )const{
        if( fabs(x-q.x)>SMALL ) return x<q.x ? true : false;
        return fabs(y-q.y)>SMALL;
    }
    
    bool operator==( const _V2t &q )const{
        return fabs(x-q.x)<=SMALL && fabs(y-q.y)<=SMALL;
    }
    bool operator!=( const _V2t &q )const{
        return fabs(x-q.x)>SMALL || fabs(y-q.y)>SMALL;
    }
    float dot( const _V2t &q )const{
        return x*q.x+y*q.y;
    }
    float length()const{
        return sqrtf(x*x+y*y);
    }
    float distance( const _V2t &q )const{
        float dx=x-q.x,dy=y-q.y;return sqrtf(dx*dx+dy*dy);
    }
    _V2t normalized()const{
        float l=length();return _V2t{ x/l,y/l};
    }
    void normalize(){
        float l=length();x/=l;y/=l;
    }
    void clear(){
        x=y=0;
    }
    
    // OTHERS
    
    _V2t toCar(){
        return {x*cosf(y),x*sin(y)};
    }
    
    _V2t toPol(){
        return {sqrt(x*x + y*y),atan2(y,x)};
    }
    
    F1t rotation(){
        return atan2(y,x);
    }
    
};

typedef union _V2t V2t;
typedef union _V2t V2;

union _I2t {
    I1t i[2];
    struct { I1t x, y; };
    struct { I1t min, max; };
    struct { I1t s, t; };
    struct { I1t width, height; };
    
};
typedef union _I2t I2t;

typedef V2t P2t;
typedef V2t P2;
typedef V2t S2t;

union _V3t {
    F1t v[3];
    
    struct { F1t x, y, z; };
    struct { F1t r, g, b; };
    struct { F1t hue, sat, val; };
    struct { F1t width, height, depth; };
    struct { V2t xy; F1t unused; };
    
    _V3t(const F1t r,const F1t g,const F1t b){
        x = r; y = g; z = b;
    }
    _V3t(const V2t v2){
        x = v2.x; y = v2.y; z = 1;
    }
    _V3t(const F1t f){
        x = y = z = f;
    }
    _V3t(){
        x = y = z = 0.0f;
    }
    
    operator float*(){
        return &x;
    }
    operator const float *(){
        return &x;
    }
    float &operator[]( int n ){
        return (&x)[n];
    }
    float operator[]( int n )const{
        return (&x)[n];
    }
//    _V3t operator=( const _V2t &q ){
//        x=q.x; y=q.y; z ? z : z=1;
//        return *this;
//    }
    _V3t operator-()const{
        return _V3t{ -x,-y,-z };
    }
    _V3t operator*( F1t scale )const{
        return _V3t{ x*scale,y*scale,z*scale };
    }
    _V3t operator*( double scale )const{
        return _V3t{ x*(F1t)scale,y*(F1t)scale,z*(F1t)scale };
    }
    _V3t operator*( const _V3t &q )const{
        return _V3t{ x*q.x,y*q.y,z*q.z };
    }
    _V3t operator/( float scale )const{
        return _V3t{ x/scale,y/scale,z/scale };
    }
    _V3t operator/( const _V3t &q )const{
        return _V3t{ x/q.x,y/q.y,z/q.z };
    }
    _V3t operator+( const _V3t &q )const{
        return _V3t{ x+q.x,y+q.y,z+q.z };
    }
    _V3t operator-( const _V3t &q )const{
        return _V3t{ x-q.x,y-q.y,z-q.z };
    }
    _V3t &operator*=( float scale ){
        x*=scale;y*=scale;z*=scale;return *this;
    }
    _V3t &operator*=( const _V3t &q ){
        x*=q.x;y*=q.y;z*=q.z;return *this;
    }
    _V3t &operator/=( float scale ){
        x/=scale;y/=scale;z/=scale;return *this;
    }
    _V3t &operator/=( const _V3t &q ){
        x/=q.x;y/=q.y;z/=q.z;return *this;
    }
    _V3t &operator+=( const _V3t &q ){
        x+=q.x;y+=q.y;z+=q.z;return *this;
    }
    _V3t &operator-=( const _V3t &q ){
        x-=q.x;y-=q.y;z-=q.z;return *this;
    }
    
    bool operator<( const _V3t &q )const{
        if( fabs(x-q.x)>SMALL ) return x<q.x ? true : false;
        if( fabs(y-q.y)>SMALL ) return y<q.y ? true : false;
        return fabs(z-q.z)>SMALL && z<q.z;
    }
    bool operator==( const _V3t &q )const{
        return fabs(x-q.x)<=SMALL && fabs(y-q.y)<=SMALL && fabs(z-q.z)<=SMALL;
    }
    bool operator!=( const _V3t &q )const{
        return fabs(x-q.x)>SMALL || fabs(y-q.y)>SMALL || fabs(z-q.z)>SMALL;
    }
    float dot( const _V3t &q )const{
        return x*q.x+y*q.y+z*q.z;
    }
    _V3t cross( const _V3t &q )const{
        return _V3t{ y*q.z-z*q.y,z*q.x-x*q.z,x*q.y-y*q.x };
    }
    float length()const{
        return sqrtf(x*x+y*y+z*z);
    }
    float distance( const _V3t &q )const{
        float dx=x-q.x,dy=y-q.y,dz=z-q.z;return sqrtf(dx*dx+dy*dy+dz*dz);
    }
    _V3t normalized()const{
        float l=length();return _V3t{ x/l,y/l,z/l };
    }
    void normalize(){
        float l=length();x/=l;y/=l;z/=l;
    }
    float yaw()const{
        return -atan2f( x,z );
    }
    float pitch()const{
        return -atan2f( y,sqrtf( x*x+z*z ) );
    }
    void clear(){
        x=y=z=0;
    }
    
}; // VECTOR 3

typedef union _V3t V3t;
typedef union _V3t V3;
typedef V3t Vector;

typedef V3t HSVColor;

union _V4t
{
    F1t v[4];
    
    struct { F1t x, y, z, w; };
    struct {F1t x_, y_, width, height;};
    struct { V3t xyz; F1t unused;};
    struct { V3t rgb; F1t alpha;};
    struct { V2t origin; S2t size;};
    struct { V2t xy; V2t st;};
    struct { V2t pp, pn; };
    
    struct { F1t r, g, b, a; };
    struct { F1t s, t, p, q; };
    struct { F1t m00, m01, m10, m11;};

    _V4t(const F1t r,const F1t g,const F1t b,const F1t a){
        x = r; y = g; z = b; w = a;
    }
    _V4t(const F1t r,const F1t g,const F1t b){
        x = r; y = g; z = b; w = 1.0;
    }
    _V4t(F1t white, F1t alpha){
        x = y = z = white; w = alpha;
    }
    _V4t(F1t f){
        x = y = z = w = f;
    }
    _V4t(){
        x = y = z = w = 0.0f;
    }
    
    _V4t(const UB4t color){
        r = color.r div255;
        g = color.g div255;
        b = color.b div255;
        a = color.a div255;
    }
    _V4t(const string& name):_V4t(UB4Colors[name]){
    }
    _V4t operator=( const _V3t &q ){
        x=q.x; y=q.y; z=q.z; w ? w : w=1;
        return *this;
    }
    _V4t operator-()const{
        V3t n = -xyz;
        return _V4t{n.x,n.y,n.z, w};
    }
    _V4t operator+( const _V4t &q )const{
        V3t n = xyz+q.xyz;
        return _V4t{n.x,n.y,n.z,w+q.w};
    }
    _V4t operator-( const _V4t &q )const{
        V3t n = xyz-q.xyz;
        return _V4t{ n.x, n.y, n.z, w-q.w};
    }
    _V4t operator*( const _V4t &q )const{
        V3t n = q.xyz.cross(xyz)+q.xyz*w+xyz*q.w;
        return _V4t{n.x,n.y,n.z,w*q.w-xyz.dot(q.xyz) };
    }
    V3t operator*( const V3t &q )const{
        return (*this * _V4t{q.x,q.y,q.z,0} * -*this).xyz;
    }
    _V4t operator*( float q )const{
        V3t n = xyz*q;
        return _V4t {n.x,n.y,n.z, w*q};
    }
    _V4t operator/( float q )const{
        V3t n = xyz/q;
        return _V4t{ n.z,n.y,n.z,w/q };
    }
    bool visible() const{
        return a > 0.0f;
    }
    float dot( const _V4t &q )const{
        return x*q.x+y*q.y+z*z+w*w;
    }
    float length()const{
        return sqrtf( w*w+x*x+y*y+z*z);
    }
    void normalize(){
        *this=*this/length();
    }
    _V4t normalized()const{
        return *this/length();
    }
    _V4t slerpTo( const _V4t &q,float a )const{
        _V4t t=q;
        float d=dot(q),b=1-a;
        if( d<0 ){ t.w=-t.w;t.xyz=-t.xyz;d=-d; }
        if( d<1-SMALL ){
            float om=acosf( d );
            float si=sinf( om );
            a=sinf( a*om )/si;
            b=sinf( b*om )/si;
        }
        return *this*b + t*a;
    }
    V3t i()const{
        return V3t{ 1-2*(y*y+z*z),2*(x*y-w*z),2*(x*z+w*y) };
    }
    V3t j()const{
        return V3t{ 2*(x*y+w*z),1-2*(x*x+z*z),2*(y*z-w*x) };
    }
    V3t k()const{
        return V3t{ 2*(x*z-w*y),2*(y*z+w*x),1-2*(x*x+y*y) };
    }
    _V4t blend(F1t blendFactor) {
        return _V4t {
            (r * blendFactor) + (1.0f - blendFactor),
            (g * blendFactor) + (1.0f - blendFactor),
            (b * blendFactor) + (1.0f - blendFactor),
            a
        };
    }
    
    _V4t fade(F1t alpha){
        return _V4t {r,g,b,a*alpha};
    }
    
} __attribute__((aligned(16)));

typedef union _V4t V4t;
typedef V4t V4;

typedef V4t Color;

typedef V4t Q4t; typedef V4t Q4;
typedef V4t R4t; typedef V4t R4;
typedef V4t M4t; typedef V4t M4;

static V4 CLEAR = {0,0,0,0};
static V4 BLACK = {0,0,0,1};
static V4 WHITE = {1,1,1,1};

union _UB3t
{
    GLubyte u[4];
    struct {GLubyte r,g,b;};
    struct {GLubyte x,y,z;};
};

typedef union _UB3t UB3t;

typedef struct {
    F1t a;
    F1t x;
    F1t y;
    F1t z;
} A4t; // ANGLE+AXIS 4

union _V6t
{
    F1t m[6];
    
    struct
    {
        V3t min;
        V3t max;
    };
    
    struct
    {
        V3t v1;
        V3t v2;
    };
    
    struct
    {
        V2t x;
        V2t y;
        V2t z;
    };
    
    _V6t(){
        memset(&m, 0, sizeof(_V6t));
    }
    
    _V6t(F1t a,F1t b,F1t c,F1t d,F1t e,F1t f){
        m[0] = a; m[1] = b; m[2] = c;
        m[3] = d; m[4] = e; m[5] = f;
    }
    
    
    _V6t( const V3t &v1,const V3t &v2 ):v1(v1),v2(v2){
    }
    
    _V6t operator+( const V3t &q )const{
        return _V6t( v1+q,v2 );
    }
    _V6t operator-( const V3t &q )const{
        return _V6t( v1-q,v2 );
    }
    V3t operator*( float q )const{
        return v1+v2*q;
    }
    V3t nearest( const V3t &q )const{
        return v1+v2*(v1.dot(q-v1)/v2.dot(v2));
    }
    
};

typedef union _V6t V6t;
typedef V6t V6;
typedef V6t Line;

union _V9t
{
    F1t m[9];
    
    struct
    {
        V3t xAxis;
        V3t yAxis;
        V3t zAxis;
    };
    struct
    {
        V3t	v1;
        V3t	v2;
        V3t	v3;
    };
} ;

typedef union _V9t T3t;
typedef union _V9t V9t;
typedef union _V9t Triangle;

union _plane {
    struct {
        V3t n; float d;
    };
    V4t v;
    
    _plane():d(0){
    }
    //normal/offset form
    _plane( const V3t &n,float d ):n(n),d(d){
    }
    
    //point/normal form
    _plane( const V3t &p,const V3t &n ):n(n),d(-n.dot(p)){
    }
    
    //create plane from tri
    _plane( const V3t &v0,const V3t &v1,const V3t &v2 ){
        set(v0, v1, v2);
    }
    
    _plane operator-()const{
        return _plane( -n,-d );
    }
    
    void set( const V3t &v0,const V3t &v1,const V3t &v2 ){
        n=(v1-v0).cross(v2-v0).normalized();d=-n.dot(v0);
    }
    
    float t_intersect( const Line &q )const{
        return -distance(q.v1)/n.dot(q.v2);
    }
    V3t intersect( const Line &q )const{
        return q*t_intersect(q);
    }
    Line intersect( const _plane &q )const{
        V3t lv=n.cross( q.n ).normalized();
        return Line( q.intersect( Line( nearest( n*-d ),n.cross(lv) ) ),lv );
    }
    V3t nearest( const V3t &q )const{
        return q-n*distance(q);
    }
    void negate(){
        n=-n;d=-d;
    }
    float distance( const V3t &q )const{
        return n.dot(q)+d;
    }
};

typedef union _plane Plane;

union _M9t
{
    F1t m[9];
    
    struct
    {
        V3t v[3];
    };
    struct
    {
        F1t colRow[3][3];
    };
    struct
    {
        F1t m00, m01, m02; //  c1r1 c1r2 c1r3
        F1t m10, m11, m12; //  c2r1 c2r2 c3r3
        F1t m20, m21, m22; //  c3r1 c3r2 c3r3
    };
    
    _M9t(){
        memset(&m, 0, sizeof(_M9t));
    }
    
    _M9t(F1t i00, F1t i01, F1t i02,
          F1t i10, F1t i11, F1t i12,
          F1t i20, F1t i21, F1t i22
          ){
        m00=i00;m01=i01;m02=i02;
        m10=i10;m11=i11;m12=i12;
        m20=i20;m21=i21;m22=i22;
    }
    
};

typedef union _M9t M9t;
typedef M9t M9;

union _B12t
{
    F1t m[12];
    struct {V3t v[4];};
    struct {V3t p1, cp1, cp2, p2;};
    
    _B12t(){
        memset(&m, 0, sizeof(_B12t));
    }
    
};
typedef union _B12t B12t;

union _M16t
{
    F1t m[16];
    
    struct
    {
        F1t m00, m01, m02, m03;
        F1t m10, m11, m12, m13;
        F1t m20, m21, m22, m23;
        F1t m30, m31, m32, m33;
    };
    struct
    {
        F1t colRow[4][4];
    };
    struct
    {
        V4t column1;
        V4t column2;
        V4t column3;
        V4t column4;
    };
    struct
    {
        V4t v[4];
    };
    
    _M16t(){
        memset(&m, 0, sizeof(_M16t));
    }
    
    _M16t(F1t i00, F1t i01, F1t i02, F1t i03,
          F1t i10, F1t i11, F1t i12, F1t i13,
          F1t i20, F1t i21, F1t i22, F1t i23,
          F1t i30, F1t i31, F1t i32, F1t i33){
        m00=i00;m01=i01;m02=i02;m03=i03;
        m10=i10;m11=i11;m12=i12;m13=i13;
        m20=i20;m21=i21;m22=i22;m23=i23;
        m30=i30;m31=i31;m32=i32;m33=i33;
    }
    //    _M16t &operator=( const _M16t &q ){
    //        column1 = q.column1;
    //        column2 = q.column2;
    //        column3 = q.column3;
    //        column4 = q.column4;
    //        return *this;
    //    }
    
} __attribute__((aligned(16)));

typedef union _M16t M16t;
typedef M16t M16;

#pragma MAKE FUNCTIONS

static inline V3t V3RandomRanged(F1t range) {
    return V3t(((rand() % 1000) * .002 * range) - range,((rand() % 1000) * .002 * range) - range,((rand() % 1000) * .002 * range) - range);
}

static inline Q4t Q4Identity(){
    return Q4t{0,0,0,1.};
}

static inline A4t A4Make(F1t angle,F1t x,F1t y,F1t z){
    A4t A4;
    
    A4.a = angle;
    A4.x = x;
    A4.y = y;
    A4.z = z;
    
    return A4;
}

static inline bool R4ContainsPoint(R4t rect, V2t point){
    for (int i = rect.x; i < rect.x + rect.size.width; i++) {
        if ((int)point.x == i) {
            for (int j = rect.y; j < rect.y + rect.size.height; j++) {
                if ((int)point.y == j) {
                    return true;
                }
            }
        }
    }
    return false;
}

static inline M9t M9IdentityMake() {
    M9t ret{};
    ret.m[0] = ret.m[4] = ret.m[8] = 1.0;
    ret.m[1] = ret.m[2] = ret.m[3] = 0.0;
    ret.m[5] = ret.m[6] = ret.m[7] = 0.0;
    return ret;
}

static inline M16t M16IdentityMake(){
    M16t ret{};
    ret.m[0] = ret.m[5] = ret.m[10] = ret.m[15] = 1.0;
    ret.m[1] = ret.m[2] = ret.m[3] = ret.m[4] = 0.0;
    ret.m[6] = ret.m[7] = ret.m[8] = ret.m[9] = 0.0;
    ret.m[11] = ret.m[12] = ret.m[13] = ret.m[14] = 0.0;
    return ret;
}

#pragma mark - Point 2 Type

static inline I2t I2Make(I1t x, I1t y) {
    I2t ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

static inline V2t V2Make(F1t x, F1t y) {
    V2t ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

#define P2Make V2Make

static inline V2t V2MakeF(F1t x){
    return V2Make(x, x);
}

static inline S2t S2Make(F1t width, F1t height) {
    S2t ret;
    ret.width = width;
    ret.height = height;
    return ret;
}


static inline V2t V2Add (V2t a, V2t b){
    return V2Make(a.x + b.x, a.y + b.y);
}

static inline V2t V2Multiply (V2t a, V2t b){
    return V2Make(a.x + b.x, a.y + b.y);
}

static inline V2t V2MultiplyScalar (V2t a, F1t b){
    return V2Make(a.x * b, a.y * b);
}

static inline V2t V2Subtract (V2t a, V2t b){
    return V2Make(a.x * b.x, a.y * b.y);
}

static inline V2t V2Divide (V2t a, V2t b){
    return V2Make(a.x / b.x, a.y / b.y);
}

static inline V2t V2DivideScalar (V2t a, F1t b){
    return V2Make(a.x / b, a.y / b);
}

static inline bool V2Bool(V2t a){
    if (a.x != 0 || a.y != 0) {
        return true;
    }
    return false;
}

static inline bool V2GreaterScalar(V2t a, F1t b){
    if (fabsf(a.x) > b || fabsf(a.y) > b) {
        return true;
    }
    return false;
}

NK_INLINE F1t V2Length(V2t vector)
{
    return sqrt(vector.v[0] * vector.v[0] + vector.v[1] * vector.v[1]);
}

NK_INLINE V2t V2Normalize(V2t vector)
{
    float scale = 1.0f / V2Length(vector);
    V2t v = V2MultiplyScalar(vector, scale);
    return v;
}

NK_INLINE F1t V2DotProduct(V2t vectorLeft, V2t vectorRight)
{
    return vectorLeft.v[0] * vectorRight.v[0] + vectorLeft.v[1] * vectorRight.v[1];
}

NK_INLINE F1t V2SquareDistance(V2t vectorLeft, V2t vectorRight){
    V2t diff = V2Subtract(vectorLeft, vectorRight);
    return V2DotProduct(diff, diff);
}

#pragma mark - Vector 3 Type

static inline V3t V3FromQ4(Q4t q1) {
    
    V3t  euler;
    
    double sqw = q1.w*q1.w;
    double sqx = q1.x*q1.x;
    double sqy = q1.y*q1.y;
    double sqz = q1.z*q1.z;
    
    double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    double test = q1.x*q1.y + q1.z*q1.w;
    if (test > 0.499*unit) { // singularity at north pole
        euler.x = 2. * atan2(q1.x,q1.w);
        euler.y = M_PI/2.;
        euler.z = 0.;
        
        return euler;
        
    }
    else if (test < -0.499*unit) { // singularity at south pole
        euler.x = -2. * atan2(q1.x,q1.w);
        euler.y = -M_PI/2.;
        euler.z = 0;
        
        return euler;
    }
    else {
        
        euler.x = atan2(2.*q1.y*q1.w-2*q1.x*q1.z , sqx - sqy - sqz + sqw);
        euler.y = asin(2.*test/unit);
        euler.z = atan2(2.*q1.x*q1.w-2*q1.y*q1.z , -sqx + sqy - sqz + sqw);
        
        return euler;
        
    }
    
    
}

static inline V3t V3Add(V3t vectorLeft, V3t vectorRight)
{
    V3t v = { vectorLeft.x + vectorRight.x,
        vectorLeft.y + vectorRight.y,
        vectorLeft.z + vectorRight.z };
    return v;
}

static inline V3t V3Subtract(V3t vectorLeft, V3t vectorRight)
{
    V3t v = { vectorLeft.x - vectorRight.x,
        vectorLeft.y - vectorRight.y,
        vectorLeft.z - vectorRight.z };
    return v;
}

static inline V3t V3Multiply(V3t vectorLeft, V3t vectorRight)
{
    V3t v = { vectorLeft.x * vectorRight.x,
        vectorLeft.y * vectorRight.y,
        vectorLeft.z * vectorRight.z };
    return v;
}

static inline V3t V3Divide(V3t vectorLeft, V3t vectorRight)
{
    V3t v = { vectorLeft.x / vectorRight.x,
        vectorLeft.y / vectorRight.y,
        vectorLeft.z / vectorRight.z };
    return v;
}

static inline V3t V3Negate(V3t vector)
{
    V3t v = { -vector.x, -vector.y, -vector.z};
    return v;
}

static inline V3t V3AddScalar(V3t vector,F1t value)
{
    V3t v = { vector.x + value,
        vector.y + value,
        vector.z + value };
    return v;
}

static inline V3t V3SubtractScalar(V3t vector,F1t value)
{
    V3t v = { vector.x - value,
        vector.y - value,
        vector.z - value };
    return v;
}

static inline V3t V3MultiplyScalar(V3t vector,F1t value)
{
    V3t v = { vector.x * value,
        vector.y * value,
        vector.z * value };
    return v;
}

static inline V3t V3DivideScalar(V3t vector,F1t value)
{
    V3t v = { vector.x / value,
        vector.y / value,
        vector.z / value };
    return v;
}


static inline F1t V3Length(V3t vector)
{
    return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

static inline F1t V3Largest(V3t vector){
    return std::max(std::max(vector.x,vector.y),vector.z);
}

static inline V3t V3UnitRetainAspect(V3t vector){
    return V3DivideScalar(vector, V3Largest(vector));
}

static inline V3t V3Normalize(V3t vector)
{
    F1t scale = 1.0f / V3Length(vector);
    V3t v = { vector.x * scale, vector.y * scale, vector.z * scale };
    return v;
}

static inline F1t V3DotProduct(V3t vectorLeft, V3t vectorRight)
{
    return vectorLeft.x * vectorRight.x + vectorLeft.y * vectorRight.y + vectorLeft.z * vectorRight.z;
}


static inline F1t V3Distance(V3t vectorStart, V3t vectorEnd)
{
    return V3Length(V3Subtract(vectorEnd, vectorStart));
}

static inline V3t V3Lerp(V3t vectorStart, V3t vectorEnd,F1t t)
{
    V3t v = { vectorStart.x + ((vectorEnd.x - vectorStart.x) * t),
        vectorStart.y + ((vectorEnd.y - vectorStart.y) * t),
        vectorStart.z + ((vectorEnd.z - vectorStart.z) * t) };
    return v;
}

static inline V3t V3CrossProduct(V3t vectorLeft, V3t vectorRight)
{
    V3t v = { vectorLeft.y * vectorRight.z - vectorLeft.z * vectorRight.y,
        vectorLeft.z * vectorRight.x - vectorLeft.x * vectorRight.z,
        vectorLeft.x * vectorRight.y - vectorLeft.y * vectorRight.x };
    return v;
}

static inline V3t V3Project(V3t vectorToProject, V3t projectionVector)
{
    F1t scale = V3DotProduct(projectionVector, vectorToProject) / V3DotProduct(projectionVector, projectionVector);
    V3t v = V3MultiplyScalar(projectionVector, scale);
    return v;
}

static inline V3t V3FromPoints(V3t start, V3t end){
    V3t ret = V3Subtract(end, start);
    V3Normalize(ret);
    return ret;
}

static inline V3t V3GetM16Translation(M16t M16) {
    return {M16.m30,M16.m31,M16.m32};
}

static inline V3t V3GetM16Scale(M16t M16) {
    V3t x_vec = {M16.m00,M16.m10,M16.m20};
    V3t y_vec = {M16.m01,M16.m11,M16.m21};
    V3t z_vec = {M16.m02,M16.m12,M16.m22};
    return {V3Length(x_vec), V3Length(y_vec), V3Length(z_vec)};
}

// --------------------------------------------------------------------------------------------
// This is an implementation of the famous Quake fast inverse square root algorithm. Although
// it comes from the Quake 3D code, which was released under the GPL, John Carmack has stated
// that this code was not written by him or his ID counterparts. The actual origins of this
// algorithm have never been definitively found, but enough different people have contributed
// to it that I believe it is safe to assume it's in the public domain.
// --------------------------------------------------------------------------------------------
static inline F1t InvSqrt(F1t x)
{
    F1t xhalf = 0.5f * x;
    U1t i = *(U1t*)&x;	// store floating-point bits in integer
    i = 0x5f3759d5 - (i >> 1);		// initial guess for Newton's method
    x = *(F1t*)&i;				// convert new bits into float
    x = x*(1.5f - xhalf*x*x);		// One round of Newton's method
    return x;
}
// END Fast invqrt code -----------------------------------------------------------------------
static inline F1t V3FastInverseMagnitude(V3t vector)
{
    return InvSqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}
static inline void V3SetFastNormalize(V3t *vector)
{
    F1t vecInverseMag = V3FastInverseMagnitude(*vector);
    if (vecInverseMag == 0.0)
    {
        vector->x = 1.0;
        vector->y = 0.0;
        vector->z = 0.0;
    }
    vector->x *= vecInverseMag;
    vector->y *= vecInverseMag;
    vector->z *= vecInverseMag;
}

static inline V3t V3FastNormalize(V3t vector)
{
    F1t vecInverseMag = V3FastInverseMagnitude(vector);
    if (vecInverseMag == 0.0)
    {
        vector.x = 1.0;
        vector.y = 0.0;
        vector.z = 0.0;
    }
    vector.x *= vecInverseMag;
    vector.y *= vecInverseMag;
    vector.z *= vecInverseMag;
    
    return vector;
}

static inline V3t V3RotatePoint(V3t p, float angle, V3t axis){
    V3t ax = V3Normalize(axis);
    
    float a = DEGREES_TO_RADIANS(angle);
    float sina = sin( a );
    float cosa = cos( a );
    float cosb = 1.0f - cosa;
    
    float nx = p.x*(ax.x*ax.x*cosb + cosa)
    + p.y*(ax.x*ax.y*cosb - ax.z*sina)
    + p.z*(ax.x*ax.z*cosb + ax.y*sina);
    float ny = p.x*(ax.y*ax.x*cosb + ax.z*sina)
    + p.y*(ax.y*ax.y*cosb + cosa)
    + p.z*(ax.y*ax.z*cosb - ax.x*sina);
    float nz = p.x*(ax.z*ax.x*cosb - ax.y*sina)
    + p.y*(ax.z*ax.y*cosb + ax.x*sina)
    + p.z*(ax.z*ax.z*cosb + cosa);
    p.x = nx; p.y = ny; p.z = nz;
    
    return p;
}

static inline bool V3Equal(V3t l, V3t r){
    if (l.x != r.x) return false;
    if (l.y != r.y) return false;
    if (l.z != r.z) return false;
    return true;
}

#pragma mark - Triangle 3 Type


static inline V3t V3GetTriangleSurfaceNormal(T3t triangle)
{
    
    V3t u = V3FromPoints(triangle.v2, triangle.v1);
    V3t v = V3FromPoints(triangle.v3, triangle.v1);
    
    V3t ret;
    ret.x = (u.y * v.z) - (u.z * v.y);
    ret.y = (u.z * v.x) - (u.x * v.z);
    ret.z = (u.x * v.y) - (u.y * v.x);
    return ret;
}

#pragma mark - Color Type

static inline HSVColor HSVfromRGB(V3t rgb)
{
    HSVColor hsv;
    
    F1t rgb_min, rgb_max;
    rgb_min = std::min(rgb.r, std::min(rgb.g, rgb.b));
    rgb_max = std::max(rgb.r, std::max(rgb.g, rgb.b));
    
    if (rgb_max == rgb_min) {
        hsv.hue = 0;
    } else if (rgb_max == rgb.r) {
        hsv.hue = 60.0f * ((rgb.g - rgb.b) / (rgb_max - rgb_min));
        hsv.hue = fmodf(hsv.hue, 360.0f);
    } else if (rgb_max == rgb.g) {
        hsv.hue = 60.0f * ((rgb.b - rgb.r) / (rgb_max - rgb_min)) + 120.0f;
    } else if (rgb_max == rgb.b) {
        hsv.hue = 60.0f * ((rgb.r - rgb.g) / (rgb_max - rgb_min)) + 240.0f;
    }
    hsv.val = rgb_max;
    if (rgb_max == 0) {
        hsv.sat = 0;
    } else {
        hsv.sat = 1.0 - (rgb_min / rgb_max);
    }
    
    return hsv;
}

//static inline void NOCColorComponentsForColor(F1t *components, Color *color)
//{
//    const CGFloat *myColor = CGColorGetComponents(color.CGColor);
//    int numColorComponents = CGColorGetNumberOfComponents(color.CGColor);
//    if(numColorComponents == 4){
//        components[0] = myColor[0];
//        components[1] = myColor[1];
//        components[2] = myColor[2];
//        components[3] = myColor[3];
//    }else{
//        if(numColorComponents == 2){
//            components[0] = myColor[0];
//            components[1] = myColor[0];
//            components[2] = myColor[0];
//            components[3] = myColor[1];
//        }else{
//            NSLog(@"ERROR: Could not find 4 color components. Found %i", numColorComponents);
//            components[0] = 1.0f;
//            components[1] = 1.0f;
//            components[2] = 1.0f;
//            components[3] = 1.0f;
//        }
//    }
//}

#pragma mark - unsigned byte 4 type

static inline bool UB4Equal(UB4t l, UB4t r){
    if (l.x != r.x) return false;
    if (l.y != r.y) return false;
    if (l.z != r.z) return false;
    if (l.w != r.w) return false;
    return true;
}

#pragma mark - Vector 4 type

//static inline float cblend(F1t col, F1t bl){
//    return ((col * bl) + (1. - bl));
//}

static inline bool V4Equal(V4t l, V4t r){
    if (l.x != r.x) return false;
    if (l.y != r.y) return false;
    if (l.z != r.z) return false;
    if (l.w != r.w) return false;
    return true;
}

/**
 * Returns the determinant of the specified 2x2 matrix values.
 *
 *   | a1 b1 |
 *   | a2 b2 |
 */
static inline F1t M4Det(F1t a1, F1t a2, F1t b1, F1t b2) {
    return a1 * b2 - b1 * a2;
}

#define C4Equal V4Equal


//
//void getOrthonormals(V3t  normal, V3t  orthonormal1, V3t  *orthonormal2)
//{
//    M16t OrthoX;
//    M16RotateX(90, OrthoX);
//
//    M16t OrthoY;
//    M16RotateY(90, OrthoY);
//
//    V3t  w = transformByMatrix(normal, &OrthoX);
//
//   F1t dot = normal.dot(w);
//
//    if (fabsf(dot) > 0.6)
//    {
//        w = transformByMatrix(normal, &OrthoY);
//        OrthoY * normal;
//    }
//    w.normalize();
//
//    *orthonormal1 = normal.cross(w);
//    orthonormal1->normalize();
//    *orthonormal2 = normal.cross(*orthonormal1);
//    orthonormal2->normalize();
//}

//F1t getQuaternionTwist(Q4t q, V3t  axis)
//{
//    axis.normalize();
//
//    //get the plane the axis is a normal of
//    V3t  orthonormal1, orthonormal2;
//
//    getOrthonormals(axis, &orthonormal1, &orthonormal2);
//
//    V3t  transformed = orthonormal1 * q;
//
//    //project transformed vector onto plane
//    V3t  flattened = transformed - transformed.dot(axis) * axis;
//    flattened.normalize();
//
//
//    //get angle between original vector and projected transform to get angle around normal
//   F1t a = (float)acosf(orthonormal1.dot(flattened));
//
//    return a;
//
//}

//V3t  transformByMatrix(V3t  v, M16t* m)
//{
//    V3t  result;
//    for ( int i = 0; i < 4; ++i )
//        result.m[i] = v[0] * m->_mat[0][i] + v[1] * m->_mat[1][i] + v[2] + m->_mat[2][i] + v[3] * m->_mat[3][i];
//    result.m[0] = result.m[0]/result.m[3];
//    result.m[1] = result.m[1]/result.m[3];
//    result.m[2] = result.m[2]/result.m[3];
//    return result.m;
//}

//static inline void getSwingTwistQuaternions( const Q4t& rotation,
//                                     const V3t &      direction,
//                                     Q4t&       swing,
//                                     Q4t&       twist)
//{
//    V3t  ra = rotation.asVec3(); // rotation axis
//    V3t  p = parallelProjection(ra,direction); // projection( ra, direction ); // return projection v1 on to v2  (parallel component)
//    twist.set( p.x, p.y, p.z, rotation.w() );
//    twist.normalize();
//    swing = rotation * twist.conj();
//}
//
//V3t  parallelProjection(V3t  vec1, V3t  vec2){
//    V3t  t = vec2.normalized();
//    return t*vec1.dot(t);
//}

#pragma mark - Q4 - QUATERNION TYPE



//static inline Q4t Q4FromV3(V3t  vector,F1t scalar)
//{
//    Q4t q = { vector.x, vector.y, vector.z, scalar };
//    return q;
//}

static inline Q4t Q4FromV3(V3t euldeg)
{
    Q4t quat;
    V3t eul = V3t(DEGREES_TO_RADIANS(euldeg.z),DEGREES_TO_RADIANS(euldeg.y),DEGREES_TO_RADIANS(euldeg.x));
    
    F1t cr, cp, cy, sr, sp, sy, cpcy, spsy;
    // calculate trig identities
    cr = cos(eul.z/2.);
    cp = cos(eul.y/2.);
    cy = cos(eul.x/2.);
    sr = sin(eul.z/2.);
    sp = sin(eul.y/2.);
    sy = sin(eul.x/2.);
    cpcy = cp * cy;
    spsy = sp * sy;
    quat.w = cr * cpcy + sr * spsy;
    quat.x = sr * cpcy - cr * spsy;
    quat.y = cr * sp * cy + sr * cp * sy;
    quat.z = cr * cp * sy - sr * sp * cy;
    
    return quat;
}

static inline Q4t Q4FromArray(F1t values[4])
{
#if defined(USE_SSE3)
    __m128 v = _mm_load_ps(values);
    return *(Q4t *)&v;
#else
    Q4t q = { values[0], values[1], values[2], values[3] };
    return q;
#endif
}

static inline Q4t Q4FromA4(A4t A4)
{
    F1t halfAngle = A4.a * 0.5f;
    F1t scale = sinf(halfAngle);
    Q4t q = { scale * A4.x, scale * A4.y, scale * A4.z, cosf(halfAngle) };
    return q;
}

static inline Q4t Q4FromAngleAndV3(F1t degrees, V3t  axisVector)
{
    return Q4FromA4(A4Make(DEGREES_TO_RADIANS(degrees), axisVector.x, axisVector.y, axisVector.z));
}

static inline Q4t Q4FromAngleAndAxes(F1t degrees,F1t x,F1t y,F1t z)
{
    return Q4FromA4(A4Make(DEGREES_TO_RADIANS(degrees), x, y, z));
}

static inline Q4t Q4Add(Q4t quaternionLeft, Q4t quaternionRight)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vaddq_f32(*(float32x4_t *)&quaternionLeft,
                              *(float32x4_t *)&quaternionRight);
    return *(Q4t *)&v;
#elif defined(USE_SSE3)
    __m128 v = _mm_load_ps(&quaternionLeft.x) + _mm_load_ps(&quaternionRight.x);
    return *(Q4t *)&v;
#else
    Q4t q = { quaternionLeft.x + quaternionRight.x,
        quaternionLeft.y + quaternionRight.y,
        quaternionLeft.z + quaternionRight.z,
        quaternionLeft.w + quaternionRight.w };
    return q;
#endif
}

/* Return product of quaternion q by scalar w. */
static inline Q4t Q4Scale(Q4t q, double w)
{
    Q4t qq;
    
    qq.w = q.w*w;
    qq.x = q.x*w;
    qq.y = q.y*w;
    qq.z = q.z*w;
    
    return (qq);
}

// THIS IS AN ALTERNATE Q4 GETTER THAN OF VERSION

static inline F1t SIGN(F1t x) {return (x >= 0.0f) ? +1.0f : -1.0f;}
static inline F1t NORM(F1t a,F1t b,F1t c,F1t d) {return sqrt(a * a + b * b + c * c + d * d);}

static inline Q4t Q4GetM16Rotate(M16t M16){
    
    Q4t quaternion;
    
    quaternion.x = ( M16.m11 + M16.m22 + M16.m33 + 1.0f) / 4.0f;
    quaternion.y = ( M16.m11 - M16.m22 - M16.m33 + 1.0f) / 4.0f;
    quaternion.z = (-M16.m11 + M16.m22 - M16.m33 + 1.0f) / 4.0f;
    quaternion.w = (-M16.m11 - M16.m22 + M16.m33 + 1.0f) / 4.0f;
    
    if(quaternion.x < 0.0f) quaternion.x = 0.0f;
    if(quaternion.y < 0.0f) quaternion.y = 0.0f;
    if(quaternion.z < 0.0f) quaternion.z = 0.0f;
    if(quaternion.w < 0.0f) quaternion.w = 0.0f;
    quaternion.x = sqrt(quaternion.x);
    quaternion.y = sqrt(quaternion.y);
    quaternion.z = sqrt(quaternion.z);
    quaternion.w = sqrt(quaternion.w);
    if(quaternion.x >= quaternion.y && quaternion.x >= quaternion.z && quaternion.x >= quaternion.w) {
        quaternion.x *= +1.0f;
        quaternion.y *= SIGN(M16.m32 - M16.m23);
        quaternion.z *= SIGN(M16.m13 - M16.m31);
        quaternion.w *= SIGN(M16.m21 - M16.m12);
    } else if(quaternion.y >= quaternion.x && quaternion.y >= quaternion.z && quaternion.y >= quaternion.w) {
        quaternion.x *= SIGN(M16.m32 - M16.m23);
        quaternion.y *= +1.0f;
        quaternion.z *= SIGN(M16.m21 + M16.m12);
        quaternion.w *= SIGN(M16.m13 + M16.m31);
    } else if(quaternion.z >= quaternion.x && quaternion.z >= quaternion.y && quaternion.z >= quaternion.w) {
        quaternion.x *= SIGN(M16.m13 - M16.m31);
        quaternion.y *= SIGN(M16.m21 + M16.m12);
        quaternion.z *= +1.0f;
        quaternion.w *= SIGN(M16.m32 + M16.m23);
    } else if(quaternion.w >= quaternion.x && quaternion.w >= quaternion.y && quaternion.w >= quaternion.z) {
        quaternion.x *= SIGN(M16.m21 - M16.m12);
        quaternion.y *= SIGN(M16.m31 + M16.m13);
        quaternion.z *= SIGN(M16.m32 + M16.m23);
        quaternion.w *= +1.0f;
    } else {
        printf("Q4 from Matrix: coding error\n");
    }
    
    F1t r = NORM(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
    quaternion.x /= r;
    quaternion.y /= r;
    quaternion.z /= r;
    quaternion.w /= r;
    
    return quaternion;
    
}

static inline Q4t Q4MultiplyM16(M16t matrixLeft, Q4t vectorRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrix = *(float32x4x4_t *)&matrixLeft;
    float32x4_t v;
    
    iMatrix.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)vectorRight.x);
    iMatrix.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)vectorRight.y);
    iMatrix.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)vectorRight.y);
    iMatrix.val[3] = vmulq_n_f32(iMatrix.val[3], (float32_t)vectorRight.z);
    
    iMatrix.val[0] = vaddq_f32(iMatrix.val[0], iMatrix.val[1]);
    iMatrix.val[2] = vaddq_f32(iMatrix.val[2], iMatrix.val[3]);
    
    v = vaddq_f32(iMatrix.val[0], iMatrix.val[2]);
    
    return *(Q4t *)&v;
#elif defined(USE_SSE3)
    const __m128 v = _mm_load_ps(&vectorRight.x);
    
    const __m128 r = _mm_load_ps(&matrixLeft.m[0])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0))
    + _mm_load_ps(&matrixLeft.m[4])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1))
    + _mm_load_ps(&matrixLeft.m[8])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2))
    + _mm_load_ps(&matrixLeft.m[12]) * _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
    
    Q4t ret;
    *(__m128*)&ret = r;
    return ret;
#else
    Q4t v = { matrixLeft.m[0] * vectorRight.x + matrixLeft.m[4] * vectorRight.y + matrixLeft.m[8] * vectorRight.y + matrixLeft.m[12] * vectorRight.z,
        matrixLeft.m[1] * vectorRight.x + matrixLeft.m[5] * vectorRight.y + matrixLeft.m[9] * vectorRight.y + matrixLeft.m[13] * vectorRight.z,
        matrixLeft.m[2] * vectorRight.x + matrixLeft.m[6] * vectorRight.y + matrixLeft.m[10] * vectorRight.y + matrixLeft.m[14] * vectorRight.z,
        matrixLeft.m[3] * vectorRight.x + matrixLeft.m[7] * vectorRight.y + matrixLeft.m[11] * vectorRight.y + matrixLeft.m[15] * vectorRight.z };
    return v;
#endif
}


/* Construct a unit quaternion from rotation matrix.  Assumes matrix is
 * used to multiply column vector on the left: vnew = mat vold.  Works
 * correctly for right-handed coordinate system and right-handed rotations.
 * Translation and perspective components ignored. */

static inline Q4t Q4FromMatrix(M16t mat)
{
    /* This algorithm avoids near-zero divides by looking for a large component
     * - first w, then x, y, or z.  When the trace is greater than zero,
     * |w| is greater than 1/2, which is as small as a largest component can be.
     * Otherwise, the largest diagonal entry corresponds to the largest of |x|,
     * |y|, or |z|, one of which must be larger than |w|, and at least 1/2. */
    Q4t qu = Q4t(0,0,0,1);
    double tr, s;
    
    tr = mat.m00 + mat.m11 + mat.m22;
    if (tr >= 0.0)
    {
        s = sqrt(tr + mat.m33);
        qu.w = s*0.5;
        s = 0.5 / s;
        qu.x = (mat.m21 - mat.m12) * s;
        qu.y = (mat.m02 - mat.m20) * s;
        qu.z = (mat.m10 - mat.m01) * s;
    }
    else
    {
        int h = 0;
        if (mat.m11 > mat.m00) h = 1;
        if (mat.m22 > mat.colRow[h][h]) h = 2;
        switch (h) {
#define caseMacro(i,j,k,I,J,K) \
case I:\
s = sqrt( (mat.colRow[I][I] - (mat.colRow[J][J]+mat.colRow[K][K])) + mat.colRow[3][3] );\
qu.i = s*0.5;\
s = 0.5 / s;\
qu.j = (mat.colRow[I][J] + mat.colRow[J][I]) * s;\
qu.k = (mat.colRow[K][I] + mat.colRow[I][K]) * s;\
qu.w = (mat.colRow[K][J] - mat.colRow[J][K]) * s;\
break
                caseMacro(x,y,z,0,1,2);
                caseMacro(y,z,x,1,2,0);
                caseMacro(z,x,y,2,0,1);
        }
    }
    if (mat.colRow[3][3] != 1.0) qu = Q4Scale(qu, 1/sqrt(mat.colRow[3][3]));
    return (qu);
}



static inline Q4t Q4Subtract(Q4t quaternionLeft, Q4t quaternionRight)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vsubq_f32(*(float32x4_t *)&quaternionLeft,
                              *(float32x4_t *)&quaternionRight);
    return *(Q4t *)&v;
#elif defined(USE_SSE3)
    __m128 v = _mm_load_ps(&quaternionLeft.x) - _mm_load_ps(&quaternionRight.x);
    return *(Q4t *)&v;
#else
    Q4t q = { quaternionLeft.x - quaternionRight.x,
        quaternionLeft.y - quaternionRight.y,
        quaternionLeft.z - quaternionRight.z,
        quaternionLeft.w - quaternionRight.w };
    return q;
#endif
}

static inline Q4t Q4Multiply(Q4t quaternionLeft, Q4t quaternionRight)
{
#if defined(USE_SSE3)
    const __m128 ql = _mm_load_ps(&quaternionLeft.x);
    const __m128 qr = _mm_load_ps(&quaternionRight.x);
    
    const __m128 ql3012 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(2, 1, 0, 3));
    const __m128 ql3120 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(0, 2, 1, 3));
    const __m128 ql3201 = _mm_shuffle_ps(ql, ql, _MM_SHUFFLE(1, 0, 2, 3));
    
    const __m128 qr0321 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(1, 2, 3, 0));
    const __m128 qr1302 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(2, 0, 3, 1));
    const __m128 qr2310 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(0, 1, 3, 2));
    const __m128 qr3012 = _mm_shuffle_ps(qr, qr, _MM_SHUFFLE(2, 1, 0, 3));
    
    uint32_t signBit = 0x80000000;
    uint32_t zeroBit = 0x0;
    uint32_t __attribute__((aligned(16))) mask0001[4] = {zeroBit, zeroBit, zeroBit, signBit};
    uint32_t __attribute__((aligned(16))) mask0111[4] = {zeroBit, signBit, signBit, signBit};
    const __m128 m0001 = _mm_load_ps((F1t *)mask0001);
    const __m128 m0111 = _mm_load_ps((F1t *)mask0111);
    
    const __m128 aline = ql3012 * _mm_xor_ps(qr0321, m0001);
    const __m128 bline = ql3120 * _mm_xor_ps(qr1302, m0001);
    const __m128 cline = ql3201 * _mm_xor_ps(qr2310, m0001);
    const __m128 dline = ql3012 * _mm_xor_ps(qr3012, m0111);
    const __m128 r = _mm_hadd_ps(_mm_hadd_ps(aline, bline), _mm_hadd_ps(cline, dline));
    
    return *(Q4t *)&r;
#else
    
    Q4t q = { quaternionLeft.w * quaternionRight.x +
        quaternionLeft.x * quaternionRight.w +
        quaternionLeft.y * quaternionRight.z -
        quaternionLeft.z * quaternionRight.y,
        
        quaternionLeft.w * quaternionRight.y +
        quaternionLeft.y * quaternionRight.w +
        quaternionLeft.z * quaternionRight.x -
        quaternionLeft.x * quaternionRight.z,
        
        quaternionLeft.w * quaternionRight.z +
        quaternionLeft.z * quaternionRight.w +
        quaternionLeft.x * quaternionRight.y -
        quaternionLeft.y * quaternionRight.x,
        
        quaternionLeft.w * quaternionRight.w -
        quaternionLeft.x * quaternionRight.x -
        quaternionLeft.y * quaternionRight.y -
        quaternionLeft.z * quaternionRight.z };
    return q;
#endif
}

static inline F1t Q4Length(Q4t quaternion)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vmulq_f32(*(float32x4_t *)&quaternion,
                              *(float32x4_t *)&quaternion);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    return sqrt(vget_lane_f32(v2, 0));
#elif defined(USE_SSE3)
    const __m128 q = _mm_load_ps(&quaternion.x);
    const __m128 product = q * q;
    const __m128 halfsum = _mm_hadd_ps(product, product);
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(halfsum, halfsum)));
#else
    return sqrt(quaternion.x * quaternion.x +
                quaternion.y * quaternion.y +
                quaternion.z * quaternion.z +
                quaternion.w * quaternion.w);
#endif
}

static inline Q4t Q4Normalize(Q4t quaternion)
{
    F1t scale = 1.0f / Q4Length(quaternion);
#if defined(__ARM_NEON__)
    float32x4_t v = vmulq_f32(*(float32x4_t *)&quaternion,
                              vdupq_n_f32((float32_t)scale));
    return *(Q4t *)&v;
#elif defined(USE_SSE3)
    const __m128 q = _mm_load_ps(&quaternion.x);
    __m128 v = q * _mm_set1_ps(scale);
    return *(Q4t *)&v;
#else
    Q4t q = { quaternion.x * scale, quaternion.y * scale, quaternion.z * scale, quaternion.w * scale };
    return q;
#endif
}

static inline Q4t Q4Conjugate(Q4t quaternion)
{
#if defined(__ARM_NEON__)
    float32x4_t *q = (float32x4_t *)&quaternion;
    
    uint32_t signBit = 0x80000000;
    uint32_t zeroBit = 0x0;
    uint32x4_t mask = vdupq_n_u32(signBit);
    mask = vsetq_lane_u32(zeroBit, mask, 3);
    *q = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(*q), mask));
    
    return *(Q4t *)q;
#elif defined(USE_SSE3)
    // Multiply first three elements by -1
    const uint32_t signBit = 0x80000000;
    const uint32_t zeroBit = 0x0;
    const uint32_t __attribute__((aligned(16))) mask[4] = {signBit, signBit, signBit, zeroBit};
    __m128 v_mask = _mm_load_ps((F1t *)mask);
    const __m128 q = _mm_load_ps(&quaternion.x);
    __m128 v = _mm_xor_ps(q, v_mask);
    
    return *(Q4t *)&v;
#else
    Q4t q = { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
    return q;
#endif
}

static inline Q4t Q4Invert(Q4t quaternion)
{
#if defined(__ARM_NEON__)
    float32x4_t *q = (float32x4_t *)&quaternion;
    float32x4_t v = vmulq_f32(*q, *q);
    float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
    v2 = vpadd_f32(v2, v2);
    float32_t scale = 1.0f / vget_lane_f32(v2, 0);
    v = vmulq_f32(*q, vdupq_n_f32(scale));
    
    uint32_t signBit = 0x80000000;
    uint32_t zeroBit = 0x0;
    uint32x4_t mask = vdupq_n_u32(signBit);
    mask = vsetq_lane_u32(zeroBit, mask, 3);
    v = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v), mask));
    
    return *(Q4t *)&v;
#elif defined(USE_SSE3)
    const __m128 q = _mm_load_ps(&quaternion.x);
    const uint32_t signBit = 0x80000000;
    const uint32_t zeroBit = 0x0;
    const uint32_t __attribute__((aligned(16))) mask[4] = {signBit, signBit, signBit, zeroBit};
    const __m128 v_mask = _mm_load_ps((F1t *)mask);
    const __m128 product = q * q;
    const __m128 halfsum = _mm_hadd_ps(product, product);
    const __m128 v = _mm_xor_ps(q, v_mask) / _mm_hadd_ps(halfsum, halfsum);
    return *(Q4t *)&v;
#else
    F1t scale = 1.0f / (quaternion.x * quaternion.x +
                        quaternion.y * quaternion.y +
                        quaternion.z * quaternion.z +
                        quaternion.w * quaternion.w);
    Q4t q = { -quaternion.x * scale, -quaternion.y * scale, -quaternion.z * scale, quaternion.w * scale };
    return q;
#endif
}

static inline V3t  Q4RotateVector3(Q4t quaternion, V3t  vector)
{
    Q4t rotatedQuaternion = Q4(vector.x, vector.y, vector.z, 0.0f);
    rotatedQuaternion = Q4Multiply(Q4Multiply(quaternion, rotatedQuaternion), Q4Invert(quaternion));
    
    return V3(rotatedQuaternion.x, rotatedQuaternion.y, rotatedQuaternion.z);
}

static inline Q4t Q4RotateQ4(Q4t quaternion, Q4t vector)
{
    Q4t rotatedQuaternion = Q4(vector.x, vector.y, vector.z, 0.0f);
    rotatedQuaternion = Q4Multiply(Q4Multiply(quaternion, rotatedQuaternion), Q4Invert(quaternion));
    
    return Q4(rotatedQuaternion.x, rotatedQuaternion.y, rotatedQuaternion.z, vector.w);
}

static inline Q4t QuatSlerp(Q4t from, Q4t to,F1t t)
{
    Q4t res;
    F1t DELTA = .01; // THRESHOLD FOR LINEAR INTERP
    
    F1t           to1[4];
    double        omega, cosom, sinom, scale0, scale1;
    // calc cosine
    cosom = from.x * to.x + from.y * to.y + from.z * to.z
    + from.w * to.w;
    // adjust signs (if necessary)
    if ( cosom <0.0 ){ cosom = -cosom; to1[0] = - to.x;
        to1[1] = - to.y;
        to1[2] = - to.z;
        to1[3] = - to.w;
    } else  {
        to1[0] = to.x;
        to1[1] = to.y;
        to1[2] = to.z;
        to1[3] = to.w;
    }
    // calculate coefficients
    if ( (1.0 - cosom) > DELTA ) {
        // standard case (slerp)
        omega = acos(cosom);
        sinom = sin(omega);
        scale0 = sin((1.0 - t) * omega) / sinom;
        scale1 = sin(t * omega) / sinom;
    } else {
        // "from" and "to" quaternions are very close
        //  ... so we can do a linear interpolation
        scale0 = 1.0 - t;
        scale1 = t;
    }
    // calculate final values
    res.x = scale0 * from.x + scale1 * to1[0];
    res.y = scale0 * from.y + scale1 * to1[1];
    res.z = scale0 * from.z + scale1 * to1[2];
    res.w = scale0 * from.w + scale1 * to1[3];
    
    return res;
}

static inline Q4t QuatMul(Q4t q1, Q4t q2){
    
    Q4t res;
    
    F1t A, B, C, D, E, F, G, H;
    A = (q1.w + q1.x)*(q2.w + q2.x);
    B = (q1.z - q1.y)*(q2.y - q2.z);
    C = (q1.w - q1.x)*(q2.y + q2.z);
    D = (q1.y + q1.z)*(q2.w - q2.x);
    E = (q1.x + q1.z)*(q2.x + q2.y);
    F = (q1.x - q1.z)*(q2.x - q2.y);
    G = (q1.w + q1.y)*(q2.w - q2.z);
    H = (q1.w - q1.y)*(q2.w + q2.z);
    res.w = B + (-E - F + G + H) /2;
    res.x = A - (E + F + G + H)/2;
    res.y = C + (E - F + G - H)/2;
    res.z = D + (E - F - G + H)/2;
    
    return res;
}

#pragma mark - Angle Axis 4 Type



static inline A4t A4FromQuat(Q4t q1) {
    
    A4t A4;
    
    if (q1.w > 1) Q4Normalize(q1); // if w>1 acos and sqrt will produce errors, this cant happen if quaternion is normalised
    A4.a = 2. * acos(q1.w);
    double s = sqrt(1-q1.w*q1.w); // assuming quaternion normalised then w is less than 1, so term always positive.
    if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
        // if s close to zero then direction of axis not important
        A4.x = q1.x; // if it is important that axis is normalised then replace with x=1; y=z=0;
        A4.y = q1.y;
        A4.z = q1.z;
    } else {
        A4.x = q1.x / s; // normalise axis
        A4.y = q1.y / s;
        A4.z = q1.z / s;
    }
    
    return A4;
    
}

#pragma mark - M9 - MATRIX 3x3 Type

/**
 * Returns the determinant of the specified 3x3 matrix values.
 *
 *  | a1 b1 c1 |
 *  | a2 b2 c2 |
 *  | a3 b3 c3 |
 */
static inline GLfloat M9Det(GLfloat a1, GLfloat a2, GLfloat a3,
                            GLfloat b1, GLfloat b2, GLfloat b3,
                            GLfloat c1, GLfloat c2, GLfloat c3) {
    return	a1 * M4Det(b2, b3, c2, c3) -
    b1 * M4Det(a2, a3, c2, c3) +
    c1 * M4Det(a2, a3, b2, b3);
}

static inline M9t M16GetM9(M16t matrix)
{
    M9t m = { matrix.m[0], matrix.m[1], matrix.m[2],
        matrix.m[4], matrix.m[5], matrix.m[6],
        matrix.m[8], matrix.m[9], matrix.m[10] };
    return m;
}

static inline void M9Transpose(M9t* mtx) {
    F1t tmp;
    tmp = mtx->m01;   mtx->m01 = mtx->m10;   mtx->m10 = tmp;
    tmp = mtx->m02;   mtx->m02 = mtx->m20;   mtx->m20 = tmp;
    tmp = mtx->m12;   mtx->m12 = mtx->m21;   mtx->m21 = tmp;
}

static inline V3t M9TransformV3(M9t* mtx, V3t v) {
    V3t vOut;
    vOut.x = (mtx->m00 * v.x) + (mtx->m10 * v.y) + (mtx->m20 * v.z);
    vOut.y = (mtx->m01 * v.x) + (mtx->m11 * v.y) + (mtx->m21 * v.z);
    vOut.z = (mtx->m02 * v.x) + (mtx->m12 * v.y) + (mtx->m22 * v.z);
    return vOut;
}

static inline M9t M9Multiply(M9t l, M9t r){
    M9t res = { l.m[0]*r.m[0],
        l.m[1]*r.m[1],
        l.m[2]*r.m[2],
        l.m[3]*r.m[3],
        l.m[4]*r.m[4],
        l.m[5]*r.m[5],
        l.m[6]*r.m[6],
        l.m[7]*r.m[7],
        l.m[8]*r.m[8]};
    return res;
}

#pragma mark - M16 - MATRIX 4x4 TYPE

static inline M16t M16Multiply(M16t matrixLeft, M16t matrixRight)
{
    
    // [ 0 4  8 12 ]   [ 0 4  8 12 ]
    // [ 1 5  9 13 ] x [ 1 5  9 13 ]
    // [ 2 6 10 14 ]   [ 2 6 10 14 ]
    // [ 3 7 11 15 ]   [ 3 7 11 15 ]
    
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&matrixLeft;
    float32x4x4_t iMatrixRight = *(float32x4x4_t *)&matrixRight;
    float32x4x4_t m;
    
    m.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
    m.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 0));
    m.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[2], 0));
    m.val[3] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[3], 0));
    
    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 1));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[3], 1));
    
    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 2));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[3], 2));
    
    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[0], 3));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[1], 3));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[2], 3));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[3], 3));
    
    return *(M16t *)&m;
    
#elif defined(USE_SSE3)
    const __m128 l0 = _mm_load_ps(&matrixLeft.m[0]);
    const __m128 l1 = _mm_load_ps(&matrixLeft.m[4]);
    const __m128 l2 = _mm_load_ps(&matrixLeft.m[8]);
    const __m128 l3 = _mm_load_ps(&matrixLeft.m[12]);
    
    const __m128 r0 = _mm_load_ps(&matrixRight.m[0]);
    const __m128 r1 = _mm_load_ps(&matrixRight.m[4]);
    const __m128 r2 = _mm_load_ps(&matrixRight.m[8]);
    const __m128 r3 = _mm_load_ps(&matrixRight.m[12]);
    
    const __m128 m0 = l0 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(0, 0, 0, 0))
    + l1 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(1, 1, 1, 1))
    + l2 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(2, 2, 2, 2))
    + l3 * _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(3, 3, 3, 3));
    
    const __m128 m1 = l0 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(0, 0, 0, 0))
    + l1 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 1, 1))
    + l2 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(2, 2, 2, 2))
    + l3 * _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 3, 3, 3));
    
    const __m128 m2 = l0 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(0, 0, 0, 0))
    + l1 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(1, 1, 1, 1))
    + l2 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(2, 2, 2, 2))
    + l3 * _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(3, 3, 3, 3));
    
    const __m128 m3 = l0 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(0, 0, 0, 0))
    + l1 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(1, 1, 1, 1))
    + l2 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(2, 2, 2, 2))
    + l3 * _mm_shuffle_ps(r3, r3, _MM_SHUFFLE(3, 3, 3, 3));
    
    M16t m;
    _mm_store_ps(&m.m[0], m0);
    _mm_store_ps(&m.m[4], m1);
    _mm_store_ps(&m.m[8], m2);
    _mm_store_ps(&m.m[12], m3);
    return m;
    
#else
#warning vector operations using non-vectorized library - find something better?
    M16t m;
    
    m.m[0]  = matrixLeft.m[0] * matrixRight.m[0]  + matrixLeft.m[4] * matrixRight.m[1]  + matrixLeft.m[8] * matrixRight.m[2]   + matrixLeft.m[12] * matrixRight.m[3];
    m.m[4]  = matrixLeft.m[0] * matrixRight.m[4]  + matrixLeft.m[4] * matrixRight.m[5]  + matrixLeft.m[8] * matrixRight.m[6]   + matrixLeft.m[12] * matrixRight.m[7];
    m.m[8]  = matrixLeft.m[0] * matrixRight.m[8]  + matrixLeft.m[4] * matrixRight.m[9]  + matrixLeft.m[8] * matrixRight.m[10]  + matrixLeft.m[12] * matrixRight.m[11];
    m.m[12] = matrixLeft.m[0] * matrixRight.m[12] + matrixLeft.m[4] * matrixRight.m[13] + matrixLeft.m[8] * matrixRight.m[14]  + matrixLeft.m[12] * matrixRight.m[15];
    
    m.m[1]  = matrixLeft.m[1] * matrixRight.m[0]  + matrixLeft.m[5] * matrixRight.m[1]  + matrixLeft.m[9] * matrixRight.m[2]   + matrixLeft.m[13] * matrixRight.m[3];
    m.m[5]  = matrixLeft.m[1] * matrixRight.m[4]  + matrixLeft.m[5] * matrixRight.m[5]  + matrixLeft.m[9] * matrixRight.m[6]   + matrixLeft.m[13] * matrixRight.m[7];
    m.m[9]  = matrixLeft.m[1] * matrixRight.m[8]  + matrixLeft.m[5] * matrixRight.m[9]  + matrixLeft.m[9] * matrixRight.m[10]  + matrixLeft.m[13] * matrixRight.m[11];
    m.m[13] = matrixLeft.m[1] * matrixRight.m[12] + matrixLeft.m[5] * matrixRight.m[13] + matrixLeft.m[9] * matrixRight.m[14]  + matrixLeft.m[13] * matrixRight.m[15];
    
    m.m[2]  = matrixLeft.m[2] * matrixRight.m[0]  + matrixLeft.m[6] * matrixRight.m[1]  + matrixLeft.m[10] * matrixRight.m[2]  + matrixLeft.m[14] * matrixRight.m[3];
    m.m[6]  = matrixLeft.m[2] * matrixRight.m[4]  + matrixLeft.m[6] * matrixRight.m[5]  + matrixLeft.m[10] * matrixRight.m[6]  + matrixLeft.m[14] * matrixRight.m[7];
    m.m[10] = matrixLeft.m[2] * matrixRight.m[8]  + matrixLeft.m[6] * matrixRight.m[9]  + matrixLeft.m[10] * matrixRight.m[10] + matrixLeft.m[14] * matrixRight.m[11];
    m.m[14] = matrixLeft.m[2] * matrixRight.m[12] + matrixLeft.m[6] * matrixRight.m[13] + matrixLeft.m[10] * matrixRight.m[14] + matrixLeft.m[14] * matrixRight.m[15];
    
    m.m[3]  = matrixLeft.m[3] * matrixRight.m[0]  + matrixLeft.m[7] * matrixRight.m[1]  + matrixLeft.m[11] * matrixRight.m[2]  + matrixLeft.m[15] * matrixRight.m[3];
    m.m[7]  = matrixLeft.m[3] * matrixRight.m[4]  + matrixLeft.m[7] * matrixRight.m[5]  + matrixLeft.m[11] * matrixRight.m[6]  + matrixLeft.m[15] * matrixRight.m[7];
    m.m[11] = matrixLeft.m[3] * matrixRight.m[8]  + matrixLeft.m[7] * matrixRight.m[9]  + matrixLeft.m[11] * matrixRight.m[10] + matrixLeft.m[15] * matrixRight.m[11];
    m.m[15] = matrixLeft.m[3] * matrixRight.m[12] + matrixLeft.m[7] * matrixRight.m[13] + matrixLeft.m[11] * matrixRight.m[14] + matrixLeft.m[15] * matrixRight.m[15];
    
    return m;
#endif
}

static inline M16t M16MakeTranslate(F1t x,F1t y,F1t z)
{
    M16t M16 = M16IdentityMake();
    // Translate slots.
    M16.m30 = x;
    M16.m31 = y;
    M16.m32 = z;
    
    return M16;
}

static inline void M16SetV3Translation(M16t *M16, V3t V3)
{
    M16->m30 = V3.x;
    M16->m31 = V3.y;
    M16->m32 = V3.z;
}

static inline M16t M16Translate(M16t matrix,F1t tx,F1t ty,F1t tz)
{
    M16t m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
        matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
        matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
        matrix.m[0] * tx + matrix.m[4] * ty + matrix.m[8] * tz + matrix.m[12],
        matrix.m[1] * tx + matrix.m[5] * ty + matrix.m[9] * tz + matrix.m[13],
        matrix.m[2] * tx + matrix.m[6] * ty + matrix.m[10] * tz + matrix.m[14],
        matrix.m[15] };
    return m;
}

static inline M16t M16TranslateWithV3(M16t matrix, V3t translationVector)
{
    M16t m = { matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
        matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
        matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
        matrix.m[0] * translationVector.v[0] + matrix.m[4] * translationVector.v[1] + matrix.m[8] * translationVector.v[2] + matrix.m[12],
        matrix.m[1] * translationVector.v[0] + matrix.m[5] * translationVector.v[1] + matrix.m[9] * translationVector.v[2] + matrix.m[13],
        matrix.m[2] * translationVector.v[0] + matrix.m[6] * translationVector.v[1] + matrix.m[10] * translationVector.v[2] + matrix.m[14],
        matrix.m[15] };
    return m;
}


static inline void M16SetQ4Rotation(M16t *M16, Q4t Q4){
    
    double length2 = Q4Length(Q4);
    
    if (fabs(length2) <= DBL_MIN)
    {
        M16->m00 = 1.0; M16->m10 = 0.0; M16->m20 = 0.0;
        M16->m01 = 0.0; M16->m11 = 1.0; M16->m21 = 0.0;
        M16->m02 = 0.0; M16->m12 = 0.0; M16->m22 = 1.0;
    }
    else
    {
        double rlength2;
        // normalize quat if required.
        // We can avoid the expensive sqrt in this case since all 'coefficients' below are products of two q components.
        // That is a square of a square root, so it is possible to avoid that
        if (length2 != 1.0)
        {
            rlength2 = 2.0/length2;
        }
        else
        {
            rlength2 = 2.0;
        }
        
        // Source: Gamasutra, Rotating Objects Using Quaternions
        //
        //http://www.gamasutra.com/features/19980703/quaternions_01.htm
        
        double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
        
        // calculate coefficients
        x2 = rlength2*Q4.x;
        y2 = rlength2*Q4.y;
        z2 = rlength2*Q4.z;
        
        xx = Q4.x * x2;
        xy = Q4.x * y2;
        xz = Q4.x * z2;
        
        yy = Q4.y * y2;
        yz = Q4.y * z2;
        zz = Q4.z * z2;
        
        wx = Q4.w * x2;
        wy = Q4.w * y2;
        wz = Q4.w * z2;
        
        // Note.  Gamasutra gets the matrix assignments inverted, resulting
        // in left-handed rotations, which is contrary to OpenGL and OSG's
        // methodology.  The matrix assignment has been altered in the next
        // few lines of code to do the right thing.
        // Don Burns - Oct 13, 2001
        M16->m00 = 1.0 - (yy + zz);
        M16->m10 = xy - wz;
        M16->m20 = xz + wy;
        
        
        M16->m01 = xy + wz;
        M16->m11 = 1.0 - (xx + zz);
        M16->m21 = yz - wx;
        
        M16->m02 = xz - wy;
        M16->m12 = yz + wx;
        M16->m22 = 1.0 - (xx + yy);
    }
    
    //#if 0
    //        _mat[0][3] = 0.0;
    //        _mat[1][3] = 0.0;
    //        _mat[2][3] = 0.0;
    //
    //        _mat[3][0] = 0.0;
    //        _mat[3][1] = 0.0;
    //        _mat[3][2] = 0.0;
    //        _mat[3][3] = 1.0;
    //#endif
    
}

static inline M16t M16MakeRotate(Q4t Q4){
    M16t M16 = M16IdentityMake();
    M16SetQ4Rotation(&M16, Q4);
    return M16;
}

static inline M16t M16MakeScale(V3t scale)
{
    M16t M16 = M16IdentityMake();
    
    // Scale slots.
    M16.m[0] = scale.x;
    M16.m[5] = scale.y;
    M16.m[10] = scale.z;
    
    return M16;
}

static inline M16t M164Scale(M16t matrix,F1t sx,F1t sy,F1t sz)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrix = *(float32x4x4_t *)&matrix;
    float32x4x4_t m;
    
    m.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)sx);
    m.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)sy);
    m.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)sz);
    m.val[3] = iMatrix.val[3];
    
    return *(M16t *)&m;
#elif defined(USE_SSE3)
    M16t m;
    
    _mm_store_ps(&m.m[0],  _mm_load_ps(&matrix.m[0])  * _mm_load1_ps(&sx));
    _mm_store_ps(&m.m[4],  _mm_load_ps(&matrix.m[4])  * _mm_load1_ps(&sy));
    _mm_store_ps(&m.m[8],  _mm_load_ps(&matrix.m[8])  * _mm_load1_ps(&sz));
    _mm_store_ps(&m.m[12], _mm_load_ps(&matrix.m[12]));
    
    return m;
#else
    
    
    M16t m = { matrix.m[0] * sx, matrix.m[1] * sx, matrix.m[2] * sx, matrix.m[3] * sx,
        matrix.m[4] * sy, matrix.m[5] * sy, matrix.m[6] * sy, matrix.m[7] * sy,
        matrix.m[8] * sz, matrix.m[9] * sz, matrix.m[10] * sz, matrix.m[11] * sz,
        matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
    return m;
#endif
}

static inline M16t M16ScaleWithV3(M16t matrix, V3t scaleVector)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrix = *(float32x4x4_t *)&matrix;
    float32x4x4_t m;
    
    m.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)scaleVector.v[0]);
    m.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)scaleVector.v[1]);
    m.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)scaleVector.v[2]);
    m.val[3] = iMatrix.val[3];
    
    return *(M16t *)&m;
#elif defined(USE_SSE3)
    M16t m;
    
    _mm_store_ps(&m.m[0],  _mm_load_ps(&matrix.m[0])  * _mm_load1_ps(&scaleVector.v[0]));
    _mm_store_ps(&m.m[4],  _mm_load_ps(&matrix.m[4])  * _mm_load1_ps(&scaleVector.v[1]));
    _mm_store_ps(&m.m[8],  _mm_load_ps(&matrix.m[8])  * _mm_load1_ps(&scaleVector.v[2]));
    _mm_store_ps(&m.m[12], _mm_load_ps(&matrix.m[12]));
    
    return m;
#else
    M16t m = { matrix.m[0] * scaleVector.v[0], matrix.m[1] * scaleVector.v[0], matrix.m[2] * scaleVector.v[0], matrix.m[3] * scaleVector.v[0],
        matrix.m[4] * scaleVector.v[1], matrix.m[5] * scaleVector.v[1], matrix.m[6] * scaleVector.v[1], matrix.m[7] * scaleVector.v[1],
        matrix.m[8] * scaleVector.v[2], matrix.m[9] * scaleVector.v[2], matrix.m[10] * scaleVector.v[2], matrix.m[11] * scaleVector.v[2],
        matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15] };
    return m;
#endif
}

static inline M16t M16MakeRotateX(F1t degrees)
{
    F1t radians = DEGREES_TO_RADIANS(degrees);
    
    M16t M16 = M16IdentityMake();
    
    // Rotate X formula.
    M16.m[5] = cosf(radians);
    M16.m[6] = -sinf(radians);
    M16.m[9] = -M16.m[6];
    M16.m[10] = M16.m[5];
    
    return M16;
}

static inline M16t M16MakeRotateY(F1t degrees)
{
    F1t radians = DEGREES_TO_RADIANS(degrees);
    
    M16t M16 = M16IdentityMake();
    
    // Rotate Y formula.
    M16.m[0] = cosf(radians);
    M16.m[2] = sinf(radians);
    M16.m[8] = -M16.m[2];
    M16.m[10] = M16.m[0];
    
    return M16;
}

static inline M16t M16MakeRotateZ(F1t degrees)
{
    F1t radians = DEGREES_TO_RADIANS(degrees);
    
    M16t M16 = M16IdentityMake();
    
    // Rotate Z formula.
    M16.m[0] = cosf(radians);
    M16.m[1] = sinf(radians);
    M16.m[4] = -M16.m[1];
    M16.m[5] = M16.m[0];
    
    return M16;
}

static inline V4t M16MultiplyV4(M16t matrixLeft, V4t vectorRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrix = *(float32x4x4_t *)&matrixLeft;
    float32x4_t v;
    
    iMatrix.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)vectorRight.v[0]);
    iMatrix.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)vectorRight.v[1]);
    iMatrix.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)vectorRight.v[2]);
    iMatrix.val[3] = vmulq_n_f32(iMatrix.val[3], (float32_t)vectorRight.v[3]);
    
    iMatrix.val[0] = vaddq_f32(iMatrix.val[0], iMatrix.val[1]);
    iMatrix.val[2] = vaddq_f32(iMatrix.val[2], iMatrix.val[3]);
    
    v = vaddq_f32(iMatrix.val[0], iMatrix.val[2]);
    
    return *(V4t *)&v;
#elif defined(USE_SSE3)
    const __m128 v = _mm_load_ps(&vectorRight.v[0]);
    
    const __m128 r = _mm_load_ps(&matrixLeft.m[0])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0))
    + _mm_load_ps(&matrixLeft.m[4])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1))
    + _mm_load_ps(&matrixLeft.m[8])  * _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2))
    + _mm_load_ps(&matrixLeft.m[12]) * _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
    
    V4t ret;
    *(__m128*)&ret = r;
    return ret;
#else
    V4t v = { matrixLeft.m[0] * vectorRight.v[0] + matrixLeft.m[4] * vectorRight.v[1] + matrixLeft.m[8] * vectorRight.v[2] + matrixLeft.m[12] * vectorRight.v[3],
        matrixLeft.m[1] * vectorRight.v[0] + matrixLeft.m[5] * vectorRight.v[1] + matrixLeft.m[9] * vectorRight.v[2] + matrixLeft.m[13] * vectorRight.v[3],
        matrixLeft.m[2] * vectorRight.v[0] + matrixLeft.m[6] * vectorRight.v[1] + matrixLeft.m[10] * vectorRight.v[2] + matrixLeft.m[14] * vectorRight.v[3],
        matrixLeft.m[3] * vectorRight.v[0] + matrixLeft.m[7] * vectorRight.v[1] + matrixLeft.m[11] * vectorRight.v[2] + matrixLeft.m[15] * vectorRight.v[3] };
    return v;
#endif
}


static inline V3t V3MultiplyM16(M16t matrixLeft, V3t vectorRight)
{
    return M16MultiplyV4(matrixLeft, V4(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 0.0f)).xyz;
}



static inline V3t V3MultiplyM16WithTranslation(M16t matrixLeft, V3t vectorRight)
{
    return M16MultiplyV4(matrixLeft, V4(vectorRight.v[0], vectorRight.v[1], vectorRight.v[2], 1.0f)).xyz;
}


//static inline V3t V3MultiplyM16(M16t m, V3t v)
//{
//    V3t res;
//
//	res.x = ( v.x * m.v[ 0 ].x ) +
//    ( v.y * m.v[ 1 ].x ) +
//    ( v.z * m.v[ 2 ].x );
//
//	res.y = ( v.x * m.v[ 0 ].y ) +
//    ( v.y * m.v[ 1 ].y ) +
//    ( v.z * m.v[ 2 ].y );
//
//	res.z = ( v.x * m.v[ 0 ].z ) +
//    ( v.y * m.v[ 1 ].z ) +
//    ( v.z * m.v[ 2 ].z );
//
//    return res;
//}

static inline V3t EulerMultiplyM16(M16t matrixLeft, V3t vectorRight)

{
    Q4t Q4 = Q4MultiplyM16(matrixLeft, Q4FromV3(vectorRight));
    return V3(Q4.x, Q4.y, Q4.z);
}


// from http://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions

static inline M16t M16MakeAngleAxis(F1t radians, V3t  V3){
    
    M16t M16;
    
    F1t ax = V3.x * radians;
    F1t ay = V3.y * radians;
    F1t az = V3.z * radians;
    
    // First Column
    M16.m[0] = cosf(ay) * cosf(az);
    M16.m[1] = -cosf(ay)*sin(az);
    M16.m[2] = sin(ay);
    M16.m[3] = 0;
    //Second
    M16.m[4] = cosf(ax) * sinf(az) + sinf(ax) * sinf(ay) * cosf(az);
    M16.m[5] = cosf(ax) * cosf(az) - sinf(ax) * sinf(ay) * sinf(az);
    M16.m[6] = -sinf(ax) * cosf(ay);
    M16.m[7] = 0;
    //Third
    M16.m[8] = cosf(ax) * sinf(az) - cosf(ax) * sinf(ay) * cosf(az);
    M16.m[9] = sinf(ax) * cosf(az) + cosf(ax) * sinf(ay) * sinf(az);
    M16.m[10] = cosf(ax) * cosf(ay);
    M16.m[11] = 0;
    //Fourth
    M16.m[12] = 0;
    M16.m[13] = 0;
    M16.m[14] = 0;
    M16.m[15] = 1;
    
    return M16;
}

static inline M16t M16MakeEuler(V3t euler) {
    V3t rad = V3(DEGREES_TO_RADIANS(euler.x), DEGREES_TO_RADIANS(euler.y),DEGREES_TO_RADIANS(euler.z));
    return M16MakeAngleAxis(1., rad);
}

static inline M16t M16MakePerspective(F1t fovyRadians,F1t aspect,F1t nearZ,F1t farZ)
{
    F1t cotan = 1.0f / tanf(fovyRadians / 2.0f);
    
    M16t m = { cotan / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, cotan, 0.0f, 0.0f,
        0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), -1.0f,
        0.0f, 0.0f, (2.0f * farZ * nearZ) / (nearZ - farZ), 0.0f };
    
    return m;
}

static inline void M16LoadPerspective(M16t* M16, F1t fovDegrees, F1t aspect, F1t nearZ, F1t farZ)
{
    float f = 1.0f / tanf( (fovDegrees * (M_PI/180)) / 2.0f);
    
    float* mtx = M16->m;
    
    mtx[0] = f / aspect;
    mtx[1] = 0.0f;
    mtx[2] = 0.0f;
    mtx[3] = 0.0f;
    
    mtx[4] = 0.0f;
    mtx[5] = f;
    mtx[6] = 0.0f;
    mtx[7] = 0.0f;
    
    mtx[8] = 0.0f;
    mtx[9] = 0.0f;
    mtx[10] = (farZ+nearZ) / (nearZ-farZ);
    mtx[11] = -1.0f;
    
    mtx[12] = 0.0f;
    mtx[13] = 0.0f;
    mtx[14] = 2 * farZ * nearZ /  (nearZ-farZ);
    mtx[15] = 0.0f;
}

static inline M16t M16MakeFrustum(F1t left,F1t right,
                                  F1t bottom,F1t top,
                                  F1t nearZ,F1t farZ)
{
    F1t ral = right + left;
    F1t rsl = right - left;
    F1t tsb = top - bottom;
    F1t tab = top + bottom;
    F1t fan = farZ + nearZ;
    F1t fsn = farZ - nearZ;
    
    M16t m = { 2.0f * nearZ / rsl, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f * nearZ / tsb, 0.0f, 0.0f,
        ral / rsl, tab / tsb, -fan / fsn, -1.0f,
        0.0f, 0.0f, (-2.0f * farZ * nearZ) / fsn, 0.0f };
    
    return m;
}

static inline M16t M16MakeOrtho(F1t left,F1t right,
                                F1t bottom,F1t top,
                                F1t nearZ,F1t farZ)
{
    F1t ral = right + left;
    F1t rsl = right - left;
    F1t tab = top + bottom;
    F1t tsb = top - bottom;
    F1t fan = farZ + nearZ;
    F1t fsn = farZ - nearZ;
    
    M16t m = { 2.0f / rsl, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / tsb, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / fsn, 0.0f,
        -ral / rsl, -tab / tsb, -fan / fsn, 1.0f };
    
    return m;
}

static inline void M16LookAt(M16t *mat, V3t eye, V3t center, V3t up )
{
    V3t n = V3Normalize(V3Subtract(eye, center));
    V3t u = V3Normalize(V3CrossProduct(up, n));
    V3t v = V3CrossProduct(n, u);
    
    mat->m00 = u.x;
    mat->m01 = u.y;
    mat->m02 = u.z;
    
    mat->m10 = v.x;
    mat->m11 = v.y;
    mat->m12 = v.z;
    
    mat->m20 = n.x;
    mat->m21 = n.y;
    mat->m22 = n.z;
}

static inline M16t M16MakeLookAt(V3t ev,V3t cv, V3t uv)
{
    V3t n = V3Normalize(V3Subtract(ev, cv));
    V3t u = V3Normalize(V3CrossProduct(uv, n));
    V3t v = V3CrossProduct(n, u);
    
    M16t m = { u.x, v.x, n.x, 0.0f,
        u.y, v.y, n.y, 0.0f,
        u.z, v.z, n.z, 0.0f,
        V3DotProduct(V3Negate(u), ev),
        V3DotProduct(V3Negate(v), ev),
        V3DotProduct(V3Negate(n), ev),
        1.0f };
    
    return m;
}

//static inline V4t M16GetRow(M16t matrix, int row)
//{
//    V4t v = { matrix.m[row], matrix.m[4 + row], matrix.m[8 + row], matrix.m[12 + row] };
//    return v;
//}

static inline V4t M16GetColumn(M16t matrix, int column)
{
#if defined(__ARM_NEON__)
    float32x4_t v = vld1q_f32(&(matrix.m[column * 4]));
    return *(V4t *)&v;
#elif defined(USE_SSE3)
    __m128 v = _mm_load_ps(&matrix.m[column * 4]);
    return *(V4t *)&v;
#else
    V4t v = { matrix.m[column * 4 + 0], matrix.m[column * 4 + 1], matrix.m[column * 4 + 2], matrix.m[column * 4 + 3] };
    return v;
#endif
}

//static inline M16t M16SetRow(M16t matrix, int row, V4t vector)
//{
//    matrix.m[row] = vector.v[0];
//    matrix.m[row + 4] = vector.v[1];
//    matrix.m[row + 8] = vector.v[2];
//    matrix.m[row + 12] = vector.v[3];
//
//    return matrix;
//}

static inline M16t M16FromM9(M9t* m9){
    M16t m16;
    for (int i = 0; i < 3; i++) {
        m16.m[i * 4 + 0] = m9->v[i].x;
        m16.m[i * 4 + 1] = m9->v[i].y;
        m16.m[i * 4 + 2] = m9->v[i].z;
    }
    return m16;
}

static inline void M16SetM9(M16t* m16, M9t* m9){
    for (int i = 0; i < 3; i++) {
        m16->m[i * 4 + 0] = m9->v[i].x;
        m16->m[i * 4 + 1] = m9->v[i].y;
        m16->m[i * 4 + 2] = m9->v[i].z;
    }
}

static inline M16t M16SetColumn(M16t matrix, int column, V4t vector)
{
#if defined(__ARM_NEON__)
    F1t *dst = &(matrix.m[column * 4]);
    vst1q_f32(dst, vld1q_f32(vector.v));
    return matrix;
#elif defined(USE_SSE3)
    *((__m128*)&matrix.m[column*4]) = *(__m128*)&vector;
    return matrix;
#else
    matrix.m[column * 4 + 0] = vector.v[0];
    matrix.m[column * 4 + 1] = vector.v[1];
    matrix.m[column * 4 + 2] = vector.v[2];
    matrix.m[column * 4 + 3] = vector.v[3];
    
    return matrix;
#endif
}


/*!
 Invert a 4x4 matrix fast.
 
 \param[in,out] m A valid 4x4 matrix that will be used for the inverse operation.
 
 \return Return 1 if the inverse is successfull, instead return 0.
 
 \sa mat4_invert_full
 */
static inline unsigned char M16Invert( M16t *m )
{
    M16t mat;
    
    float d = ( m->v[ 0 ].x * m->v[ 0 ].x +
               m->v[ 1 ].x * m->v[ 1 ].x +
               m->v[ 2 ].x * m->v[ 2 ].x );
    
    if( !d ) return 0;
    
    d = 1.0f / d;
    
    mat.v[ 0 ].x = d * m->v[ 0 ].x;
    mat.v[ 0 ].y = d * m->v[ 1 ].x;
    mat.v[ 0 ].z = d * m->v[ 2 ].x;
    
    mat.v[ 1 ].x = d * m->v[ 0 ].y;
    mat.v[ 1 ].y = d * m->v[ 1 ].y;
    mat.v[ 1 ].z = d * m->v[ 2 ].y;
    
    mat.v[ 2 ].x = d * m->v[ 0 ].z;
    mat.v[ 2 ].y = d * m->v[ 1 ].z;
    mat.v[ 2 ].z = d * m->v[ 2 ].z;
    
    mat.v[ 3 ].x = -( mat.v[ 0 ].x * m->v[ 3 ].x + mat.v[ 1 ].x * m->v[ 3 ].y + mat.v[ 2 ].x * m->v[ 3 ].z );
    mat.v[ 3 ].y = -( mat.v[ 0 ].y * m->v[ 3 ].x + mat.v[ 1 ].y * m->v[ 3 ].y + mat.v[ 2 ].y * m->v[ 3 ].z );
    mat.v[ 3 ].z = -( mat.v[ 0 ].z * m->v[ 3 ].x + mat.v[ 1 ].z * m->v[ 3 ].y + mat.v[ 2 ].z * m->v[ 3 ].z );
    
    mat.v[ 0 ].w =
    mat.v[ 1 ].w =
    mat.v[ 2 ].w = 0.0f;
    mat.v[ 3 ].w = 1.0f;
    
    memcpy(m->m, mat.m,sizeof(M16t));
    
    return 1;
}

static inline bool M16InvertAdjoint(M16t* m) {
    M16t adj;	// The adjoint matrix (inverse after dividing by determinant)
    
    // Create the transpose of the cofactors, as the classical adjoint of the matrix.
    adj.m00 =  M9Det(m->m11, m->m12, m->m13, m->m21, m->m22, m->m23, m->m31, m->m32, m->m33);
    adj.m01 = -M9Det(m->m01, m->m02, m->m03, m->m21, m->m22, m->m23, m->m31, m->m32, m->m33);
    adj.m02 =  M9Det(m->m01, m->m02, m->m03, m->m11, m->m12, m->m13, m->m31, m->m32, m->m33);
    adj.m03 = -M9Det(m->m01, m->m02, m->m03, m->m11, m->m12, m->m13, m->m21, m->m22, m->m23);
    
    adj.m10 = -M9Det(m->m10, m->m12, m->m13, m->m20, m->m22, m->m23, m->m30, m->m32, m->m33);
    adj.m11 =  M9Det(m->m00, m->m02, m->m03, m->m20, m->m22, m->m23, m->m30, m->m32, m->m33);
    adj.m12 = -M9Det(m->m00, m->m02, m->m03, m->m10, m->m12, m->m13, m->m30, m->m32, m->m33);
    adj.m13 =  M9Det(m->m00, m->m02, m->m03, m->m10, m->m12, m->m13, m->m20, m->m22, m->m23);
    
    adj.m20 =  M9Det(m->m10, m->m11, m->m13, m->m20, m->m21, m->m23, m->m30, m->m31, m->m33);
    adj.m21 = -M9Det(m->m00, m->m01, m->m03, m->m20, m->m21, m->m23, m->m30, m->m31, m->m33);
    adj.m22 =  M9Det(m->m00, m->m01, m->m03, m->m10, m->m11, m->m13, m->m30, m->m31, m->m33);
    adj.m23 = -M9Det(m->m00, m->m01, m->m03, m->m10, m->m11, m->m13, m->m20, m->m21, m->m23);
    
    adj.m30 = -M9Det(m->m10, m->m11, m->m12, m->m20, m->m21, m->m22, m->m30, m->m31, m->m32);
    adj.m31 =  M9Det(m->m00, m->m01, m->m02, m->m20, m->m21, m->m22, m->m30, m->m31, m->m32);
    adj.m32 = -M9Det(m->m00, m->m01, m->m02, m->m10, m->m11, m->m12, m->m30, m->m31, m->m32);
    adj.m33 =  M9Det(m->m00, m->m01, m->m02, m->m10, m->m11, m->m12, m->m20, m->m21, m->m22);
    
    // Calculate the determinant as a combination of the cofactors of the first row.
    GLfloat det = (adj.m00 * m->m00) + (adj.m01 * m->m10) + (adj.m02 * m->m20) + (adj.m03 * m->m30);
    
    // If determinant is zero, matrix is not invertable.
    //CC3AssertC(det != 0.0f, @"%@ is singular and cannot be inverted", NSStringFromCC3Matrix4x4(m));
    if (det == 0.0f) return false;
    
    // Divide the classical adjoint matrix by the determinant and set back into original matrix.
    GLfloat ooDet = 1.0 / det;		// Turn div into mult for speed
    m->m00 = adj.m00 * ooDet;
    m->m01 = adj.m01 * ooDet;
    m->m02 = adj.m02 * ooDet;
    m->m03 = adj.m03 * ooDet;
    m->m10 = adj.m10 * ooDet;
    m->m11 = adj.m11 * ooDet;
    m->m12 = adj.m12 * ooDet;
    m->m13 = adj.m13 * ooDet;
    m->m20 = adj.m20 * ooDet;
    m->m21 = adj.m21 * ooDet;
    m->m22 = adj.m22 * ooDet;
    m->m23 = adj.m23 * ooDet;
    m->m30 = adj.m30 * ooDet;
    m->m31 = adj.m31 * ooDet;
    m->m32 = adj.m32 * ooDet;
    m->m33 = adj.m33 * ooDet;
    
    return true;
}

static inline void M16InvertRigid(M16t* mtx) {
    // Extract and transpose the 3x3 linear matrix
    M9t linMtx = M16GetM9(*mtx);
    M9Transpose(&linMtx);
    // Extract the translation and transform it by the transposed linear matrix
    V3t t = M16GetColumn(*mtx, 3).xyz;
    t = M9TransformV3(&linMtx, V3Negate(t));
    
    // Populate the 4x4 matrix with the transposed rotation and transformed translation
    M16SetM9(mtx, &linMtx);
    mtx->m30 = t.x;
    mtx->m31 = t.y;
    mtx->m32 = t.z;
}


/*!
 Invert a 4x4 matrix fast.
 
 \param[in,out] m A valid 4x4 matrix that will be used for the inverse operation.
 
 \return Return 1 if the inverse is successfull, instead return 0.
 
 \sa mat4_invert
 */
static inline unsigned char M16FullInvert( M16t *m )
{
    M16t inv;
    
    float d;
    
    inv.v[ 0 ].x = m->v[ 1 ].y * m->v[ 2 ].z * m->v[ 3 ].w -
    m->v[ 1 ].y * m->v[ 2 ].w * m->v[ 3 ].z -
    m->v[ 2 ].y * m->v[ 1 ].z * m->v[ 3 ].w +
    m->v[ 2 ].y * m->v[ 1 ].w * m->v[ 3 ].z +
    m->v[ 3 ].y * m->v[ 1 ].z * m->v[ 2 ].w -
    m->v[ 3 ].y * m->v[ 1 ].w * m->v[ 2 ].z;
    
    inv.v[ 1 ].x = -m->v[ 1 ].x * m->v[ 2 ].z * m->v[ 3 ].w +
    m->v[ 1 ].x * m->v[ 2 ].w * m->v[ 3 ].z +
    m->v[ 2 ].x * m->v[ 1 ].z * m->v[ 3 ].w -
    m->v[ 2 ].x * m->v[ 1 ].w * m->v[ 3 ].z -
    m->v[ 3 ].x * m->v[ 1 ].z * m->v[ 2 ].w +
    m->v[ 3 ].x * m->v[ 1 ].w * m->v[ 2 ].z;
    
    inv.v[ 2 ].x = m->v[ 1 ].x * m->v[ 2 ].y * m->v[ 3 ].w -
    m->v[ 1 ].x * m->v[ 2 ].w * m->v[ 3 ].y -
    m->v[ 2 ].x * m->v[ 1 ].y * m->v[ 3 ].w +
    m->v[ 2 ].x * m->v[ 1 ].w * m->v[ 3 ].y +
    m->v[ 3 ].x * m->v[ 1 ].y * m->v[ 2 ].w -
    m->v[ 3 ].x * m->v[ 1 ].w * m->v[ 2 ].y;
    
    inv.v[ 3 ].x = -m->v[ 1 ].x * m->v[ 2 ].y * m->v[ 3 ].z +
    m->v[ 1 ].x * m->v[ 2 ].z * m->v[ 3 ].y +
    m->v[ 2 ].x * m->v[ 1 ].y * m->v[ 3 ].z -
    m->v[ 2 ].x * m->v[ 1 ].z * m->v[ 3 ].y -
    m->v[ 3 ].x * m->v[ 1 ].y * m->v[ 2 ].z +
    m->v[ 3 ].x * m->v[ 1 ].z * m->v[ 2 ].y;
    
    inv.v[ 0 ].y = -m->v[ 0 ].y * m->v[ 2 ].z * m->v[ 3 ].w +
    m->v[ 0 ].y * m->v[ 2 ].w * m->v[ 3 ].z +
    m->v[ 2 ].y * m->v[ 0 ].z * m->v[ 3 ].w -
    m->v[ 2 ].y * m->v[ 0 ].w * m->v[ 3 ].z -
    m->v[ 3 ].y * m->v[ 0 ].z * m->v[ 2 ].w +
    m->v[ 3 ].y * m->v[ 0 ].w * m->v[ 2 ].z;
    
    inv.v[ 1 ].y = m->v[ 0 ].x * m->v[ 2 ].z * m->v[ 3 ].w -
    m->v[ 0 ].x * m->v[ 2 ].w * m->v[ 3 ].z -
    m->v[ 2 ].x * m->v[ 0 ].z * m->v[ 3 ].w +
    m->v[ 2 ].x * m->v[ 0 ].w * m->v[ 3 ].z +
    m->v[ 3 ].x * m->v[ 0 ].z * m->v[ 2 ].w -
    m->v[ 3 ].x * m->v[ 0 ].w * m->v[ 2 ].z;
    
    inv.v[ 2 ].y = -m->v[ 0 ].x * m->v[ 2 ].y * m->v[ 3 ].w +
    m->v[ 0 ].x * m->v[ 2 ].w * m->v[ 3 ].y +
    m->v[ 2 ].x * m->v[ 0 ].y * m->v[ 3 ].w -
    m->v[ 2 ].x * m->v[ 0 ].w * m->v[ 3 ].y -
    m->v[ 3 ].x * m->v[ 0 ].y * m->v[ 2 ].w +
    m->v[ 3 ].x * m->v[ 0 ].w * m->v[ 2 ].y;
    
    inv.v[ 3 ].y = m->v[ 0 ].x * m->v[ 2 ].y * m->v[ 3 ].z -
    m->v[ 0 ].x * m->v[ 2 ].z * m->v[ 3 ].y -
    m->v[ 2 ].x * m->v[ 0 ].y * m->v[ 3 ].z +
    m->v[ 2 ].x * m->v[ 0 ].z * m->v[ 3 ].y +
    m->v[ 3 ].x * m->v[ 0 ].y * m->v[ 2 ].z -
    m->v[ 3 ].x * m->v[ 0 ].z * m->v[ 2 ].y;
    
    inv.v[ 0 ].z = m->v[ 0 ].y * m->v[ 1 ].z * m->v[ 3 ].w -
    m->v[ 0 ].y * m->v[ 1 ].w * m->v[ 3 ].z -
    m->v[ 1 ].y * m->v[ 0 ].z * m->v[ 3 ].w +
    m->v[ 1 ].y * m->v[ 0 ].w * m->v[ 3 ].z +
    m->v[ 3 ].y * m->v[ 0 ].z * m->v[ 1 ].w -
    m->v[ 3 ].y * m->v[ 0 ].w * m->v[ 1 ].z;
    
    inv.v[ 1 ].z = -m->v[ 0 ].x * m->v[ 1 ].z * m->v[ 3 ].w +
    m->v[ 0 ].x * m->v[ 1 ].w * m->v[ 3 ].z +
    m->v[ 1 ].x * m->v[ 0 ].z * m->v[ 3 ].w -
    m->v[ 1 ].x * m->v[ 0 ].w * m->v[ 3 ].z -
    m->v[ 3 ].x * m->v[ 0 ].z * m->v[ 1 ].w +
    m->v[ 3 ].x * m->v[ 0 ].w * m->v[ 1 ].z;
    
    inv.v[ 2 ].z = m->v[ 0 ].x * m->v[ 1 ].y * m->v[ 3 ].w -
    m->v[ 0 ].x * m->v[ 1 ].w * m->v[ 3 ].y -
    m->v[ 1 ].x * m->v[ 0 ].y * m->v[ 3 ].w +
    m->v[ 1 ].x * m->v[ 0 ].w * m->v[ 3 ].y +
    m->v[ 3 ].x * m->v[ 0 ].y * m->v[ 1 ].w -
    m->v[ 3 ].x * m->v[ 0 ].w * m->v[ 1 ].y;
    
    inv.v[ 3 ].z = -m->v[ 0 ].x * m->v[ 1 ].y * m->v[ 3 ].z +
    m->v[ 0 ].x * m->v[ 1 ].z * m->v[ 3 ].y +
    m->v[ 1 ].x * m->v[ 0 ].y * m->v[ 3 ].z -
    m->v[ 1 ].x * m->v[ 0 ].z * m->v[ 3 ].y -
    m->v[ 3 ].x * m->v[ 0 ].y * m->v[ 1 ].z +
    m->v[ 3 ].x * m->v[ 0 ].z * m->v[ 1 ].y;
    
    inv.v[ 0 ].w = -m->v[ 0 ].y * m->v[ 1 ].z * m->v[ 2 ].w +
    m->v[ 0 ].y * m->v[ 1 ].w * m->v[ 2 ].z +
    m->v[ 1 ].y * m->v[ 0 ].z * m->v[ 2 ].w -
    m->v[ 1 ].y * m->v[ 0 ].w * m->v[ 2 ].z -
    m->v[ 2 ].y * m->v[ 0 ].z * m->v[ 1 ].w +
    m->v[ 2 ].y * m->v[ 0 ].w * m->v[ 1 ].z;
    
    inv.v[ 1 ].w = m->v[ 0 ].x * m->v[ 1 ].z * m->v[ 2 ].w -
    m->v[ 0 ].x * m->v[ 1 ].w * m->v[ 2 ].z -
    m->v[ 1 ].x * m->v[ 0 ].z * m->v[ 2 ].w +
    m->v[ 1 ].x * m->v[ 0 ].w * m->v[ 2 ].z +
    m->v[ 2 ].x * m->v[ 0 ].z * m->v[ 1 ].w -
    m->v[ 2 ].x * m->v[ 0 ].w * m->v[ 1 ].z;
    
    inv.v[ 2 ].w = -m->v[ 0 ].x * m->v[ 1 ].y * m->v[ 2 ].w +
    m->v[ 0 ].x * m->v[ 1 ].w * m->v[ 2 ].y +
    m->v[ 1 ].x * m->v[ 0 ].y * m->v[ 2 ].w -
    m->v[ 1 ].x * m->v[ 0 ].w * m->v[ 2 ].y -
    m->v[ 2 ].x * m->v[ 0 ].y * m->v[ 1 ].w +
    m->v[ 2 ].x * m->v[ 0 ].w * m->v[ 1 ].y;
    
    inv.v[ 3 ].w = m->v[ 0 ].x * m->v[ 1 ].y * m->v[ 2 ].z -
    m->v[ 0 ].x * m->v[ 1 ].z * m->v[ 2 ].y -
    m->v[ 1 ].x * m->v[ 0 ].y * m->v[ 2 ].z +
    m->v[ 1 ].x * m->v[ 0 ].z * m->v[ 2 ].y +
    m->v[ 2 ].x * m->v[ 0 ].y * m->v[ 1 ].z -
    m->v[ 2 ].x * m->v[ 0 ].z * m->v[ 1 ].y;
    
    d = m->v[ 0 ].x * inv.v[ 0 ].x +
    m->v[ 0 ].y * inv.v[ 1 ].x +
    m->v[ 0 ].z * inv.v[ 2 ].x +
    m->v[ 0 ].w * inv.v[ 3 ].x;
    
    if( !d ) return 0;
    
    d = 1.0f / d;
    
    inv.v[ 0 ].x *= d;
    inv.v[ 0 ].y *= d;
    inv.v[ 0 ].z *= d;
    inv.v[ 0 ].w *= d;
    
    inv.v[ 1 ].x *= d;
    inv.v[ 1 ].y *= d;
    inv.v[ 1 ].z *= d;
    inv.v[ 1 ].w *= d;
    
    inv.v[ 2 ].x *= d;
    inv.v[ 2 ].y *= d;
    inv.v[ 2 ].z *= d;
    inv.v[ 2 ].w *= d;
    
    inv.v[ 3 ].x *= d;
    inv.v[ 3 ].y *= d;
    inv.v[ 3 ].z *= d;
    inv.v[ 3 ].w *= d;
    
    memcpy(m->m, inv.m,sizeof(M16t));
    
    return 1;
}


/*!
 Transpose a 4x4 matrix.
 
 \param[in,out] m A valid 4x4 matrix pointer to use as the source and destination of the transpose operation.
 */
static inline void M16Transpose( M16t *m )
{
    float t;
    
    t			= m->v[ 0 ].y;
    m->v[ 0 ].y = m->v[ 1 ].x;
    m->v[ 1 ].x = t;
    
    t			= m->v[ 0 ].z;
    m->v[ 0 ].z = m->v[ 2 ].x;
    m->v[ 2 ].x = t;
    
    t			= m->v[ 0 ].w;
    m->v[ 0 ].w = m->v[ 3 ].x;
    m->v[ 3 ].x = t;
    
    t			= m->v[ 1 ].z;
    m->v[ 1 ].z = m->v[ 2 ].y;
    m->v[ 2 ].y = t;
    
    t			= m->v[ 1 ].w ;
    m->v[ 1 ].w = m->v[ 3 ].y ;
    m->v[ 3 ].y = t;
    
    t			= m->v[ 2 ].w ;
    m->v[ 2 ].w = m->v[ 3 ].z ;
    m->v[ 3 ].z = t;
}


static inline M9t M16GetInverseNormalMatrix(M16t modelViewMatrix){
    
    M16t copy = modelViewMatrix;
    
    M16FullInvert(&copy);
    
    M16Transpose(&copy);
    
    return M16GetM9(copy);
}


#pragma mark - Logging Functions

static inline void nkLogV2(std::string name, V2t vec){
    nkLog("%s : x: %1.2f y: %1.2f", name, vec.x, vec.y);
}

static inline void NKLogV3(std::string name, V3t vec){
    printf("%s : x: %1.2f y: %1.2f z: %1.2f \n", name.c_str(), vec.x, vec.y, vec.z);
}

static inline void NKLogV4(std::string name, V4t vec){
    printf("%s : x: %1.2f y: %1.2f z:%1.2f w:%1.2f \n", name.c_str(), vec.x, vec.y, vec.z, vec.w);
}

static inline void NKLogM16(std::string name, M16t mat){
    printf("matrix %s \n \
           %f %f %f %f \n \
           %f %f %f %f \n \
           %f %f %f %f \n \
           %f %f %f %f \n",
           name.c_str(),
           mat.m00, mat.m10, mat.m20, mat.m30,
           mat.m01, mat.m11, mat.m21, mat.m31,
           mat.m02, mat.m12, mat.m22, mat.m32,
           mat.m03, mat.m13, mat.m23, mat.m33);
}

#pragma mark MATRIX - DECOMPOSITION - ported FROM OpenFrameworks, untested

//typedef double _HMatrix[4][4];
//static _HMatrix mat_id = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
//typedef Q4t HVect;
//
//enum QuatPart {X, Y, Z, W};
//
//typedef struct
//{
//	Q4t t;     // Translation Component;
//	Q4t q;           // Essential Rotation
//	Q4t u;          //Stretch rotation
//	HVect k;          //Sign of determinant
//	double f;          // Sign of determinant
//} _affineParts;

//#define SQRTHALF (0.7071067811865475244)
//#define qxtoz Q4(0,SQRTHALF,0,SQRTHALF)
//#define qytoz Q4(SQRTHALF,0,SQRTHALF,0)
//#define q0001 Q4(0,0,0,1)
//#define qppmm Q4( 0.5, 0.5,-0.5,-0.5)
//#define qpppp Q4( 0.5, 0.5, 0.5, 0.5)
//#define qmpmm Q4(-0.5, 0.5,-0.5,-0.5)
//#define q1000 Q4( 1.0, 0.0, 0.0, 0.0)
//#define qpppm Q4( 0.5, 0.5, 0.5,-0.5)

//// HELPERS FOR POLAR DECOMPOSITION
//
//#define matrixCopy(C, gets, A, n) {int i, j; for (i=0;i<n;i++) for (j=0;j<n;j++)\
//C[i][j] gets (A[i][j]);}
//
///** Copy transpose of nxn matrix A to C using "gets" for assignment **/
//#define mat_tpose(AT,gets,A,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
//AT[i][j] gets (A[j][i]);}
//
///** Fill out 3x3 matrix to 4x4 **/
//#define mat_pad(A) (A[W][X]=A[X][W]=A[W][Y]=A[Y][W]=A[W][Z]=A[Z][W]=0,A[W][W]=1)
//
///** Assign nxn matrix C the element-wise combination of A and B using "op" **/
//#define matBinop(C,gets,A,op,B,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
//C[i][j] gets (A[i][j]) op (B[i][j]);}
//
///** Copy nxn matrix A to C using "gets" for assignment **/
//#define mat_copy(C,gets,A,n) {int i,j; for(i=0;i<n;i++) for(j=0;j<n;j++)\
//C[i][j] gets (A[i][j]);}
//
//
//
///** Return index of column of M containing maximum abs entry, or -1 if M=0 **/
//static inline int find_max_col(_HMatrix M)
//{
//    double abs, max;
//    int i, j, col;
//    max = 0.0; col = -1;
//    for (i=0; i<3; i++) for (j=0; j<3; j++) {
//        abs = M[i][j]; if (abs<0.0) abs = -abs;
//        if (abs>max) {max = abs; col = j;}
//    }
//    return col;
//}
//
//static inline void vcross(double *va, double *vb, double *v)
//{
//    v[0] = va[1]*vb[2] - va[2]*vb[1];
//    v[1] = va[2]*vb[0] - va[0]*vb[2];
//    v[2] = va[0]*vb[1] - va[1]*vb[0];
//}
//
///** Return dot product of length 3 vectors va and vb **/
//static inline double vdot(double *va, double *vb)
//{
//    return (va[0]*vb[0] + va[1]*vb[1] + va[2]*vb[2]);
//}
//
///** Set MadjT to transpose of inverse of M times determinant of M **/
//static inline void adjoint_transpose(_HMatrix M, _HMatrix MadjT)
//{
//    vcross(M[1], M[2], MadjT[0]);
//    vcross(M[2], M[0], MadjT[1]);
//    vcross(M[0], M[1], MadjT[2]);
//}
//
///** Setup u for Household reflection to zero all v components but first **/
//static inline void make_reflector(double *v, double *u)
//{
//    double s = sqrt(vdot(v, v));
//    u[0] = v[0]; u[1] = v[1];
//    u[2] = v[2] + ((v[2]<0.0) ? -s : s);
//    s = sqrt(2.0/vdot(u, u));
//    u[0] = u[0]*s; u[1] = u[1]*s; u[2] = u[2]*s;
//}
//
///** Apply Householder reflection represented by u to column vectors of M **/
//static inline void reflect_cols(_HMatrix M, double *u)
//{
//    int i, j;
//    for (i=0; i<3; i++) {
//        double s = u[0]*M[0][i] + u[1]*M[1][i] + u[2]*M[2][i];
//        for (j=0; j<3; j++) M[j][i] -= u[j]*s;
//    }
//}
//
///** Apply Householder reflection represented by u to row vectors of M **/
//static inline void reflect_rows(_HMatrix M, double *u)
//{
//    int i, j;
//    for (i=0; i<3; i++) {
//        double s = vdot(u, M[i]);
//        for (j=0; j<3; j++) M[i][j] -= u[j]*s;
//    }
//}
//
///** Multiply the upper left 3x3 parts of A and B to get AB **/
//static inline void mat_mult(_HMatrix A, _HMatrix B, _HMatrix AB)
//{
//    int i, j;
//    for (i=0; i<3; i++) for (j=0; j<3; j++)
//        AB[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + A[i][2]*B[2][j];
//}
//
//static inline double mat_norm(_HMatrix M, int tpose)
//{
//    int i;
//    double sum, max;
//    max = 0.0;
//    for (i=0; i<3; i++) {
//        if (tpose) sum = fabs(M[0][i])+fabs(M[1][i])+fabs(M[2][i]);
//        else       sum = fabs(M[i][0])+fabs(M[i][1])+fabs(M[i][2]);
//        if (max<sum) max = sum;
//    }
//    return max;
//}
//
//
//static inline double norm_inf(_HMatrix M) {return mat_norm(M, 0);}
//static inline double norm_one(_HMatrix M) {return mat_norm(M, 1);}
//
//
///** Find orthogonal factor Q of rank 1 (or less) M **/
//static inline void do_rank1(_HMatrix M, _HMatrix Q)
//{
//    double v1[3], v2[3], s;
//    int col;
//    mat_copy(Q,=,mat_id,4);
//    /* If rank(M) is 1, we should find a non-zero column in M */
//    col = find_max_col(M);
//    if (col<0) return; /* Rank is 0 */
//    v1[0] = M[0][col]; v1[1] = M[1][col]; v1[2] = M[2][col];
//    make_reflector(v1, v1); reflect_cols(M, v1);
//    v2[0] = M[2][0]; v2[1] = M[2][1]; v2[2] = M[2][2];
//    make_reflector(v2, v2); reflect_rows(M, v2);
//    s = M[2][2];
//    if (s<0.0) Q[2][2] = -1.0;
//    reflect_cols(Q, v1); reflect_rows(Q, v2);
//}
//
///** Find orthogonal factor Q of rank 2 (or less) M using adjoint transpose **/
//static inline void do_rank2(_HMatrix M, _HMatrix MadjT, _HMatrix Q)
//{
//    double v1[3], v2[3];
//    double w, x, y, z, c, s, d;
//    int col;
//    /* If rank(M) is 2, we should find a non-zero column in MadjT */
//    col = find_max_col(MadjT);
//    if (col<0) {do_rank1(M, Q); return;} /* Rank<2 */
//    v1[0] = MadjT[0][col]; v1[1] = MadjT[1][col]; v1[2] = MadjT[2][col];
//    make_reflector(v1, v1); reflect_cols(M, v1);
//    vcross(M[0], M[1], v2);
//    make_reflector(v2, v2); reflect_rows(M, v2);
//    w = M[0][0]; x = M[0][1]; y = M[1][0]; z = M[1][1];
//    if (w*z>x*y) {
//        c = z+w; s = y-x; d = sqrt(c*c+s*s); c = c/d; s = s/d;
//        Q[0][0] = Q[1][1] = c; Q[0][1] = -(Q[1][0] = s);
//    } else {
//        c = z-w; s = y+x; d = sqrt(c*c+s*s); c = c/d; s = s/d;
//        Q[0][0] = -(Q[1][1] = c); Q[0][1] = Q[1][0] = s;
//    }
//    Q[0][2] = Q[2][0] = Q[1][2] = Q[2][1] = 0.0; Q[2][2] = 1.0;
//    reflect_cols(Q, v1); reflect_rows(Q, v2);
//}
//
//
//
//
//
///******* Polar Decomposition *******/
///* Polar Decomposition of 3x3 matrix in 4x4,
// * M = QS.  See Nicholas Higham and Robert S. Schreiber,
// * Fast Polar Decomposition of An Arbitrary Matrix,
// * Technical Report 88-942, October 1988,
// * Department of Computer Science, Cornell University.
// */
//
//static inline double polarDecomp( _HMatrix M, _HMatrix Q, _HMatrix S)
//{
//
//#define TOL 1.0e-6
//	_HMatrix Mk, MadjTk, Ek;
//	double det, M_one, M_inf, MadjT_one, MadjT_inf, E_one, gamma, g1, g2;
//	int i, j;
//
//	mat_tpose(Mk,=,M,3);
//	M_one = norm_one(Mk);  M_inf = norm_inf(Mk);
//
//	do
//	{
//		adjoint_transpose(Mk, MadjTk);
//		det = vdot(Mk[0], MadjTk[0]);
//		if (det==0.0)
//		{
//			do_rank2(Mk, MadjTk, Mk);
//			break;
//		}
//
//		MadjT_one = norm_one(MadjTk);
//		MadjT_inf = norm_inf(MadjTk);
//
//		gamma = sqrt(sqrt((MadjT_one*MadjT_inf)/(M_one*M_inf))/fabs(det));
//		g1 = gamma*0.5;
//		g2 = 0.5/(gamma*det);
//		matrixCopy(Ek,=,Mk,3);
//		matBinop(Mk,=,g1*Mk,+,g2*MadjTk,3);
//		mat_copy(Ek,-=,Mk,3);
//		E_one = norm_one(Ek);
//		M_one = norm_one(Mk);
//		M_inf = norm_inf(Mk);
//
//	} while(E_one>(M_one*TOL));
//
//	mat_tpose(Q,=,Mk,3); mat_pad(Q);
//	mat_mult(Mk, M, S);  mat_pad(S);
//
//	for (i=0; i<3; i++)
//		for (j=i; j<3; j++)
//			S[i][j] = S[j][i] = 0.5*(S[i][j]+S[j][i]);
//	return (det);
//}
//
//
///******* Spectral Decomposition *******/
///* Compute the spectral decomposition of symmetric positive semi-definite S.
// * Returns rotation in U and scale factors in result, so that if K is a diagonal
// * matrix of the scale factors, then S = U K (U transpose). Uses Jacobi method.
// * See Gene H. Golub and Charles F. Van Loan. Matrix Computations. Hopkins 1983.
// */
//static inline HVect spectDecomp(_HMatrix S, _HMatrix U)
//{
//    HVect kv;
//    double Diag[3],OffD[3]; /* OffD is off-diag (by omitted index) */
//    double g,h,fabsh,fabsOffDi,t,theta,c,s,tau,ta,OffDq,a,b;
//    static char nxt[] = {Y,Z,X};
//    int sweep, i, j;
//    mat_copy(U,=,mat_id,4);
//    Diag[X] = S[X][X]; Diag[Y] = S[Y][Y]; Diag[Z] = S[Z][Z];
//    OffD[X] = S[Y][Z]; OffD[Y] = S[Z][X]; OffD[Z] = S[X][Y];
//    for (sweep=20; sweep>0; sweep--) {
//        double sm = fabs(OffD[X])+fabs(OffD[Y])+fabs(OffD[Z]);
//        if (sm==0.0) break;
//        for (i=Z; i>=X; i--) {
//            int p = nxt[i]; int q = nxt[p];
//            fabsOffDi = fabs(OffD[i]);
//            g = 100.0*fabsOffDi;
//            if (fabsOffDi>0.0) {
//                h = Diag[q] - Diag[p];
//                fabsh = fabs(h);
//                if (fabsh+g==fabsh) {
//                    t = OffD[i]/h;
//                } else {
//                    theta = 0.5*h/OffD[i];
//                    t = 1.0/(fabs(theta)+sqrt(theta*theta+1.0));
//                    if (theta<0.0) t = -t;
//                }
//                c = 1.0/sqrt(t*t+1.0); s = t*c;
//                tau = s/(c+1.0);
//                ta = t*OffD[i]; OffD[i] = 0.0;
//                Diag[p] -= ta; Diag[q] += ta;
//                OffDq = OffD[q];
//                OffD[q] -= s*(OffD[p] + tau*OffD[q]);
//                OffD[p] += s*(OffDq   - tau*OffD[p]);
//                for (j=Z; j>=X; j--) {
//                    a = U[j][p]; b = U[j][q];
//                    U[j][p] -= s*(b + tau*a); //                }
//            }
//        }
//    }
//    kv.x = Diag[X]; kv.y = Diag[Y]; kv.z = Diag[Z]; kv.w = 1.0;
//    return (kv);
//}
//
///******* Spectral Axis Adjustment *******/
//
///* Given a unit quaternion, q, and a scale vector, k, find a unit quaternion, p,
// * which permutes the axes and turns freely in the plane of duplicate scale
// * factors, such that q p has the largest possible w component, i.e. the
// * smallest possible angle. Permutes k's components to go with q p instead of q.
// * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
// * Proceedings of Graphics Interface 1992. Details on p. 262-263.
// */
//static inline Q4t snuggle(Q4t q, HVect *k)
//{
//#define sgn(n,v)    ((n)?-(v):(v))
//#define swap(a,i,j) {a[3]=a[i]; a[i]=a[j]; a[j]=a[3];}
//#define cycle(a,p)  if (p) {a[3]=a[0]; a[0]=a[1]; a[1]=a[2]; a[2]=a[3];}\
//else   {a[3]=a[2]; a[2]=a[1]; a[1]=a[0]; a[0]=a[3];}
//
//	Q4t p = Q4(0,0,0,1);
//	double ka[4];
//	int i, turn = -1;
//	ka[X] = k->x; ka[Y] = k->y; ka[Z] = k->z;
//
//	if (ka[X]==ka[Y]) {
//		if (ka[X]==ka[Z])
//			turn = W;
//		else turn = Z;
//	}
//	else {
//		if (ka[X]==ka[Z])
//			turn = Y;
//		else if (ka[Y]==ka[Z])
//			turn = X;
//	}
//	if (turn>=0) {
//		Q4t qtoz, qp;
//		unsigned int  win;
//		double mag[3], t;
//		switch (turn) {
//			default: return (Q4Conjugate(q));
//			case X: q = Q4Multiply(q, qtoz = qxtoz); swap(ka,X,Z) break;
//			case Y: q = Q4Multiply(q, qtoz = qytoz); swap(ka,Y,Z) break;
//			case Z: qtoz = q0001; break;
//		}
//		q = Q4Conjugate(q);
//		mag[0] = (double)q.z*q.z+(double)q.w*q.w-0.5;
//		mag[1] = (double)q.x*q.z-(double)q.y*q.w;
//		mag[2] = (double)q.y*q.z+(double)q.x*q.w;
//
//		bool neg[3];
//		for (i=0; i<3; i++)
//		{
//			neg[i] = (mag[i]<0.0);
//			if (neg[i]) mag[i] = -mag[i];
//		}
//
//		if (mag[0]>mag[1]) {
//			if (mag[0]>mag[2])
//				win = 0;
//			else win = 2;
//		}
//		else {
//			if (mag[1]>mag[2]) win = 1;
//			else win = 2;
//		}
//
//		switch (win) {
//			case 0: if (neg[0]) p = q1000; else p = q0001; break;
//			case 1: if (neg[1]) p = qppmm; else p = qpppp; cycle(ka,0) break;
//			case 2: if (neg[2]) p = qmpmm; else p = qpppm; cycle(ka,1) break;
//		}
//
//		qp = Q4Multiply(q, p);
//		t = sqrt(mag[win]+0.5);
//		p = Q4Multiply(p, Q4(0.0,0.0,-qp.z/t,qp.w/t));
//		p = Q4Multiply(qtoz, Q4Conjugate(p));
//	}
//
//	else {
//		double qa[4], pa[4];
//		unsigned int lo, hi;
//		bool par = false;
//		bool neg[4];
//		double all, big, two;
//		qa[0] = q.x; qa[1] = q.y; qa[2] = q.z; qa[3] = q.w;
//		for (i=0; i<4; i++) {
//			pa[i] = 0.0;
//			neg[i] = (qa[i]<0.0);
//			if (neg[i]) qa[i] = -qa[i];
//			par ^= neg[i];
//		}
//
//		/* Find two largest components, indices in hi and lo */
//		if (qa[0]>qa[1]) lo = 0;
//		else lo = 1;
//
//		if (qa[2]>qa[3]) hi = 2;
//		else hi = 3;
//
//		if (qa[lo]>qa[hi]) {
//			if (qa[lo^1]>qa[hi]) {
//				hi = lo; lo ^= 1;
//			}
//			else {
//				hi ^= lo; lo ^= hi; hi ^= lo;
//			}
//		}
//		else {
//			if (qa[hi^1]>qa[lo]) lo = hi^1;
//		}
//
//		all = (qa[0]+qa[1]+qa[2]+qa[3])*0.5;
//		two = (qa[hi]+qa[lo])*SQRTHALF;
//		big = qa[hi];
//		if (all>two) {
//			if (all>big) {/*all*/
//				{int i; for (i=0; i<4; i++) pa[i] = sgn(neg[i], 0.5);}
//				cycle(ka,par);
//			}
//			else {/*big*/ pa[hi] = sgn(neg[hi],1.0);}
//		} else {
//			if (two>big) { /*two*/
//				pa[hi] = sgn(neg[hi],SQRTHALF);
//				pa[lo] = sgn(neg[lo], SQRTHALF);
//				if (lo>hi) {
//					hi ^= lo; lo ^= hi; hi ^= lo;
//				}
//				if (hi==W) {
//					hi = "\001\002\000"[lo];
//					lo = 3-hi-lo;
//				}
//				swap(ka,hi,lo);
//			}
//			else {/*big*/
//				pa[hi] = sgn(neg[hi],1.0);
//			}
//		}
//		p.x = -pa[0]; p.y = -pa[1]; p.z = -pa[2]; p.w = pa[3];
//	}
//	k->x = ka[X]; k->y = ka[Y]; k->z = ka[Z];
//	return (p);
//}
//
///******* Decompose Affine Matrix *******/
//
///* Decompose 4x4 affine matrix A as TFRUK(U transpose), where t contains the
// * translation components, q contains the rotation R, u contains U, k contains
// * scale factors, and f contains the sign of the determinant.
// * Assumes A transforms column vectors in right-handed coordinates.
// * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
// * Proceedings of Graphics Interface 1992.
// */
//
//static inline void decompAffine(_HMatrix A, _affineParts * parts)
//{
//	_HMatrix Q, S, U;
//	Q4t p;
//
//	//Translation component.
//	parts->t = Q4(A[X][W], A[Y][W], A[Z][W], 0);
//	double det = polarDecomp(A, Q, S);
//	if (det<0.0)
//	{
//		matrixCopy(Q, =, -Q, 3);
//		parts->f = -1;
//	}
//	else
//		parts->f = 1;
//
//	parts->q = Q4FromMatrix(Q);
//	parts->k = spectDecomp(S, U);
//	parts->u = Q4FromMatrix(U);
//	p = snuggle(parts->u, &parts->k);
//	parts->u = Q4Multiply(parts->u, p);
//}
//
//static inline void M16Decompose(M16t M16,V3t t,Q4t r,V3t s,Q4t so ){
//
//	_affineParts parts;
//    _HMatrix hmatrix;
//
//    // Transpose copy of LTW
//    for ( int i =0; i<4; i++)
//    {
//        for ( int j=0; j<4; j++)
//        {
//            hmatrix[i][j] = M16.m[j*4+i];
//        }
//    }
//
//    decompAffine(hmatrix, &parts);
//
//    double mul = 1.0;
//    if (parts.t.w != 0.0)
//        mul = 1.0 / parts.t.w;
//
//    t.x = parts.t.x * mul;
//    t.y = parts.t.y * mul;
//    t.z = parts.t.z * mul;
//
//    r = Q4(parts.q.x, parts.q.y, parts.q.z, parts.q.w);
//
//    mul = 1.0;
//    if (parts.k.w != 0.0)
//        mul = 1.0 / parts.k.w;
//
//    // mul be sign of determinant to support negative scales.
//    mul *= parts.f;
//    s.x= parts.k.x * mul;
//    s.y = parts.k.y * mul;
//    s.z = parts.k.z * mul;
//
//    so = Q4(parts.u.x, parts.u.y, parts.u.z, parts.u.w);
//}

#endif



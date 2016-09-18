#import <Foundation/Foundation.h>
#import "cocoa.h"

using namespace std;

string NSToCppString(NSString *s){
        return string([s cStringUsingEncoding:NSUTF8StringEncoding]);
}

NSString* cppToNSString(string str){
        return [NSString stringWithUTF8String:str.c_str()];
}

namespace Platform {
string appDirectory() {
#if TARGET_OSX
        return NSToCppString([[NSBundle mainBundle] bundlePath]) +"/Contents/Resources/";
#elif TARGET_IOS
        return NSToCppString([[NSBundle mainBundle] bundlePath]) + "/";
#endif

}

string pathForResource(string name, string ext) {
        return NSToCppString([[NSBundle mainBundle] pathForResource: cppToNSString(name) ofType: cppToNSString(ext)]);
}
}

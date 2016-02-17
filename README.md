# NodeKittenX

NodeKittenX is an experimental cross-platform openGL toolkit

It is designed to be light-weight and be embedded into a GL Context, rather than a windowing platform.

NodeKitten Views come in a couple flavors so far
1. Cocoa
2. SDL
3. Emscripten (SDL)

This library is under development, and is buggy as such, the cocoapod demo should run easily out of the box. Beyond that YMMV.

Roadmap is:
1. finish platform abstraction
2. document api
3. the fun stuff

# Use with CocoaPods

To use this project inside an Xcode project use CocoaPods

pod 'NodeKittenX', '~> 0.1.0'

There are many places to insert a view, including interface builder, here is a programmatic example.

put the following in your ViewController file
```
#import <NodeKittenX/NodeKitten.h>
```
make sure to rename any files that include c++ from .m to .mm
```
@implementation ViewController

- (void)loadView {
    [super loadView];
    
    NKNSView* view = [[NKNSView alloc]initWithFrame:self.view.frame];
    self.view = view;
    view.nkView->setScene(make_shared<ExampleScene>(V2t{(float)self.view.bounds.size.width,(float)self.view.bounds.size.height}));
}
```

for ios use NKUIView
for osx use NKNSView

# Use BIICODE blocks with SDL and BYO c++ IDE

To use this project with SDL on X86 or ARM Mac/Win/Linux use biicode and point to the NodeKittenX block

#build status
```
OSX - pass
Emscripten - running (see structuresound.com/nodeKitten) but not fully implemented
RaspberryPi - fail (some invalid gl commands need abstracted)
Linux64 - untested
Windows - untested
```

make a new biicode project
```
bii init prj_name
```
or use with CLION IDE
```
bii init -l clion prj_name
```
then add this as a main file
(the include statements will automatically tell the BIICODE dep manager where to find the NodeKittenX Block)
```
#include "leif/NodeKittenX/NodeKitten.h"
#include "leif/NodeKittenX/Platform/SDL/NKSDLView.h"

NKSDLView *sdlView;

int main(int, char**){

    sdlView = new NKSDLView(800,600);

    sdlView->setup();
    sdlView->setScene(make_shared<ExampleScene>(V2t{800,600}));

    return sdlView->loop();

}
```
then download the dependency with:
```
bii find
bii configure
bii build
```




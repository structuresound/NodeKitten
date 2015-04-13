# NodeKittenX

NodeKittenX is an experimental cross-platform openGL toolkit

It is designed to be light-weight and be embedded in native platform tools, rather than being its own platform.

Traditional platform abstraction is provided through SDL.

Modern platform abstraction is provided through native hooks ie on iOS use NKUIView

# Use with CocoaPods

To use this project inside an XCode project use CocoaPods

pod 'NodeKittenX' ~> '0.1.0'

There are many places to insert a view, including interface builder, here is a programmatic example.

for ios use NKUIView
for osx use NKNSView

```
@implementation ViewController

- (void)loadView {
    [super loadView];
    
    NKNSView* view = [[NKNSView alloc]initWithFrame:self.view.frame];
    self.view = view;
    view.nkView->setScene(make_shared<ExampleScene>(V2t{(float)self.view.bounds.size.width,(float)self.view.bounds.size.height}));
}
```

# Use BIICODE blocks with SDL and BYO c++ IDE

To use this project with SDL on X86 or ARM Mac/Win/Linux use biicode and point to the NodeKittenX block

make a new biicode project
```
bii init <your_project>
```
or use with CLION IDE
```
bii init -lClion <your_project>
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




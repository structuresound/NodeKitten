#include "examples.h"
#include <nodeKitten/platform/sdl/sdl.hpp>

NKSDLView *sdlView;

using namespace std;

int main(int, char**){

    sdlView = new NKSDLView(800,600);

    sdlView->setup();
    sdlView->setScene(make_shared<AnimationExample>(sdlView->size.get()));
    return sdlView->loop();
}
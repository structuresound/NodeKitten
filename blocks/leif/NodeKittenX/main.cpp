#include "NodeKitten.h"
#include "NKSDLView.h"
#include "Examples.h"


NKSDLView *sdlView;

int main(int, char**){

    sdlView = new NKSDLView(800,600);

    sdlView->setup();
    sdlView->setScene(make_shared<LayoutExample>(sdlView->size.get()));

    return sdlView->loop();
}
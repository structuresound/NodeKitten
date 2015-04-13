#include "ExampleScene.h"
#include "NKSDLView.h"

NKSDLView *sdlView;

int main(int, char**){

    sdlView = new NKSDLView(800,600);

    sdlView->setup();
    sdlView->setScene(make_shared<ExampleScene>(V2t{800,600}));

    return sdlView->loop();

}
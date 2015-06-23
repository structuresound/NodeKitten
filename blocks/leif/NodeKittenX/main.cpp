#include "ExampleScene.h"
#include "FreeTypeScene.h"
#include "ExampleMenu.h"
#include "NKSDLView.h"

NKSDLView *sdlView;

int main(int, char**){

    sdlView = new NKSDLView(800,600);

    sdlView->setup();
    sdlView->setScene(make_shared<ExampleScene>(sdlView->size()));

    return sdlView->loop();

}
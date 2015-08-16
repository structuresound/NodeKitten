//
// Created by Leif Shackelford on 4/11/15.
//

#ifndef NODEKITTENX_NKSDLVIEW_H
#define NODEKITTENX_NKSDLVIEW_H

#include "SceneController.h"
#include "miguel/sdl2/include/SDL.h"

class NKSDLView : public SceneController {

public:

    NKSDLView(F1t width = 800, F1t height = 600) : SceneController(V2t{width, height}) {
        sdl_width = width;
        sdl_height = height;
    }

    ~NKSDLView(){
        _running = false;
        cleanup();
    }

    bool setup();
    bool loop();


    int _cycle_ms = 10;
    double _time = 0;

private:
    int sdl_width;
    int sdl_height;

    SDL_Window* displayWindow = nullptr;
    SDL_Renderer* displayRenderer = nullptr;

    bool    _running = true;

    void onKeyboardDown(SDL_KeyboardEvent event);
    void onKeyboardUp(SDL_KeyboardEvent event);
    void onMouseMotion(SDL_MouseMotionEvent event);
    void onMouseButtonDown(SDL_MouseButtonEvent event);
    void onEvent(SDL_Event *event);

    void cleanup();

};
#endif //NODEKITTENX_NKSDLVIEW_H
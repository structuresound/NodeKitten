//
// Created by Leif Shackelford on 4/11/15.
//

#include "NKSDLView.h"

bool NKSDLView::setup(){

    //First we need to start up SDL, and make sure it went ok
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 0;
    }

    SDL_RendererInfo displayRendererInfo;
    SDL_CreateWindowAndRenderer(sdl_width, sdl_height, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
    SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
    /*TODO: Check that we have OpenGL */

    if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        /*TODO: Handle this. We have no render surface and not accelerated. */
    }

    nkLog("SDL_Init Correctly: %d x %d", sdl_width, sdl_height);

    return 1;
}

void NKSDLView::onKeyboardDown(SDL_KeyboardEvent event){

}
void NKSDLView::onKeyboardUp(SDL_KeyboardEvent event){
    _running = false;
}

void NKSDLView::onMouseMotion(SDL_MouseMotionEvent event){
}

void NKSDLView::onMouseButtonDown(SDL_MouseButtonEvent event){
}

void NKSDLView::onEvent(SDL_Event *event){
}

void NKSDLView::cleanup()
{
    //if(font)TTF_CloseFont(font);
    SDL_DestroyWindow(displayWindow);
    SDL_Quit();
};

bool NKSDLView::loop() {

    SDL_Event event;

    while(_running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)_running = false;
            else if(event.type == SDL_KEYDOWN)
                onKeyboardDown(event.key);
            else if(event.type == SDL_KEYUP)onKeyboardUp(event.key);
            else if(event.type == SDL_MOUSEMOTION){
                SDL_MouseMotionEvent mmevent=event.motion;
                mmevent.y=sdl_height-mmevent.y;
                mmevent.yrel=-mmevent.yrel;
                onMouseMotion(mmevent);
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN){
                SDL_MouseButtonEvent mbevent=event.button;
                mbevent.y=sdl_height-mbevent.y;
                onMouseButtonDown(mbevent);
            }


            if(event.type == SDL_WINDOWEVENT){
                switch(event.window.event){
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        sdl_width = event.window.data1;
                        sdl_height = event.window.data2;

                        glClearColor(0.f, 0.f, 0.f, 1.f);
                        glViewport(0, 0, sdl_width, sdl_height);
                        glClear(GL_COLOR_BUFFER_BIT);

                        //onResized(width, height);
                        break;

                    default:
                        onEvent(&event);

                }
            }
            else
                onEvent(&event);
        }

        // DRAW CYCLE
        SDL_RenderClear(displayRenderer);
        NKView::draw();
        SDL_RenderPresent(displayRenderer);
        //

        SDL_Delay(_cycle_ms);
        _time+=_cycle_ms*0.001;
        //test
        //_view_y+=1.1;
    }

    cleanup();

    return 0;
}
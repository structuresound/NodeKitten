#include "../../macro.h"
#include "../../gl/frameBuffer.h"
#include "../common/sceneController.h"
#include <sdl/SDL.h>

static char *data_path = NULL;
static float renderScaleX = 1.0;
static float renderScaleY = 1.0;
static SDL_Window* displayWindow = nullptr;
static SDL_Renderer* displayRenderer = nullptr;

void InitializeDataPath() {
    char *base_path = SDL_GetBasePath();
    if (base_path) {
        data_path = base_path;
    } else {
        data_path = SDL_strdup("./");
    }
}

namespace Platform {
  namespace Screen {
    F1t scale(){
      return 1.0;
    }
  }

  std::string pathForResource(std::string name, std::string ext){
    return std::string(data_path) + '/' + name + '.' + ext;
  }

  std::string appDirectory(){
    return std::string(data_path);
  }
}

class NKSDLView : public SceneController {

public:
    NKSDLView(F1t width = 800, F1t height = 600) : SceneController(V2t{width, height}) {
        sdl_width = width;
        sdl_height = height;
    }

    ~NKSDLView() {
        _running = false;
        cleanup();
    }

    bool setup(){

        InitializeDataPath();

        //First we need to start up SDL, and make sure it went ok
        if (SDL_Init(SDL_INIT_VIDEO) != 0){
            std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return 0;
        }

        //to do: SDL_WINDOW_ALLOW_HIGHDPI
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

    bool loop() {

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
            SceneController::draw();
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


    int _cycle_ms = 10;
    double _time = 0;

private:
    int sdl_width;
    int sdl_height;

    bool _running = true;

    virtual void onKeyboardDown(SDL_KeyboardEvent event){};
    virtual void onKeyboardUp(SDL_KeyboardEvent event){};
    virtual void onMouseMotion(SDL_MouseMotionEvent event){};
    virtual void onMouseButtonDown(SDL_MouseButtonEvent event){};
    virtual void onEvent(SDL_Event *event){};

    virtual void cleanup()
    {
        //if(font)TTF_CloseFont(font);
        SDL_DestroyWindow(displayWindow);
        SDL_Quit();
    };

};

Simple DirectMedia Layer
=========================

SDL is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. It is used by video playback software, emulators, and popular games including Valve's award winning catalog and many Humble Bundle games.

NOTE: SDL2 relies on local audio libs to uniformize audio. On linux, probably you'll need to install libasound2 or pulse:
	apt-get install libasound2-dev libpulse-dev

Supported OS 
-------------
SDL officially supports Windows, Mac OS X, Linux, iOS, and Android. Support for other platforms may be found in the source code.

Language
----------
SDL is written in C, works natively with C++, and there are bindings available for several other languages, including C# and Python.

License
--------
SDL 2.0 is distributed under the zlib license. This license allows you to use SDL freely in any software.

Official site
-------------
https://www.libsdl.org

BIICODE ADAPTATION
--------
The library was downloaded from: 
  github.com/mhernando/sdl2 which is a fork of davidsiaw/SDL2 (mirror from mercurial official sdl2)
  The code was minimally adapted. Check biicode_adaptation.md too view the changes
  
  Building an apk only requires the call of two macros:
  
  - SET_SDL_ANDROID_PROYECT(target package)
  
  - SET_SDL_DATA(target data/*.* data) 
  that are defined into the cmake folder (sdlhelper.cmake)

Examples
----------
Take a look at some [simple examples] adapted from the original official site (just changing includes!)
(http://www.biicode.com/miguel/test_sdl2/) using SDL 2.0
(http://www.biicode.com/examples/game_utils) using SDL 2.0 and more
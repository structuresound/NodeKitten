#include "platform.h"

//**************
// SECONDARY CONDITIONALS
//**************
#define NK_EMULATE_VAO !NK_SUPPORTS_VAO
#define NK_SUPPORTS_INSTANCED_DRAWING NK_SUPPORTS_VAO


//*******************************************************************************************
// USER MACROS
//*******************************************************************************************
#define NK_SUPPRESS_LOGS 0
//** FOR GL RELATED LOGS
#define NK_LOG_GL 0
#define NK_LOG_CV 0
//**
//** LOG / PRINT TIME METRICS
#define NK_LOG_METRICS 1
//**

// SCENE DEBUG
#define DRAW_HIT_BUFFER 0

// SPRITES
#if NK_USE_GLES
#define NK_BATCH_SIZE 16
#else
#define NK_BATCH_SIZE 32
#endif

#define NK_BUILD_SDL 0
#define NK_BUILD_EMSCRIPTEN 0

// MODULES
#define NK_USE_OVR 0
#define NK_USE_ASSIMP 0
#define NK_USE_MIDI 0
#define NK_LOG_MIDI 0
#define NK_USE_KINECT 0

// SYSTEM MACROS
#ifdef DEBUG
#define _DEBUG
#endif

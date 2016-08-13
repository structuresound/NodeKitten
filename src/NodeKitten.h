/***********************************************************************

 Written by Leif Shackelford
 Copyright (c) 2014 Chroma.io
 All rights reserved. *
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met: *
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of CHROMA GAMES nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission. *
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE. *
 ***********************************************************************/

#pragma once

#include "commonTypes.h"
#include "commonUtils.h"

// REAL-TIME CLASSES

#include "action.h"
#include "uxEvent.h"

// ABSTRACT / BASE
#include "node.h"

// SCENE
#include "sceneController.h"
#include "scene.h"
#include "camera.h"
#include "light.h"

// 2D NODES = VIEW + SUBCLASSES
#include "view.h"
#include "collectionView.h"
#include "label.h"

// 3D NODES / MESH + SUBCLASSES
#include "mesh.h"
#include "meshBatcher.h"
#include "spriteEmitter.h"


//// TEXTURE
#include "frameBuffer.h"
#include "fontAtlas.h"
#include "texture.h"
#include "frameBuffer.h"

//// SHADERS
#include "shaderProgram.h"
#include "shaderModule.h"
#include "shaderTools.h"

// OTHERS VIEWS SHOULD BE INCLUDED IN PLATFORM SPECIFIC EXAMPLES
#if (TARGET_IOS || TARGET_OSX)
#include "cocoaDeviceTools.h"
#endif

#ifdef __OBJC__
#include "cocoaViews.h"
#include "cocoaUXEvent.h"
#endif

using namespace std;
using namespace Shader;

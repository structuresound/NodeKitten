#pragma once

#include "type.h"
#include "util/util.h"
#include "platform/platform.h"
#include "gl/gl.h"

// SCENE
#include "animation/action.h"
#include "node/node.h"
#include "view/scene.h"
#include "node/camera.h"
#include "node/light.h"

// 2D NODES = VIEW + SUBCLASSES
#include "view/view.h"
#include "view/collectionView.h"
#include "view/label.h"

// 3D NODES / MESH + SUBCLASSES
#include "node/mesh.h"
#include "node/meshBatcher.h"
#include "node/spriteEmitter.h"
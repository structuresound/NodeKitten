#pragma once

#include "../type.h"
#include "../generic/generic.h"
#include "../vector/vector.h"

#include "uxEvent.h"
#include "sceneController.h"

namespace Platform {
  namespace Screen {
    F1t scale();
  }

  std::string pathForResource(std::string name, std::string ext);
  std::string appDirectory();
}

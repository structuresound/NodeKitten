//
//  Platform.h
//  Pods
//
//  Created by Leif Shackelford on 5/25/15.
//
//

#pragma once

#include "commonTypes.h"

namespace Platform {
  namespace Screen {
    F1t scale();
  }

  std::string pathForResource(std::string name, std::string ext);
  std::string appDirectory();
}

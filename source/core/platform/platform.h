//
//  Platform.h
//  Pods
//
//  Created by Leif Shackelford on 5/25/15.
//
//

#pragma once

#include "../type.h"
#include "../generic/generic.h"
#include "../vector/vector.h"

#include "common/uxEvent.h"
#include "common/sceneController.h"

// OTHERS VIEWS SHOULD BE INCLUDED IN PLATFORM SPECIFIC EXAMPLES
#if (TARGET_IOS || TARGET_OSX)
#include "cocoa/cocoaDeviceTools.h"
#endif
#ifdef __OBJC__
#include "cocoa/cocoaViews.h"
#include "cocoa/cocoaUXEvent.h"
#endif

namespace Platform {
  namespace Screen {
    F1t scale();
  }

  std::string pathForResource(std::string name, std::string ext);
  std::string appDirectory();
}

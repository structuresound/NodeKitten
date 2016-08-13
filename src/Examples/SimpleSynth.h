//
//  LayoutExample.h
//  Pods
//
//  Created by Leif Shackelford on 6/19/15.
//
//

#ifndef __LayoutExample__
#define __LayoutExample__

#include "NodeKitten.h"

class SimpleSynth : public Scene {

public:
  SimpleSynth(S2t size) : Scene(size){};

  void moveToView() override;
  void handleUXEvent(shared_ptr<UXEvent> event) override;

};

#endif /* defined(__LayoutExample__) */

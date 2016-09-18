//
//  Text3DExample.h
//  Pods
//
//  Created by Leif Shackelford on 5/2/15.
//
//

#ifndef __Text3DExample__
#define __Text3DExample__

#include <nodeKitten/core.h>

class Text3DExample : public Scene {
    
public:
    Text3DExample(S2t size) : Scene(size){};
    
    void moveToView() override;
};


#endif /* defined(__Pods__Text3DExample__) */

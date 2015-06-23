//
//  Platform.h
//  Pods
//
//  Created by Leif Shackelford on 5/25/15.
//
//

#ifndef __Platform__
#define __Platform__

// FUNCTIONS TO BE DEFINED WITHIN PLATFORM SPECIFIC FILES

namespace Platform {
    namespace Screen {
        F1t scale();
    }
    
    string pathForResource(string name, string ext);
    string appDirectory();
}

#endif /* defined(__Pods__Platform__) */

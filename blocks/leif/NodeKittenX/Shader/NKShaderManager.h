//
//  NKShaderManager.h
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#ifndef __NKShaderManager__
#define __NKShaderManager__

#include "NKShaderTools.h"

class NKShaderProgram;
class NKNode;

class NKShaderManager {
    
public:
    static NKShaderManager* sharedInstance();
    
    static void newUIDColorForNode(NKNode* node);
    static NKNode* nodeForColor(shared_ptr<NKByteColor> color);
    
    static map<string, shared_ptr<NKShaderProgram>> programCache;
    static map<shared_ptr<NKByteColor>, NKNode*> nodeMap;
    
private:
    
    NKShaderManager();
    static NKShaderManager* pSingleton;		// singleton instance
};

#endif

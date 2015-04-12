//
//  NKShaderManager.m
//  EMA Stage
//
//  Created by Leif Shackelford on 5/24/14.
//  Copyright (c) 2014 EMA. All rights reserved.
//

#include "NKShaderManager.h"
#include "NKNode.h"

map<string, shared_ptr<NKShaderProgram>> NKShaderManager::programCache;
map<shared_ptr<NKByteColor>, NKNode*> NKShaderManager::nodeMap;

NKShaderManager* NKShaderManager::pSingleton = nullptr;

NKShaderManager::NKShaderManager()
{
    // do init stuff
}

NKShaderManager* NKShaderManager::sharedInstance()
{
    if (pSingleton== NULL) {
        pSingleton = new NKShaderManager();
    }
    return pSingleton;
}

NKNode* NKShaderManager::nodeForColor(shared_ptr<NKByteColor> color){
    return nodeMap[color];
}

void NKShaderManager::newUIDColorForNode(NKNode *node){
    
    shared_ptr<NKByteColor> color;
    
    for (int i = 0; i < INT_MAX; i++) {
        shared_ptr<NKByteColor> test = make_shared<NKByteColor>(rand() % 255,rand() % 255,rand() % 255,255);
        if (!nodeMap[test]) {
            node->uidColor = test;
            nodeMap[test] = node;
            return;
        }
    }
    
    nkLog("failed to get UID color");
}


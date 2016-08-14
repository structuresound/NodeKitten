//
//  NKMatrixStack.m
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 6/20/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//

#include "stack.h"

int M16Stack::locationByDepth(M16t matrix) {
    for (int i = 0;  i < _count; i++){
        if (matrix.m32 < (_data+_count)->m32) {
            return i;
        }
    }
    return _count;
}

void M16Stack::pushMultiply(M16t matrix){
    if (_count > 0) {
        _data[_count] = M16Multiply(_data[_count-1], matrix);
    }
    else {
        _data[_count] = matrix;
    }

    push();
}

void M16Stack::pushScale(V3t scale) {
    if (_count > 0) {
        _data[_count] = M16ScaleWithV3(_data[_count-1], scale);
    }
    push();
}

//
//  NKMatrixStack.h
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 6/20/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//



#ifndef __ScalarStack__
#define __ScalarStack__

#include "VectorTypes.h"
#include <initializer_list>

template <typename T> class ScalarStack

{
protected:
    unsigned long _blockSize {0};
    unsigned long _count {0};
    U1t _stride {0};
    T* _data {0};
    
public:
    
    ScalarStack() {
        _stride = sizeof(T);
        _data = (T*)malloc(_stride * NK_BATCH_SIZE);
        _blockSize = NK_BATCH_SIZE;
        _count = 0;
    }
    
    ScalarStack(std::initializer_list<T> elements) {
        _stride = sizeof(T);
        _data = (T*)malloc(_stride * elements.size());
        _blockSize = elements.size();
        _count = elements.size();
        
        std::copy(elements.begin(), elements.end(), _data);
    }
    
    ScalarStack(const T* data, unsigned long size) {
        _stride = sizeof(T);
        _data = (T*)malloc(_stride * size);
        memcpy(_data, data, _stride * size);
        _blockSize = size;
        _count = size;
    }
    
    ScalarStack(T** data, unsigned long size) {
        _stride = sizeof(T);
        steal(data, size);
    }
    
    ScalarStack(const ScalarStack& stack) : ScalarStack(stack.data(), stack.size()){
    }
    
    ScalarStack(ScalarStack& stack){
        _stride = sizeof(T);
        steal(stack.data(), stack.size());
    }
    
    ScalarStack(ScalarStack&& stack){
        _stride = sizeof(T);
        steal(stack.data(), stack.size());
    }
    
    ScalarStack& operator=(const ScalarStack& stack) {
        if (_data) free(_data);
        _stride = sizeof(T);
        _blockSize =  stack.size();
        _count = _blockSize;
        _data = (T*)malloc(_stride * _count);
        memcpy(_data, stack.data(), _stride * _count);
        return *this;
    }
    
    ScalarStack& operator=(ScalarStack& stack){
        _stride = sizeof(T);
        steal(stack.data(), stack.size());
    }
    
    ScalarStack& operator=(ScalarStack&& stack){
        _stride = sizeof(T);
        steal(stack.dataRef(), stack.size());
        return *this;
    }
    
    
    void steal(T** data, unsigned long size){
        if (_data) {
            free(_data);
        }
        nkLog("'steal' %lu elements \n", size);
        _data = *data;
        _blockSize = size;
        _count = size;
        *data = nullptr;
    }
    
    
    T& operator[](int n) const
    {
        return _data[n];
    }
    
    ~ScalarStack() {
        if (_data) {
            free(_data);
        }
    }
    
    T* data() const {
        return _data;
    }
    
    T** dataRef() {
        return &_data;
    }
    
    unsigned long size() const{
        return _count;
    }
    
    unsigned long length() const{
        return _stride * _count;
    }
    
    T lastObject() {
        return _data[_count-1];
    }
    
    void push() {
        _count++;
        if (_blockSize <= _count) {
            //nkLog("Expanding MATRIX STACK allocation size \n");
            
            void* copyBlock = malloc(_stride * (_count*2));
            memcpy(copyBlock, _data, _stride * _count);
            free(_data);
            _data = (T*)copyBlock;
            
            _blockSize = _count * 2;
        }
        
    }
    
    void pop() {
        if (_count > 0) {
            _count--;
        }
        else {
            nkLog("STACK UNDERFLOW \n");
        }
    }
    
    void clear() {
        _count = 0;
    }
    
    void append(T& item) {
        _data[_count] = item;
        push();
    }
    
    void append(T&& item) {
        _data[_count] = item;
        push();
    }
    
    void insert(T item, U1t index) {
        T* copyBlock = malloc(_stride * (_blockSize));
        memcpy(copyBlock, _data, _stride*index);
        memcpy(copyBlock+index, item, _stride);
        memcpy(copyBlock+index+1, _data+index, _stride * (_count - index));
        free(_data);
        _data = copyBlock;
        push();
    }
    
    void print(){
        F1t *ptr = (F1t*)_data;
        int inc = _stride / sizeof(F1t);
        
        for (int i = 0; i < _count; i++){
            for (int e = 0; e < inc; e++){
                nkLog("%1.2f ", ptr[i*inc + e]);
            }
            nkLog("\n");
        }
        
    }
    
};

class M16Stack : public ScalarStack<M16t>{
    using ScalarStack<M16t>::ScalarStack;
public:
    int locationByDepth(M16t matrix);
    void pushMultiply(M16t matrix);
    void pushScale(V3t scale);
};

class M9Stack : public ScalarStack<M9t> {using ScalarStack<M9t>::ScalarStack;};
class V4Stack : public ScalarStack<V4t> {using ScalarStack<V4t>::ScalarStack;};
class V3Stack : public ScalarStack<V3t> {using ScalarStack<V3t>::ScalarStack;};
class V2Stack : public ScalarStack<V2t> {using ScalarStack<V2t>::ScalarStack;};
class F1Stack : public ScalarStack<F1t> {using ScalarStack<F1t>::ScalarStack;};
class U1Stack : public ScalarStack<U1t> {using ScalarStack<U1t>::ScalarStack;};

#endif

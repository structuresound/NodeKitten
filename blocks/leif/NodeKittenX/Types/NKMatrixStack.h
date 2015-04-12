//
//  NKMatrixStack.h
//  NodeKittenIOS
//
//  Created by Leif Shackelford on 6/20/14.
//  Copyright (c) 2014 chroma. All rights reserved.
//



#ifndef __NKStack__
#define __NKStack__

#include "NKMacro.h"

#include "NKVectorTypes.h"
#include <initializer_list>

template <typename T> class NKStack

{
protected:
    unsigned long _blockSize {0};
    unsigned long _count {0};
    U1t _stride {0};
    T* _data {0};
    
public:
    
    NKStack() {
        _stride = sizeof(T);
        _data = (T*)malloc(_stride * NK_BATCH_SIZE);
        _blockSize = NK_BATCH_SIZE;
        _count = 0;
    }
    
    NKStack(std::initializer_list<T> elements) {
        _stride = sizeof(T);
        _data = (T*)malloc(_stride * elements.size());
        _blockSize = elements.size();
        _count = elements.size();
        
        std::copy(elements.begin(), elements.end(), _data);
    }
    
    NKStack(const T* data, unsigned long size) {
        _stride = sizeof(T);
        _data = (T*)malloc(_stride * size);
        memcpy(_data, data, _stride * size);
        _blockSize = size;
        _count = size;
    }
    
    NKStack(T** data, unsigned long size) {
        _stride = sizeof(T);
        steal(data, size);
    }
    
    NKStack(const NKStack& stack) : NKStack(stack.data(), stack.size()){
    }
    
    NKStack(NKStack& stack){
        _stride = sizeof(T);
        steal(stack.data(), stack.size());
    }
    
    NKStack(NKStack&& stack){
        _stride = sizeof(T);
        steal(stack.data(), stack.size());
    }
    
    NKStack& operator=(const NKStack& stack) {
        if (_data) free(_data);
        _stride = sizeof(T);
        _blockSize =  stack.size();
        _count = _blockSize;
        _data = (T*)malloc(_stride * _count);
        memcpy(_data, stack.data(), _stride * _count);
        return *this;
    }
    
    NKStack& operator=(NKStack& stack){
        _stride = sizeof(T);
        steal(stack.data(), stack.size());
    }
    
    NKStack& operator=(NKStack&& stack){
        _stride = sizeof(T);
        steal(stack.dataRef(), stack.size());
        return *this;
    }
    
    
    void steal(T** data, unsigned long size){
        if (_data) {
            free(_data);
        }
        printf("'steal' %lu elements \n", size);
        _data = *data;
        _blockSize = size;
        _count = size;
        *data = nullptr;
    }
    
    
    T& operator[](int n) const
    {
        return _data[n];
    }
    
    ~NKStack() {
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
            printf("Expanding MATRIX STACK allocation size \n");
            
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
            printf("STACK UNDERFLOW \n");
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
                printf("%1.2f ", ptr[i*inc + e]);
            }
            printf("\n");
        }
        
    }
    
};

class M16Stack : public NKStack<M16t>{
    using NKStack<M16t>::NKStack;
public:
    int locationByDepth(M16t matrix);
    void pushMultiply(M16t matrix);
    void pushScale(V3t scale);
};

class M9Stack : public NKStack<M9t> {using NKStack<M9t>::NKStack;};
class V4Stack : public NKStack<V4t> {using NKStack<V4t>::NKStack;};
class V3Stack : public NKStack<V3t> {using NKStack<V3t>::NKStack;};
class V2Stack : public NKStack<V2t> {using NKStack<V2t>::NKStack;};
class F1Stack : public NKStack<F1t> {using NKStack<F1t>::NKStack;};
class U1Stack : public NKStack<U1t> {using NKStack<U1t>::NKStack;};

#endif

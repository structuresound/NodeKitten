#pragma once

#include "underscore.hpp"
#include "string.hpp"
#include "file.hpp"
#include "generator.hpp"

#include "underscore.h"

namespace underscore {
  
  template<class Container, class Value>
  void append_unique( Container& c, const Value& v ) {
    if( find(begin(c), end(c), v) == end(c) )
      c.emplace_back(v);
    assert( !c.empty() );
  }
  
  template <typename T>
  void append(std::vector<T>& a, const std::vector<T>& b)
  {
    a.insert(a.end(), b.begin(), b.end());
  }
  
  // substracts b<T> to a<T>
  template <typename T>
  void
  subtract(std::vector<T>& a, const std::vector<T>& b)
  {
    typename std::vector<T>::iterator       it = a.begin();
    typename std::vector<T>::const_iterator it2 = b.begin();
    typename std::vector<T>::iterator       end = a.end();
    typename std::vector<T>::const_iterator end2 = b.end();
    
    while (it != end)
    {
      while (it2 != end2)
      {
        if (*it == *it2)
        {
          it = a.erase(it);
          end = a.end();
          it2 = b.begin();
        }
        else
          ++it2;
      }
      ++it;
      it2 = b.begin();
    }
  }
  

  template <typename C, typename T>
  void erase(C& vec, T a) {
    for (auto it = vec.begin(); it != vec.end(); ++it){
      if (*it == a){
        vec.erase(it);
        return;
      }
    }
  }
  
  template <typename C, typename T>
  void eraseUniquePtr(C& vec, T& a) {
    for (auto it = vec.begin(); it != vec.end(); ++it){
      if (*it == a){
        vec.erase(it);
        return;
      }
    }
  }
  
  template <typename C, typename T>
  void eraseSmartFromRaw(C& vec, T a) {
    for (auto it = vec.begin(); it != vec.end(); ++it){
      if ((*it).get() == a){
        vec.erase(it);
        return;
      }
    }
  }
  
  template <typename R, typename C>
  R rawCopy(R& raw, C& container) {
    for (auto& e : container){
      raw.push_back(e.get());
    }
    return raw;
  }
  
  template <typename C, typename L>
  void eachMutable(C& container, L func) {
    std::vector<typename C::value_type::element_type *> copy;
    for (auto& e : container){
      copy.push_back(e.get());
    }
    for (auto p : copy){
      func(p);
    }
  }
  
  template <typename T> void _printVector(std::vector<T>& vec) {
    
    int inc = sizeof(T) / sizeof(float);
    for (int i = 0; i < vec.size(); i++){
      for (int e = 0; e < inc; e++){
        printf("%1.2f ", (double)vec[i*inc + e]);
      }
      printf("\n");
    }
  }
  
} //namespace

namespace _ = underscore;

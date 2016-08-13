//
//  NKTemplateFunctions.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/25/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#pragma once

#include "_.hpp"

namespace _h {

    // This is the Hash128to64 function from Google's cityhash (available
    // under the MIT License).  We use it to reduce multiple 64 bit hashes
    // into a single hash.
    inline uint64_t hash_128_to_64(const uint64_t upper, const uint64_t lower) {
      // Murmur-inspired hashing.
      const uint64_t kMul = 0x9ddfea08eb382d69ULL;
      uint64_t a = (lower ^ upper) * kMul;
      a ^= (a >> 47);
      uint64_t b = (upper ^ a) * kMul;
      b ^= (b >> 47);
      b *= kMul;
      return b;
    }

    template <typename T, typename... Ts>
    size_t hash_combine(const T& t, const Ts&... ts) {
      size_t seed = std::hash<T>()(t);
      if (sizeof...(ts) == 0) {
        return seed;
      }
      size_t remainder = hash_combine(ts...);   // not recursion!
      return hash_128_to_64(seed, remainder);

    }

} //namespace

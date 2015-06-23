//
//  NKTemplateFunctions.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/25/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef NodeKittenX_NKTemplateFunctions_h
#define NodeKittenX_NKTemplateFunctions_h

#pragma mark - CONTAINERS

namespace _c {
    
    template<class Container, class Value>
    void append_unique( Container& c, const Value& v ) {
        if( find(begin(c), end(c), v) == end(c) )
            c.emplace_back(v);
        assert( !c.empty() );
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
    
//template <typename C, typename T>
//void erase_object(C& vec, T a) {
//    auto compare = [a](T b){return a == b;};
//    remove_if(vec.begin(), vec.end(), compare);
//}

template <typename C, typename T>
void erase(C& vec, T a) {
    for (auto it = vec.begin(); it != vec.end(); ++it){
        if (*it == a){
            //printf("erase object \n");
            vec.erase(it);
            return;
        }
    }
//    auto compare = [a](T b){return a == b;};
//    remove_if(vec.begin(), vec.end(), compare);
}

template <typename C, typename T>
bool contains(C& vec, T a) {
    for (auto it = vec.begin(); it != vec.end(); ++it){
        if (*it == a){
            return true;
        }
    }
    return false;
}

template <typename T> void _printVector(vector<T>& vec) {
    
    int inc = sizeof(T) / sizeof(F1t);
    for (int i = 0; i < vec.size(); i++){
        for (int e = 0; e < inc; e++){
            //nkLog("{0}", vec[i*inc + e]);
            printf("%1.2f ", (double)vec[i*inc + e]);
        }
        printf("\n");
    }
}

//template <typename C, typename T>
//void erase_object(C& vec, T obj) {
//    vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
//}

} //namespace

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


#endif

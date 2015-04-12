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

template<class Container, class Value>
void append_unique( Container& c, const Value& v ) {
    if( find(begin(c), end(c), v) == end(c) )
        c.emplace_back(v);
    assert( !c.empty() );
}

// substracts b<T> to a<T>
template <typename T>
void
substract_vector(std::vector<T>& a, const std::vector<T>& b)
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
void erase_object(C& vec, T a) {
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
bool containsObject(C& vec, T a) {
    for (auto it = vec.begin(); it != vec.end(); ++it){
        if (*it == a){
            return true;
        }
    }
    return false;
}

//template <typename C, typename T>
//void erase_object(C& vec, T obj) {
//    vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
//}


#endif

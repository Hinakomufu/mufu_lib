#pragma once
//https://github.com/Hinakomufu/mufu_lib/blob/main/lib/number_type/matrix_semiring.hpp
#include <bits/stdc++.h>
#include "../number_type/matrix.hpp"
using namespace std;

template<class T>
struct min_plus{
    static T add(T const a,T const b){ return min(a,b); }
    static T zero(){ return numeric_limits<T>::max()-1; }
    static T mul(T const a,T const b){
        if(is_inf(a)||is_inf(b)) return zero();
        return a+b;
    }
    static T one(){ return 0; }
    static bool is_inf(T const x){ return x>=numeric_limits<T>::max()-1; }
};

template<class T>
struct max_plus{
    static T add(T const a,T const b){ return max(a,b); }
    static T zero(){ return numeric_limits<T>::lowest()+1; }
    static T mul(T const a,T const b){
        if(is_inf(a)||is_inf(b)) return zero();
        return a+b;
    }
    static T one(){ return 0; }
    static bool is_inf(T const x){ return x<=numeric_limits<T>::lowest()+1; }
};

struct or_and{
    static bool add(bool const a,bool const b){ return a||b; }
    static bool zero(){ return false; }
    static bool mul(bool const a,bool const b){ return a&&b; }
    static bool one(){ return true; }
    static bool is_inf(bool const x){ return x==zero(); }
};

struct xor_and{
    using T = unsigned int;
    static T add(T const a,T const b){ return a^b; }
    static T zero(){ return 0; }
    static T mul(T const a,T const b){ return a&b; }
    static T one(){ return ~0; }
    static bool is_inf(T const x){ return x==zero(); }
};



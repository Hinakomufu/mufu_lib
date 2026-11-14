#pragma once
#include <bits/stdc++.h>
using namespace std;

template<typename U>
struct Max{
    using T=U;
    static T f(const T&a,const T&b){ return max(a,b); }
    static T e(){ return numeric_limits<T>::lowest()+1; }
};

template<typename U>
struct Min{
    using T=U;
    static T f(const T&a,const T&b){ return min(a,b); }
    static T e(){ return numeric_limits<T>::max()-1; }
};



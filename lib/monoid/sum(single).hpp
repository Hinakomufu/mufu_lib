#pragma once
//https://github.com/Hinakomufu/mufu_lib/blob/main/lib/monoid/sum(single).hpp
#include <bits/stdc++.h>
using namespace std;

template<typename U>
struct Sum{
    using T=U;
    static T f(const T&a,const T&b){ return a+b; }
    static T e(){ return T(0); }
};



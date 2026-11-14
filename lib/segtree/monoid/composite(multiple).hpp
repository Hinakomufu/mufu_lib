#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class Mint>
struct Type_comp{
    Mint a;
    Mint b;

    Type_comp(Mint const a=0,Mint const b=0) : a(a), b(b){}
    static constexpr Type_comp e(){ return {1,0}; }
};

template<class Mint>
struct Composite{
    using T=::Type_comp<Mint>;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){return{ta.a*tb.a,tb.a*ta.b+tb.b};}
    static T e(){ return T::e(); }
};



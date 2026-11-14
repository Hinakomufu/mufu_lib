#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class Mint>
struct Type_comp{
    Mint a;
    Mint b;
    long len;

    Type_comp(Mint const a_=0,Mint const b_=0,long const len_=1) : a(a_), b(b_), len(len_){}
    static constexpr Type_comp e(){ return {1,0,0}; }
};

template<class Mint>
struct Func_comp{
    Mint c;
    Mint d;

    static constexpr Func_comp id(){ return {1,0}; }

    bool operator==(Func_comp const&other) const{
        return c==other.c&&d==other.d;
    }

    bool operator!=(Func_comp const&other) const{ return !(*this==other); }
};

template<class Mint>
struct Composite{
    using T=::Type_comp<Mint>;
    using F=::Func_comp<Mint>;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){ return {ta.a*tb.a,tb.a*ta.b+tb.b,ta.len+tb.len}; }
    static T e(){ return T::e(); }

    // apply operation f to segments, f(t)
    static T ap(const F&ff,const T&tt){
        Mint a=1;
        Mint b=0;
        Mint c=ff.c;
        Mint d=ff.d;
        long k=tt.len;
        while(k){
            if(k&1){
                a=c*a;
                b=c*b+d;
            }
            d=c*d+d;
            c=c*c;
            k>>=1;
        }
        return {a,b,tt.len};
    }

    // y=f(x) -> z=g(y), z=g(f(x))
    // left first, right next
    static F cmp(const F&ff,const F&fg){
        return fg;
    }

    static F id(){ return F::id(); }
};



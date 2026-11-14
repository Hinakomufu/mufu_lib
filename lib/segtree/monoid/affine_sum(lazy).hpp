#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class Mint>
struct Type_affine{
    Mint val;
    long len;

    Type_affine(long const val_=0,long const len_=0): val(val_),len(len_) {}
    static constexpr Type_affine e(){return {0,0};}
};

template<class Mint>
struct Func_affine{
    Mint mul;
    Mint add;

    static constexpr Func_affine id(){return {1,0};}

    bool operator==(Func_affine const&other) const{
        return mul==other.mul&&add==other.add;
    }

    bool operator!=(Func_affine const&other) const{return !(*this==other);}
};

template<class Mint>
struct Sum{
    using T=::Type_affine<Mint>;
    using F=::Func_affine<Mint>;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){ return {ta.val+tb.val,ta.len+tb.len}; }
    static T e(){ return T::e(); }

    // apply operation f to segments, f(t)
    static T ap(const F&ff,const T&tt){
        return {ff.mul*tt.val+ff.add*tt.len,tt.len};
    }

    // y=f(x) -> z=g(y), z=g(f(x))
    // left first, right next
    static F cmp(const F&ff,const F&fg){
        return {
        fg.mul*ff.mul,
        fg.mul*ff.add+fg.add
        };
    }

    static F id(){ return F::id(); }
};



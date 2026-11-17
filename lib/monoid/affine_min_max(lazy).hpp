#pragma once
//https://github.com/Hinakomufu/mufu_lib/blob/main/lib/monoid/affine_min_max(lazy).hpp
#include <bits/stdc++.h>
using namespace std;

// mul>=0 only

template<typename U>
struct Type{
    U val;
    Type(U const val_=U{}): val(val_){}
};

template<typename U>
struct Func{
    U mul; // mul>=0 only
    U add;

    static constexpr Func id(){return {1,0};}

    bool operator==(Func const&other) const{
        return mul==other.mul&&add==other.add;
    }

    bool operator!=(Func const&other) const{
        return !(*this==other);
    }
};


template<typename U>
struct Max{
    using T=::Type<U>;
    using F=::Func<U>;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){ return {max(ta.val,tb.val)}; }
    static T e(){ return numeric_limits<U>::lowest(); }

    // apply operation f to segments, f(t)
    static T ap(const F&ff,const T&tt){
        return {ff.mul*tt.val+ff.add};
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

template<typename U>
struct Min{
    using T=::Type<U>;
    using F=::Func<U>;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){ return {min(ta.val,tb.val)}; }
    static T e(){ return numeric_limits<U>::max(); }

    // apply operation f to segments, f(t)
    static T ap(const F&ff,const T&tt){
        return {ff.mul*tt.val+ff.add};
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



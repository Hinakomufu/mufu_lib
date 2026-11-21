#pragma once
#include <bits/stdc++.h>
#include "../floor_sum/floor_sum.hpp"
#include "../modpow.hpp"
using namespace std;

template<class T>
struct sequence{
    T first; // s(0)
    T diff;  // s(1)-s(0)
    T size;

    sequence(T first,T diff,T size): first(first), diff(diff), size(size){
        if(this->size>T(0)&&this->diff<T(0)){
            this->first+=this->diff*(this->size-1);
            this->diff=-this->diff;
        }
    }

    T operator()(T const&i) const{ return first+diff*i; }
    T back() const{ return first+diff*(size-1); }

    T sum() const{ // sum of s(i) [0,size-1]
        T n=size;
        T a=first+back();
        if(n%T(2)==T(0)) n/=T(2);
        else a/=T(2);
        return n*a;
    }

    T pow_sum(T x,T mod) const{ // mod of (sum of x^s(i) [0,size-1])
        return pow_sum(x,mod,modpow(x,diff,mod));
    }

    T pow_sum(T x,T mod,T q) const{
        if(size<=T(0)) return T(0);
        // q must be modpow(x,diff,mod)
        return modpow(x,first,mod)*geometric_sum(size-T(1),q,mod)%mod;
    }
};

// returns a collection of arithmetic sequences of ((a*i+b) mod m) for [0,x].
template<class T>
vector<sequence<T>> mod_sequence(T const&x,T a,T b,T m){
    if(x<T(0)) return {};
    if(m==T(0)) return {};
    if(m<T(0))return mod_sequence(x,a,b,-m);
    if(a<T(0))return mod_sequence(x,-a,a*x+b,m);
    a=mod_floor(a,m);
    b=mod_floor(b,m);
    if(a==T(0)){ return {{b,T(0),x+1}}; }

    T D=sqrt(x+1);
    T dx=1;
    T best=m;
    for(T i=1;i<=D;++i){
        T v=a*i%m;
        T val=min(v,m-v);
        if(best>val){
            best=val;
            dx=i;
        }
    }
    T dy=a*dx%m;
    bool const flip=dy>m-dy;
    if(flip){
        a=mod_floor(-a,m);
        b=m-1-b;
        dy=m-dy;
    }
    vector<sequence<T>> ans;
    for(T di=0;di<dx;++di){
        T y=(b+a*di)%m;
        T cnt=(x-di)/dx+1;
        if(dy==0){
            ans.push_back({y,T(0),cnt});
            continue;
        }
        T lim=(dy*(cnt-1)+y)/m;
        T le=0;
        for(T k=0;k<=lim;++k){
            T ri=k==lim?cnt:(m*(k+1)-y+dy-1)/dy;
            ans.push_back({(y+dy*le)%m,dy,ri-le});
            le=ri;
        }
    }
    if(flip){
        for(auto&s:ans)s=sequence<T>(m-1-s.first,-s.diff,s.size);
    }
    return ans;
}



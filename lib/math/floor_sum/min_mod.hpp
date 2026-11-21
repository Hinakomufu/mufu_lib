#pragma once
#include <bits/stdc++.h>
#include "../floor_sum/floor_sum.hpp"
using namespace std;

// Store the positions where the prefix minimum is updated
template<class T>
pair<vector<T>,vector<T>>
    min_mod_segments(T const a,T const b,T const m){
    // p/q<=(m-a)/m<=r/s
    T p=0,q=1;
    T r=1,s=1;
    T det_l=m-a;
    T det_r=a;
    T pos=0;
    T y=b;
    vector<T> X={0};
    vector<T> DX;
    while(y!=T(0)){
        // Update r/s
        T k=det_r/det_l;
        det_r%=det_l;
        if(det_r==T(0)){
            --k;
            det_r=det_l;
        }
        r+=k*p;
        s+=k*q;
        while(true){
            // ceil((det_l-y)/det_r)
            k=max(T(0),(det_l-y+det_r-1)/det_r);
            if(det_l-k*det_r<=T(0))break;
            det_l-=k*det_r;
            p+=k*r;
            q+=k*s;
            // aq-pm = det_l
            k=y/det_l;
            y-=k*det_l;
            pos+=q*k;
            X.push_back(pos);
            DX.push_back(q);
        }
        k=det_l/det_r;
        det_l-=k*det_r;
        p+=k*r;
        q+=k*s;
    }
    return {X,DX};
}

// min in [0,x] of (ai+b) mod m (m!=0)
template<class T>
T min_mod(T const&x,T a,T b,T m){
    if(x<T(0)||m==T(0))return T(-1);
    if(m<T(0))return min_mod(x,a,b,-m);
    if(a<T(0))return min_mod(x,-a,a*x+b,m);
    a=mod_floor(a,m);
    b=mod_floor(b,m);
    if(a==T(0))return b;
    T g=gcd(a,m);
    T const bg=b%g;
    a/=g;
    b/=g;
    m/=g;
    auto [X, DX]=min_mod_segments(a,b,m);
    T pos=T(0);
    for(int i=0;i+1<static_cast<int>(X.size());++i){
        T const xl=X[i];
        T const xr=X[i+1];
        if(xr<=x){
            pos=xr;
            continue;
        }
        pos=xl+(x-pos)/DX[i]*DX[i];
        break;
    }
    T const reduced=(a*pos+b)%m;
    return bg+g*reduced;
}

template<class T>
T min_mod_range(T const&l,T const&r,T a,T b,T m){
    if(l>r||m==T(0))return T(-1);
    return min_mod(r-l,a,a*l+b,m);
}



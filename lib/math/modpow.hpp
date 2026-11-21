#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class T>
T modpow(T a,T b,T m){ // mod m of a^b
    if(m<=T(1))return T(0);
    T x=1;
    while(b>T(0)){
        if(b&T(1)) x=(x*a)%m;
        a=(a*a)%m;
        b>>=T(1);
    }
    return x;
}

template<class T>
T geometric_sum(T n,T x,T m,T& p){
    // p=x^(n+1) mod m
    if(m<=T(1)){
        p=T(0);
        return T(0);
    }
    if(n<T(0)){
        p=T(1)%m;
        return T(0);
    }
    T len=n+1;
    T s=0;
    T q=1;
    T bs=1;
    T bp=x%m;
    while(len>T(0)){
        if(len&T(1)){
            s=(s+q*bs)%m;
            q=q*bp%m;
        }
        bs=bs*(1+bp)%m;
        bp=bp*bp%m;
        len>>=1;
    }
    p=q;
    return s;
}

template<class T>
T geometric_sum(T n,T x,T m){ // mod of (sum of x^i [0,n])
    if(m<=T(1)) return T(0);
    T p;
    return geometric_sum(n,x,m,p);
}



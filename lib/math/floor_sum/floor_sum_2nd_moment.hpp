#pragma once
#include <bits/stdc++.h>
#include "../floor_sum/floor_sum.hpp"

// sum in [0,x] of n^p * floor((an+b)/m)^q (p+q<=2,m!=0)
template<class T>
struct floor_sum_2nd{
    private:
        T moment[3][3]{}; // moment[p][q], p+q<=2
        // sum in [0,x] of n^p * floor((an+b)/m)^q
        static floor_sum_2nd<T> calc(T const x,T a,T b,T const m){
            if(x<T(0)) return {};
            if(m==T(0)) return {};
            if(m<T(0)) return calc(x,-a,-b,-m);
            if(a<T(0)){
                auto const fs=calc(x,-a,a*x+b,m);
                floor_sum_2nd<T> res=fs;
                res.moment[1][1]=x*fs.moment[0][1]-fs.moment[1][1];
                return res;
            }
            T const qa=div_floor(a,m);
            T const qb=div_floor(b,m);
            a=mod_floor(a,m);
            b=mod_floor(b,m);
            T const N=x+T(1);
            T const S0=N;
            T const S1=x*(x+T(1))/T(2);
            T const S2=x*(x+T(1))*(T(2)*x+T(1))/T(6);
            floor_sum_2nd<T> res;
            res.moment[0][0]=S0;
            res.moment[1][0]=S1;
            res.moment[2][0]=S2;
            if(a==T(0)||a*x+b<m){
                res.moment[0][1]=qa*S1+qb*S0;
                res.moment[1][1]=qa*S2+qb*S1;
                res.moment[0][2]=qa*qa*S2+T(2)*qa*qb*S1+qb*qb*S0;
                return res;
            }
            T const y=div_floor(a*x+b,m);
            auto const fs=calc(y-T(1),m,m+a-b-T(1),a);
            T const H01=fs.moment[0][1];
            T const H11=fs.moment[1][1]+fs.moment[0][1];
            T const G01=N*y-H01;
            T const G11=y*S1-(fs.moment[0][2]-fs.moment[0][1])/T(2);
            T const G02=N*y*y-T(2)*H11+H01;
            res.moment[0][1]=qa*S1+qb*S0+G01;
            res.moment[1][1]=qa*S2+qb*S1+G11;
            res.moment[0][2]=qa*qa*S2+T(2)*qa*qb*S1+qb*qb*S0+T(2)*qa*G11+T(2)*qb*G01+G02;
            return res;
        }

    public:
        floor_sum_2nd() = default;

        floor_sum_2nd(T const x,T const a,T const b,T const m){ *this=calc(x,a,b,m); }

        T&operator()(int const p,int const q){ return moment[p][q]; }
        T const&operator()(int const p,int const q) const{ return moment[p][q]; }
};

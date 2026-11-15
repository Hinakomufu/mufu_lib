#pragma once
#include <bits/stdc++.h>
using namespace std;

using ld=long double;
inline ld inf=1L<<60;
inline ld eps=1L/(1L<<20);
inline ld pi=acosl(-1.0);
inline ld torad(long const deg){ return static_cast<ld>(deg)*pi/180; }
inline ld todeg(ld const ang){ return ang*180/pi; }

// Sort by polar angle (by counterclockwise) in radians
// zero_2pi=true -> [0,2pi),zero_2pi=false -> (-pi,pi]
// if(angle_cmp(a,b)), a comes before b in polar angle order
inline bool angle_comparison(const pair<long,long>&a,const pair<long,long>&b,
                             const bool counterclockwise=true,const bool zero_2pi=true){
    auto group=[&](const pair<long,long>&p){
        if(p.second<0) return (zero_2pi?4:0); // Negative angle
        if(p.second==0&&p.first>=0) return 1; // Angle zero
        if(p.second>0) return 2;              // Positive angle
        return 3;                             // Angle pi
    };

    int const ca=group(a);
    int const cb=group(b);
    if(ca!=cb) return (counterclockwise?ca<cb:ca>cb);

    if(a.second==0&&b.second==0) return false; // a:0 or pi
    long const cross=a.first*b.second-a.second*b.first;
    return (counterclockwise?cross>0:cross<0);
}

// if(angle_cmp(a,b)), a comes before b in polar angle order
inline bool angle_cmp(const pair<long,long>&a,const pair<long,long>&b){
    return angle_comparison(a,b,false,false);
}

// Sort by absolute polar angle (ascending) in radians [0, pi]
inline bool angle_abs_cmp(const pair<long,long>&a,const pair<long,long>&b){
    auto reflect=[](pair<long,long> p){
        if(p.second<0) p.second=-p.second;
        return p;
    };
    return angle_comparison(reflect(a),reflect(b),true,false);
}

// Return angle ABC in radians [0, pi]
inline ld angle_rad(const pair<long,long>&a,const pair<long,long>&b,const pair<long,long>&c){
    ld const bax=a.first-b.first;
    ld const bay=a.second-b.second;
    ld const bcx=c.first-b.first;
    ld const bcy=c.second-b.second;

    ld const dot=bax*bcx+bay*bcy;
    ld const cross=bax*bcy-bay*bcx;
    return atan2l(abs(cross),dot);
}

// Return angle ABC in degrees [0, 180]
inline ld angle_deg(const pair<long,long>&a,const pair<long,long>&b,const pair<long,long>&c){
    return todeg(angle_rad(a,b,c));
}



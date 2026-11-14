#pragma once
//https://github.com/Hinakomufu/mufu_lib/blob/main/lib/segtree/normal/algorithm/count(single).hpp
#include <bits/stdc++.h>
#include "../single.hpp"
#include "../../monoid/sum(single).hpp"
using namespace std;

// mp[val[i]]->a[i]
// compressed values,reverse map
template<class T>
pair<vector<long>,vector<T>> compress(const vector<T>&a){
    int const n=a.size();
    vector<T> t=a;
    sort(t.begin(),t.end());
    t.erase(unique(t.begin(),t.end()),t.end());
    vector<long> b(n);
    for(long i=0;i<n;i++){
        long const idx=lower_bound(t.begin(),t.end(),a[i])-t.begin();
        b[i]=idx; // compressed index (0-based)
    }
    // b: compressed values, t: reverse map
    return {b,t};
}

template<class T>
vector<long> direct_count(vector<T> const&v,bool const right_dir=true,bool const larger=true,bool const strict=true){
    long const n=v.size();
    auto [val, mp]=compress(v);
    vector<long> cnt(n);
    long const mx=*max_element(val.begin(),val.end());
    vector<long> const a(mx+1,0L);
    segtree<Sum<long>> s(a);
    long const start=right_dir?n-1:0;
    long const end=right_dir?-1:n;
    long const step=right_dir?-1:1;
    for(long i=start;i!=end;i+=step){
        long const x=val[i];
        if(larger){ // larger: i<j -> s[i]<s[j]
            long const lim=strict?x+1:x;
            if(0<=lim)cnt[i]=s(lim,mx);
        } else{ // smaller: i<j -> s[i]>s[j]
            long const lim=strict?x-1:x;
            if(0<=lim)cnt[i]=s(0,lim);
        }
        ++s.update(x);
    }
    return cnt;
}

template<class T>
long inversion(vector<T> const&v){
    vector<long> const u=direct_count(v,true,false,true);
    long val=0;
    for(long const x:u) val+=x;
    return val;
}



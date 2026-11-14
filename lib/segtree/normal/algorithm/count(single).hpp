#pragma once
#include <bits/stdc++.h>
#include "../single.hpp"
#include "../../monoid/sum(single).hpp"
using namespace std;

// mp[val[i]]->a[i]
// compressed values,reverse map
inline pair<vector<long>,vector<long>>compress(const vector<long>&a){
    int const n=a.size();
    vector<long> t=a;
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

inline vector<long>direct_count(vector<long> const&v,bool const right_dir=true,
    bool const larger=true,bool const strict=true){
    long const n=v.size();
    auto [val, mp]=compress(v);
    vector<long> cnt(n);
    long const mx=*max_element(val.begin(),val.end());
    vector<long> const a(mx+1,0L);
    segtree<Sum> s(a);
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

inline long inversion(vector<long> const&v){
    vector<long> const u=direct_count(v,true,false,true);
    long val=0;
    for(long const x:u) val+=x;
    return val;
}



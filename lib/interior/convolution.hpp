#pragma once
#include <bits/stdc++.h>
using namespace std;

//
inline int max_degree=2000000;
//

template<class mint>
vector<mint> convolution(vector<mint> a,vector<mint> b,int const n=max_degree){
    assert(0<=n&&n<=max_degree);
    assert(mint::can_ntt());
    auto [rank_2,primitive_root]=mint::ntt_info();

    auto ntt=[&](vector<mint>&res,bool const invert){
        int const MOD=mint::mod_val();
        int sz=res.size();
        assert(sz<=(1<<rank_2));
        static vector<int> rev;
        static vector<mint> roots{0,1};

        if(static_cast<int>(rev.size())!=sz){
            int const k=__builtin_ctz(sz);
            rev.resize(sz);

            for(int i=0;i<sz;++i){
                rev[i]=(rev[i>>1]>>1)
                       |((i&1)<<(k-1));
            }
        }

        if(static_cast<int>(roots.size())<sz){
            int k=__builtin_ctz(roots.size());
            roots.resize(sz);

            while((1<<k)<sz){
                mint e=mint(primitive_root).pow((MOD-1)>>(k+1));
                for(int i=1<<(k-1);i<(1<<k);++i){
                    roots[2*i]=roots[i];
                    roots[2*i+1]=roots[i]*e;
                }
                ++k;
            }
        }

        for(int i=0;i<sz;++i){
            if(i<rev[i])
                swap(res[i],res[rev[i]]);
        }

        for(int len=1;len<sz;len<<=1){
            for(int i=0;i<sz;i+=len<<1){
                for(int j=0;j<len;++j){
                    int const u=res[i+j].v;
                    int const v=static_cast<long>(res[i+j+len].v)
                                *roots[len+j].v%MOD;
                    int x=u+v;
                    if(x>=MOD) x-=MOD;
                    int y=u-v;
                    if(y<0) y+=MOD;
                    res[i+j].v=x;
                    res[i+j+len].v=y;
                }
            }
        }

        if(invert){
            reverse(res.begin()+1,res.end());
            int const inv_n=mint(sz).inv().v;
            for(auto&x:res){
                x.v=static_cast<long>(x.v)*inv_n%MOD;
            }
        }
    };

    if(a.empty()||b.empty()) return {};
    int sz=1;
    int const l=min(static_cast<int>(a.size()),n+1);
    int const r=min(static_cast<int>(b.size()),n+1);
    a.resize(l);
    b.resize(r);
    while(l+r-1>sz) sz<<=1;
    a.resize(sz);
    b.resize(sz);

    ntt(a,false);
    ntt(b,false);
    for(int i=0;i<sz;i++) a[i]*=b[i];
    ntt(a,true);
    a.resize(min(n+1,l+r-1));
    return a;
}



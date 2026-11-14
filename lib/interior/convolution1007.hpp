#pragma once
#include <bits/stdc++.h>
using namespace std;

//
inline int max_degree=2000000;
//

template<class mint>
vector<mint> convolution(vector<mint> a,vector<mint> b,int const n=max_degree){
    assert(0<=n&&n<=max_degree);

    auto conv=[]<class T>(vector<T> A,vector<T> B,int const N){
        auto ntt=[](vector<T>&res,bool const invert){
            int const MOD=T::mod_val();
            int sz=res.size();

            static vector<int> rev;
            static vector<T> roots{0,1};

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
                    T e=T(3).pow((MOD-1)>>(k+1));
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
                int const inv_n=T(sz).inv().v;
                for(auto&x:res){
                    x.v=static_cast<long>(x.v)*inv_n%MOD;
                }
            }
        };

        if(A.empty()||B.empty()) return vector<T>{};
        int sz=1;
        int const l=min(static_cast<int>(A.size()),N+1);
        int const r=min(static_cast<int>(B.size()),N+1);
        A.resize(l);
        B.resize(r);
        while(l+r-1>sz) sz<<=1;
        A.resize(sz);
        B.resize(sz);

        ntt(A,false);
        ntt(B,false);
        for(int i=0;i<sz;i++) A[i]*=B[i];
        ntt(A,true);
        A.resize(min(N+1,l+r-1));
        return A;
    };

    constexpr long p1=998244353;
    constexpr long p2=469762049;
    constexpr long p3=167772161;

    using mint1=modint<p1>;
    using mint2=modint<p2>;
    using mint3=modint<p3>;

    int const a_sz=a.size();
    int const b_sz=b.size();

    vector<mint1> a1(a_sz),b1(b_sz);
    vector<mint2> a2(a_sz),b2(b_sz);
    vector<mint3> a3(a_sz),b3(b_sz);

    for(int i=0;i<a_sz;i++){
        a1[i]=a[i].v;
        a2[i]=a[i].v;
        a3[i]=a[i].v;
    }

    for(int i=0;i<b_sz;i++){
        b1[i]=b[i].v;
        b2[i]=b[i].v;
        b3[i]=b[i].v;
    }

    auto c1=conv(a1,b1,n);
    auto c2=conv(a2,b2,n);
    auto c3=conv(a3,b3,n);

    vector<mint> c(a_sz+b_sz-1);
    auto crt=[](long r1,long const r2,long const r3){
        mint2 const t2=(mint2(r2)-mint2(r1))/mint2(p1);
        mint3 const t3=(mint3(r3)-mint3(r1)-mint3(p1)*t2.v)/(mint3(p1)*mint3(p2));
        return mint(r1)+mint(p1)*t2.v+mint(p1)*mint(p2)*t3.v;
    };

    for(int i=0;i<a_sz+b_sz-1;i++){ c[i]=crt(c1[i].v,c2[i].v,c3[i].v); }

    return c;
}

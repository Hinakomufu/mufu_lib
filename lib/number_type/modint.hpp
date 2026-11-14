#pragma once
#include <bits/stdc++.h>
using namespace std;

template<int mod>
struct modint{
    int v;

    constexpr modint(long const _v=0){
        long x=_v%mod;
        if(x<0) x+=mod;
        v=static_cast<int>(x);
    }

    constexpr operator int() const{ return v; }
    static constexpr int mod_val(){ return mod; }
    constexpr modint operator-() const{ return modint(v==0?0:mod-v); }

    static constexpr modint raw(int v){
        modint x;
        x.v=v;
        return x;
    }

    // NTT info: {max power of 2, primitive root}
    static constexpr pair<int,int> ntt_info(){
        if(mod==120586241) return {20,74066978};
        if(mod==167772161) return {25,17};
        if(mod==469762049) return {26,30};
        if(mod==754974721) return {24,362};
        if(mod==880803841) return {23,211};
        if(mod==943718401) return {22,663003469};
        if(mod==998244353) return {23,31};
        if(mod==1045430273) return {20,363};
        if(mod==1051721729) return {20,330};
        if(mod==1053818881) return {20,2789};
        return {-1,-1};
    }

    static constexpr bool can_ntt(){ return ntt_info().first>=0; }

    friend std::istream&operator>>(std::istream&is,modint&m){
        long x;
        is>>x;
        m=modint(x);
        return is;
    }

    friend std::ostream&operator<<(std::ostream&os,const modint&m){
        return os<<m.v;
    }

    constexpr modint&operator=(long const x){
        long y=x%mod;
        if(y<0) y+=mod;
        v=static_cast<int>(y);
        return *this;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr modint operator+(T x) const{
        return *this+modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr modint operator+(T x,const modint&m){
        return modint(x)+m;
    }

    constexpr modint operator+(const modint&other) const{
        int x=v+other.v;
        if(x>=mod) x-=mod;
        return modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr modint operator-(T x) const{
        return *this-modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr modint operator-(T x,const modint&m){
        return modint(x)-m;
    }

    constexpr modint operator-(const modint&other) const{
        int x=v-other.v;
        if(x<0) x+=mod;
        return modint(x);
    }

    constexpr modint operator*(const modint&other) const{
        return modint(1LL*v*other.v%mod);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr modint operator*(T x) const{
        return *this*modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr modint operator*(T x,const modint&m){
        return modint(x)*m;
    }

    constexpr modint pow(long n) const{
        if(n<0) return inv().pow(-n);
        modint r(1),x(*this);
        while(n>0){
            if(n&1) r=r*x;
            x=x*x;
            n>>=1;
        }
        return r;
    }

    constexpr modint inv() const{ // mod:prime only
        return pow(mod-2);
    }

    constexpr modint operator/(const modint&other) const{
        return *this*other.inv();
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr modint operator/(T x) const{
        return *this*modint(x).inv();
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr modint operator/(T x,const modint&m){
        return modint(x)*m.inv();
    }

    constexpr modint&operator+=(const modint&other){
        v+=other.v;
        if(v>=mod) v-=mod;
        return *this;
    }

    constexpr modint&operator-=(const modint&other){
        v-=other.v;
        if(v<0) v+=mod;
        return *this;
    }

    constexpr modint&operator*=(const modint&other){
        v=static_cast<int>(1LL*v*other.v%mod);
        return *this;
    }

    constexpr modint&operator/=(const modint&other){
        return *this*=other.inv();
    }

    constexpr modint&operator++(){
        v++;
        if(v==mod) v=0;
        return *this;
    }

    constexpr modint operator++(int){
        modint tmp=*this;
        ++*this;
        return tmp;
    }

    constexpr modint&operator--(){
        if(v==0) v=mod-1;
        else v--;
        return *this;
    }

    constexpr modint operator--(int){
        modint tmp=*this;
        --*this;
        return tmp;
    }

    friend constexpr bool operator==(const modint&a,const modint&b){
        return a.v==b.v;
    }

    friend constexpr bool operator!=(const modint&a,const modint&b){
        return a.v!=b.v;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr bool operator==(T x) const{
        return v==x;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr bool operator!=(T x) const{
        return v!=x;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr bool operator==(T x,const modint&m){
        return x==m.v;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr bool operator!=(T x,const modint&m){
        return x!=m.v;
    }
};

template<class mint>
struct combination{
    private:
        int max_n;
        vector<mint> factorial,inv_factorial;
        vector<mint> power,inv_power;

    public:
        explicit combination(int n,mint r): max_n(n){
            factorial.resize(n+1);
            inv_factorial.resize(n+1);
            power.resize(n+1);
            inv_power.resize(n+1);

            factorial[0]=1;
            for(long i=1;i<=n;i++) factorial[i]=factorial[i-1]*i;

            inv_factorial[n]=factorial[n].inv();
            for(long i=n;i>0;i--) inv_factorial[i-1]=inv_factorial[i]*i;

            power[0]=1;
            for(int i=1;i<=n;i++) power[i]=power[i-1]*r;

            inv_power[n]=power[n].inv();
            for(int i=n;i>0;i--) inv_power[i-1]=inv_power[i]*r;
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint fact(T n) const{
            if(n<0||n>max_n) return mint(1);
            return factorial[n];
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint invfact(T n) const{
            if(n<0||n>max_n) return mint(1);
            return inv_factorial[n];
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint mintpow(T n) const{
            if(n<0||n>max_n) return mint(1);
            return power[n];
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint mintinvpow(T n) const{
            if(n<0||n>max_n) return mint(1);
            return inv_power[n];
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint C(T n,T r) const{
            if(n<0||r<0||r>n) return mint(0);
            if(n<=max_n)return factorial[n]*inv_factorial[r]*inv_factorial[n-r];
            mint res=1;
            for(T i=0;i<r;++i){
                res*=mint(n-i);
                res/=mint(i+1);
            }
            return res;
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint P(T n,T const r) const{
            if(n<0||r<0||r>n) return mint(0);
            if(n<=max_n)return factorial[n]*inv_factorial[n-r];
            mint res=1;
            for(T i=0;i<r;++i){
                res*=mint(n-i);
            }
            return res;
        }
};

using modint167772161 = modint<167772161>;
using modint469762049 = modint<469762049>;
using modint998244353 = modint<998244353>;
using modint1000000007 = modint<1000000007>;



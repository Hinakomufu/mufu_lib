#pragma once
//https://github.com/Hinakomufu/mufu_lib/blob/main/lib/number_type/longint.hpp
#include <bits/stdc++.h>
using namespace std;

struct longint{
    private:
        __int128_t v; //value
        void from_string(const string&s){
            if(s.empty())return;
            v=0;
            int i=0;
            bool neg=false;
            if(s[i]=='-'){
                neg=true;
                i++;
            } else if(s[i]=='+'){
                i++;
            }
            for(;i<static_cast<int>(s.size());i++){
                char const c=s[i];
                if('0'<=c&&c<='9'){
                    v=v*10-(c-'0');
                }
            }
            if(!neg) v=-v;
        }

    public:
        longint() : v(0){}
        longint(int const x) : v(x){}
        longint(long const x) : v(x){}
        longint(long long const x) : v(x){}
        longint(__int128_t const x) : v(x){}

        operator int() const{ return static_cast<int>(v); }
        operator long() const{ return static_cast<long>(v); }
        operator long long() const{ return static_cast<long long>(v); }
        operator __int128_t() const{ return v; }
        operator double() const{ return static_cast<double>(v); }
        operator long double() const{ return static_cast<long double>(v); }
        explicit operator bool() const{ return v!=0; }

        string to_string() const{
            if(v==0) return "0";
            bool const neg=v<0;
            __uint128_t x;
            if(neg)x=static_cast<__uint128_t>(-(v+1))+1;
            else x=static_cast<__uint128_t>(v);
            string s;
            while(x>0){
                s.push_back('0'+static_cast<int>(x%10));
                x/=10;
            }
            if(neg) s.push_back('-');
            reverse(s.begin(),s.end());
            return s;
        }

        static longint l_stol(const std::string&s){
            longint x;
            x.from_string(s);
            return x;
        }

        longint operator-() const{ return longint(-v); }
        bool operator<(const longint&y) const{ return v<y.v; }
        bool operator>(const longint&y) const{ return v>y.v; }
        bool operator<=(const longint&y) const{ return v<=y.v; }
        bool operator>=(const longint&y) const{ return v>=y.v; }
        bool operator==(const longint&y) const{ return v==y.v; }
        bool operator!=(const longint&y) const{ return v!=y.v; }

        longint operator+(const longint&y) const{ return longint(v+y.v); }
        longint operator-(const longint&y) const{ return longint(v-y.v); }
        longint operator*(const longint&y) const{ return longint(v*y.v); }
        longint operator/(const longint&y) const{ return longint(v/y.v); }
        longint operator%(const longint&y) const{ return longint(v%y.v); }

        longint operator&(const longint&y) const{ return longint(v&y.v); }
        longint operator|(const longint&y) const{ return longint(v|y.v); }
        longint operator^(const longint&y) const{ return longint(v^y.v); }

        longint operator<<(uint64_t const k) const{
            if(k>=127) return longint(0);
            return longint(v<<k);
        }

        longint operator>>(uint64_t const k) const{
            if(k>=127) return longint(v>=0?0:-1);
            return longint(v>>k);
        }

        longint&operator+=(const longint&y){
            *this=*this+y;
            return *this;
        }

        longint&operator-=(const longint&y){
            *this=*this-y;
            return *this;
        }

        longint&operator*=(const longint&y){
            *this=*this*y;
            return *this;
        }

        longint&operator/=(const longint&y){
            *this=*this/y;
            return *this;
        }

        longint&operator%=(const longint&y){
            *this=*this%y;
            return *this;
        }

        longint&operator&=(const longint&y){
            *this=*this&y;
            return *this;
        }

        longint&operator|=(const longint&y){
            *this=*this|y;
            return *this;
        }

        longint&operator^=(const longint&y){
            *this=*this^y;
            return *this;
        }

        longint&operator<<=(const longint&y){
            uint64_t const k=static_cast<uint64_t>(y.v);
            *this=*this<<k;
            return *this;
        }

        longint&operator>>=(const longint&y){
            uint64_t const k=static_cast<uint64_t>(y.v);
            *this=*this>>k;
            return *this;
        }

        longint&operator++(){
            *this+=longint(1);
            return *this;
        }

        longint operator++(int){
            longint const tmp=*this;
            *this+=longint(1);
            return tmp;
        }

        longint&operator--(){
            *this-=longint(1);
            return *this;
        }

        friend istream&operator>>(istream&is,longint&x){
            string s;
            is>>s;
            x.from_string(s);
            return is;
        }

        longint operator--(int){
            longint const tmp=*this;
            *this-=longint(1);
            return tmp;
        }

        friend ostream&operator<<(ostream&os,const longint&x){
            os<<x.to_string();
            return os;
        }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator+(Int const x) const{ return *this+longint(x); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator-(Int const x) const{ return *this-longint(x); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator*(Int const x) const{ return *this*longint(x); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator/(Int const x) const{ return *this/longint(x); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator<<(Int const k) const{
            uint64_t const s=static_cast<uint64_t>(k);
            if(s>=127)return longint(0);
            return longint(v<<s);
        }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator>>(Int const k) const{
            uint64_t const s=static_cast<uint64_t>(k);
            if(s>=127)return longint(v>=0?0:-1);
            return longint(v>>s);
        }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend longint operator+(Int const x,const longint&f){ return longint(x)+f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend longint operator-(Int const x,const longint&f){ return longint(x)-f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend longint operator*(Int const x,const longint&f){ return longint(x)*f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend longint operator/(Int const x,const longint&f){ return longint(x)/f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator==(Int const&y) const{ return v==static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator!=(Int const&y) const{ return v!=static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator<(Int const&y) const{ return v<static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator>(Int const&y) const{ return v>static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator<=(Int const&y) const{ return v<=static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator>=(Int const&y) const{ return v>=static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator==(Int const&x,const longint&y){ return static_cast<__int128>(x)==y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator!=(Int const&x,const longint&y){ return static_cast<__int128>(x)!=y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator<(Int const&x,const longint&y){ return static_cast<__int128>(x)<y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator>(Int const&x,const longint&y){ return static_cast<__int128>(x)>y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator<=(Int const&x,const longint&y){ return static_cast<__int128>(x)<=y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator>=(Int const&x,const longint&y){ return static_cast<__int128>(x)>=y.v; }

        int l_popcount() const{
            uint64_t const lo=static_cast<uint64_t>(v);
            uint64_t const hi=static_cast<uint64_t>(v>>64);
            return __builtin_popcountll(lo)+__builtin_popcountll(hi);
        }

        longint l_sqrt() const{
            assert(v>=0);
            if(v==0) return longint(0);
            __uint128_t const x=static_cast<__uint128_t>(v);
            __uint128_t r=x;
            while(true){
                __uint128_t const nr=(r+x/r)>>1;
                if(nr>=r) break;
                r=nr;
            }
            return longint(static_cast<__int128_t>(r));
        }

        static longint l_floor_i_root(longint const n,int const i){
            auto ok=[&](longint const&x){
                longint v=1;
                for(int k=0;k<i;k++){
                    v*=x;
                    if(v>n) return false; // x^i > n -> false
                }
                return true; // x^i <= n -> true
            };

            longint l=1,r=n,ans=1;
            while(l<=r){
                longint m=(l+r)/2;
                if(ok(m)){
                    ans=m;
                    l=m+1;
                } else r=m-1;
            }
            return ans;
        }

        static longint l_gcd(longint const a,longint const b){
            __int128_t x=a.v;
            __int128_t y=b.v;
            if(x<0) x=-x;
            if(y<0) y=-y;
            __uint128_t ux=static_cast<__uint128_t>(x);
            __uint128_t uy=static_cast<__uint128_t>(y);
            while(uy!=0){
                __uint128_t const r=ux%uy;
                ux=uy;
                uy=r;
            }
            return longint(static_cast<__int128_t>(ux));
        }

        static longint l_lcm(longint const a,longint const b){ return a/l_gcd(a,b)*b; }
};

template<>
struct std::numeric_limits<longint>{
    public:
        static constexpr bool is_specialized=true;
        static longint max() noexcept{ return (longint(1)<<126)-1+(longint(1)<<126); }
        static longint lowest() noexcept{ return -max()-1; }
        static longint min() noexcept{ return lowest(); }

        static constexpr int digits=127;
        static constexpr int digits10=38;
        static constexpr bool is_signed=true;
        static constexpr bool is_integer=true;
        static constexpr bool is_exact=true;
};

inline longint stol(string const&s){ return longint::l_stol(s); }
inline longint sqrt(longint const&a){ return a.l_sqrt(); }
inline longint floor_i_root(longint const&n,int const&i){ return longint::l_floor_i_root(n,i); }
inline longint gcd(longint const&a,longint const&b){ return longint::l_gcd(a,b); }
inline longint lcm(longint const&a,longint const&b){ return longint::l_lcm(a,b); }
using lint=longint;



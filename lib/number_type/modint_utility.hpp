#pragma once
#include <bits/stdc++.h>
#include "modint.hpp"
using namespace std;

// clang-format off
static combination<mint> comb(1000000,2);
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint mintpow(T const n){ return comb.mintpow(n); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint mintinvpow(T const n){return comb.mintinvpow(n);}
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint modpow(mint const a,T const n){ return a.pow(n); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint fact(T n){ return comb.fact(n); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint invfact(T n){ return comb.invfact(n); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint c(T n,T r){ return comb.C(n,r); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint p(T n,T r){ return comb.P(n,r); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint h(T n,T r){ return comb.C(n+r-1,r); }
// clang-format on



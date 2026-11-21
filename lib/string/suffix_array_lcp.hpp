#pragma once
#include "../data_struct/sparse_table.hpp"
#include "../monoid/min_max(single).hpp"
#include "suffix_array.hpp"

template<class T>
struct suffix_array_lcp:suffix_array<T>{
    private:
        using Base=suffix_array<T>;
        mutable optional<sparse_table<Min<int>>> lcp_st;
        int adjacent_lcp(int const i) const{ return this->next_lcp(i); }

    public:
        using Base::Base;

        int lcp(int const i,int const j) const{
            if(i==j) return this->size()-i;
            int l=this->rank(i);
            int r=this->rank(j);
            if(l>r) swap(l,r);
            if(r-l==1) return this->next_lcp(l);
            if(!lcp_st){ lcp_st.emplace(this->next_lcp()); }
            return (*lcp_st)(l,r-1);
        }

        T lcp_substring(int const i,int const j) const{
            int len=lcp(i,j);
            const T&s=this->str();
            return T(s.begin()+i,s.begin()+i+len);
        }

        T operator()(int const i,int const j) const{ return lcp_substring(i,j); }
};



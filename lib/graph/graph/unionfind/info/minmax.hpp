#pragma once
#include <bits/stdc++.h>
#include "../unionfind.hpp"
using namespace std;

template<typename U>
struct MinMax{
    U max_val=numeric_limits<U>::lowest()+1;
    U min_val=numeric_limits<U>::max()-1;

    void add_edge(MinMax const&){}

    void merge(MinMax const& other) {
        max_val=max(max_val,other.max_val);
        min_val=min(min_val,other.min_val);
    }

    void compress(MinMax const&){}
};



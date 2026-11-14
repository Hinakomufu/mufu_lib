#pragma once
#include <bits/stdc++.h>
#include "../../edge.hpp"
using namespace std;

template<typename U>
struct Sum{
    U sum_val=0;

    void add_edge(Sum const&){}
    void merge(Sum const& other) {
        sum_val+=other.sum_val;
    }

    void compress(Sum const&){}
};



#pragma once
#include <bits/stdc++.h>
using namespace std;

template<typename U>
struct edge{
    int from;
    int to;
    U weight;
    int id;

    edge(int const u=-1,int const v=-1,U const w=0,int const id=-1): from(u), to(v), weight(w), id(id){}

    bool operator<(edge const&other) const{ return tie(weight,id)<tie(other.weight,other.id); }
    bool operator>(edge const&other) const{ return tie(weight,id)>tie(other.weight,other.id); }
};



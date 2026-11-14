#pragma once
#include <bits/stdc++.h>
#include "../tree/tree.hpp"
using namespace std;

template<typename T>
struct cartesian_tree{
    private:
        bool is_max;

        vector<int> cartesian_parent(){
            int const n=val.size();
            parents.assign(n,-1);
            vector<int> st;
            for(int i=0;i<n;++i){
                int last=-1;
                while(!st.empty()&&(is_max?val[st.back()]<val[i]:val[st.back()]>val[i])){
                    last=st.back();
                    st.pop_back();
                }
                if(!st.empty())parents[i]=st.back();
                if(last!=-1)parents[last]=i;
                st.push_back(i);
            }
            return parents;
        }

        void build_ranges(){
            int const n=val.size();
            left.resize(n);
            right.resize(n);
            iota(left.begin(),left.end(),0);
            iota(right.begin(),right.end(),0);
            order.clear();
            order.reserve(n);
            if(n==0)return;
            order.push_back(root());
            for(int i=0;i<static_cast<int>(order.size());i++){
                int v=order[i];
                for(int u:tree.node(v).child_nodes())order.push_back(u);
            }
            for(int i=n-1;i>=0;i--){
                int v=order[i];
                int p=parents[v];
                if(p==-1)continue;
                left[p]=min(left[p],left[v]);
                right[p]=max(right[p],right[v]);
            }
        }

    public:
        vector<T> val;
        vector<int> parents;
        tree_hld<T> tree;
        vector<int> left;
        vector<int> right;
        vector<int> order;

        explicit cartesian_tree(vector<T> const&v,bool const is_max=false): is_max(is_max), val(v){
            parents=cartesian_parent();
            tree=tree_hld<T>(parents);
            build_ranges();
        }

        vector<int> const&parent_nodes() const{ return parents; }
        T operator()(int i) const{ return val[i]; }

        int arg_top(int l,int r){
            if(l>r)swap(l,r);
            if(l<0||static_cast<int>(val.size())<=r)return -1;
            return tree.path(l,r).lca();
        }

        int root(){ return arg_top(0,static_cast<int>(val.size())-1); }
        T top_val(int const l,int const r) const{ return val[arg_top(l,r)]; }
        pair<int,int> top_range(int const p) const{ return {left[p],right[p]}; }
        int top_range_count(int const p) const{ return right[p]-left[p]+1; }
};



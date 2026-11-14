#pragma once
#include <bits/stdc++.h>
using namespace std;

struct default_info{
    void add_edge(default_info const&) const{}
    void merge(default_info const&) const{}
    void compress(default_info const&) const{}
};

template<class Info = default_info>
struct unionfind{
    private:
        vector<int> parent;
        vector<int> node_count;
        vector<int> edge_count;
        int components=0;
        int n;

    public:
        vector<Info> info;

        explicit unionfind(int const n): parent(n,-1), node_count(n,1), edge_count(n,0), components(n),
                                         n(n), info(n){}

        int add_node(){
            parent.push_back(-1);
            node_count.push_back(1);
            edge_count.push_back(0);
            info.emplace_back();
            components++;
            n++;
            return parent.size()-1; // return number of new node
        }

        int root(int const x){
            if(parent[x]==-1) return x;
            int const p=parent[x];
            int const r=root(p);
            info[x].compress(info[p]);
            return parent[x]=r;
        }

        Info& operator()(int const x) {return info[root(x)];}
        Info const& operator()(int const x) const {return info[root(x)];}
        Info& operator[](int const x) {return info[x];}
        Info const& operator[](int const x) const{return info[x];}

        bool is_connected(int const x,int const y){ return root(x)==root(y); }

        bool unite(int const x,int const y){
            int rx=root(x);
            int ry=root(y);
            if(rx==ry){
                edge_count[rx]++;
                return false;
            }

            // Union by Size (default)
            if(node_count[rx]<node_count[ry]) swap(rx,ry);
            parent[ry]=rx;
            node_count[rx]+=node_count[ry];
            edge_count[rx]+=edge_count[ry]+1;
            components--;

            // Merge additional information
            info[rx].merge(info[ry]);
            return true;
        }

        vector<int> root_nodes() const{
            vector<int> res;
            res.reserve(components);
            for(int i=0;i<n;i++){ if(parent[i]==-1)res.push_back(i); }
            return res;
        }

        vector<vector<int>> group_nodes(){
            vector<int> const roots=root_nodes();
            int const x=components;
            unordered_map<int,int> mp;
            mp.reserve(x);
            for(int i=0;i<x;i++)mp[roots[i]]=i;
            vector<vector<int>> res(x);
            for(int i=0;i<n;i++){
                int r=root(i);
                res[mp[r]].push_back(i);
            }
            return res;
        }

        int group_node_count(int const x=0){ return node_count[root(x)]; }
        int group_edge_count(int const x=0){ return edge_count[root(x)]; }
        int group_count() const{ return components; }

        vector<int> group_node_counts() const{
            vector<int> res;
            res.reserve(components);
            for(int i=0;i<n;i++){ if(parent[i]==-1)res.push_back(node_count[i]); }
            return res;
        }
};



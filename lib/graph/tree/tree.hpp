#pragma once
#include <bits/stdc++.h>
#include "../tree/tree_hld.hpp"
using namespace std;

template<typename T>
struct child_range{
    tree_hld<T>&tr;
    int v;
    bool include_heavy;

    child_range(tree_hld<T>&tr_,int const v_,bool const include_heavy_)
        : tr(tr_), v(v_), include_heavy(include_heavy_){}

    struct iterator{
        tree_hld<T>&tr;
        int v;
        int idx;
        bool include_heavy;

        iterator(tree_hld<T>&tr_,int const v_,int const idx_,bool const include_heavy_)
            : tr(tr_), v(v_), idx(idx_), include_heavy(include_heavy_){
            skip_nodes();
        }

        void skip_nodes(){
            while(idx<static_cast<int>(tr.gr[v].size())){
                int u=get<0>(tr.gr[v][idx]);
                if(u==tr.parent_[v]||(!include_heavy&&u==tr.heavy_[v]))++idx;
                else break;
            }
        }

        int operator*() const{ return get<0>(tr.gr[v][idx]); }

        iterator&operator++(){
            ++idx;
            skip_nodes();
            return *this;
        }

        bool operator==(iterator const&other) const{
            return &tr==&other.tr&&v==other.v&&idx==other.idx;
        }

        bool operator!=(iterator const&other) const{ return !(*this==other); }
    };

    iterator begin(){
        tr.ensure_hld_built();
        return iterator(tr,v,0,include_heavy);
    }

    iterator end(){
        tr.ensure_hld_built();
        return iterator(tr,v,static_cast<int>(tr.gr[v].size()),include_heavy);
    }
};

template<typename T>
struct node_proxy{
    tree_hld<T>&tr;
    int v;

    node_proxy(tree_hld<T>&tr_,int const v_): tr(tr_), v(v_){}

    int pos(){
        tr.ensure_hld_built();
        return tr.pos_[v];
    }

    int parent(){
        tr.ensure_hld_built();
        return tr.parent_[v];
    }

    vector<int> vertices_at_distance(int const d) const{ // return v:tree_pos()
        vector<int> dist(tr.node_count,-1);
        vector<int> res;
        queue<int> q;
        q.push(v);
        dist[v]=0;
        while(!q.empty()){
            int x=q.front();
            q.pop();
            if(dist[x]==d){
                res.push_back(x);
                continue;
            }
            for(auto [y,w,eid]:tr.gr[x]){
                if(dist[y]!=-1) continue;
                dist[y]=dist[x]+1;
                q.push(y);
            }
        }
        return res;
    }

    vector<int> vertices_at_weight(T const&d) const{ // return v:tree_pos()
        vector<int> res;
        vector<tuple<int,int,T>> st;
        st.emplace_back(v,-1,T(0));
        while(!st.empty()){
            auto [x,p,dist]=st.back();
            st.pop_back();
            if(dist>d) continue;
            if(dist==d) res.push_back(x);
            for(auto [y,w,eid]:tr.gr[x]){
                if(y==p) continue;
                st.emplace_back(y,x,dist+w);
            }
        }
        return res;
    }

    int heavy_child(){ // return v:tree_pos()
        tr.ensure_hld_built();
        return tr.heavy_[v];
    }

    int depth(){
        tr.ensure_hld_built();
        return tr.depth_[v];
    }

    int lca(int const other) const{ return tr.lca(v,other); } // return v:tree_pos()
    int kth_ancestor(int const k) const{ return tr.kth_ancestor(v,k); } // return v:tree_pos()

    int heavy_path_head(){ // return v:tree_pos()
        tr.ensure_hld_built();
        return tr.head_[v];
    }

    vector<int> heavy_path_vertices() const{ // return v:tree_pos()
        tr.ensure_hld_built();
        vector<int> res;
        for(int u=v;u!=-1;u=tr.heavy_[u])res.push_back(u);
        return res;
    }

    child_range<T> child_nodes(bool const include_heavy=true){
        tr.ensure_hld_built();
        return child_range<T>(tr,v,include_heavy);
    }

    bool is_ancestor(int const other){
        tr.ensure_hld_built();
        return tr.pos_[v]<=tr.pos_[other]&&tr.pos_[other]<tr.pos_[v]+tr.subtree_[v];
    }

    bool is_leaf(){
        tr.ensure_hld_built();
        return tr.gr[v].size()==(tr.parent_[v]==-1?0:1);
    }
};

template<typename T>
struct edge_proxy{
    tree_hld<T>&tr;
    int eid;
    edge_proxy(tree_hld<T>&tr_,int const eid_): tr(tr_), eid(eid_){}

    int parent_node(){ // return v:tree_pos()
        tr.ensure_hld_built();
        return tr.parent_[tr.child_edge_[eid]];
    }

    int child_node(){ // return v:tree_pos()
        tr.ensure_hld_built();
        return tr.child_edge_[eid];
    }
};

template<typename T>
struct path_proxy{
    tree_hld<T>&tr;
    int u,v;

    path_proxy(tree_hld<T>&tr_,int const u_,int const v_): tr(tr_), u(u_), v(v_){}

    int lca() const{ return tr.lca(u,v); }
    int node_count() const{ return tr.tree_node_count(u,v); }
    int edge_count() const{ return tr.tree_node_count(u,v)-1; }
    vector<int> vertices() const{ return tr.path_vertices(u,v); }
    int kth_node(int const k) const{ return tr.kth_node(u,v,k); }

    vector<pair<int,int>> node_ranges() const{ // return p:hld_pos[]
        tr.ensure_hld_built();
        vector<pair<int,int>> left,right;
        int a=u,b=v;
        while(tr.head_[a]!=tr.head_[b]){
            if(tr.depth_[tr.head_[a]]>=tr.depth_[tr.head_[b]]){
                left.emplace_back(tr.pos_[tr.head_[a]],tr.pos_[a]);
                a=tr.parent_[tr.head_[a]];
            } else{
                right.emplace_back(tr.pos_[b],tr.pos_[tr.head_[b]]);
                b=tr.parent_[tr.head_[b]];
            }
        }
        if(tr.depth_[a]>=tr.depth_[b])left.emplace_back(tr.pos_[b],tr.pos_[a]);
        else right.emplace_back(tr.pos_[a],tr.pos_[b]);

        reverse(right.begin(),right.end());
        left.insert(left.end(),right.begin(),right.end());
        return left;
    }

    template<class F>
    void node_ranges_query(F&&f) const{ for(auto [l,r]:node_ranges())f(l,r); }

    vector<pair<int,int>> edge_ranges() const{ // return p:hld_pos[]
        tr.ensure_hld_built();
        vector<pair<int,int>> left,right;
        int a=u,b=v;
        while(tr.head_[a]!=tr.head_[b]){
            if(tr.depth_[tr.head_[a]]>=tr.depth_[tr.head_[b]]){
                left.emplace_back(tr.pos_[tr.head_[a]],tr.pos_[a]);
                a=tr.parent_[tr.head_[a]];
            } else{
                right.emplace_back(tr.pos_[b],tr.pos_[tr.head_[b]]);
                b=tr.parent_[tr.head_[b]];
            }
        }
        if(tr.depth_[a]>=tr.depth_[b]){ if(a!=b)left.emplace_back(tr.pos_[b]+1,tr.pos_[a]); }
        else right.emplace_back(tr.pos_[a]+1,tr.pos_[b]);
        reverse(right.begin(),right.end());
        left.insert(left.end(),right.begin(),right.end());
        return left;
    }

    template<class F>
    void edge_ranges_query(F&&f) const{ for(auto [l,r]:edge_ranges())f(l,r); }
};

template<typename T>
struct subtree_proxy{
    tree_hld<T>&tr;
    int v;

    subtree_proxy(tree_hld<T>&tr_,int const v_):tr(tr_), v(v_){}

    int node_count() const{
        tr.ensure_hld_built();
        return tr.subtree_[v];
    }

    // return v:tree_pos()
    vector<int> vertices() const{ return tr.subtree_vertices(v); }

    pair<int,int> node_range() const{ // return p:hld_pos[]
        tr.ensure_hld_built();
        int l=tr.pos_[v];
        int r=l+tr.subtree_[v]-1;
        return {l,r};
    }

    template<class F>
    decltype(auto) node_range_query(F&&f) const{
        auto [l, r]=node_range();
        return f(l,r);
    }

    pair<int,int> edge_range() const{ // return p:hld_pos[]
        tr.ensure_hld_built();
        int l=tr.pos_[v]+1;
        int r=tr.pos_[v]+tr.subtree_[v]-1;
        if(l>r) return {0,-1};
        return {l,r};
    }

    template<class F>
    void edge_range_query(F&&f) const{
        auto [l, r]=edge_range();
        if(l<=r) f(l,r);
    }
};

template<typename T>
node_proxy<T> tree_hld<T>::node(int const v){
    return node_proxy<T>(*this,v);
}

template<typename T>
edge_proxy<T> tree_hld<T>::edge(int const eid){
    return edge_proxy<T>(*this,eid);
}

template<typename T>
path_proxy<T> tree_hld<T>::path(int const u,int const v){
    ensure_hld_built();
    return path_proxy<T>(*this,u,v);
}

template<typename T>
path_proxy<T> tree_hld<T>::path(pair<int,int> const&p){
    return path(p.first,p.second);
}

template<typename T>
subtree_proxy<T> tree_hld<T>::subtree(int const v){
    ensure_hld_built();
    return subtree_proxy<T>(*this,v);
}

/*

    Functions that return ranges on the HLD order return HLD positions, not tree vertex indices.
    [l,r] -> return p:hld_pos[]
*/
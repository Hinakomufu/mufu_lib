#pragma once
#include <bits/stdc++.h>
using namespace std;

template<typename T = long> struct tree_hld;
template<typename T> struct child_range;
template<typename T> struct node_proxy;
template<typename T> struct edge_proxy;
template<typename T> struct path_proxy;
template<typename T> struct subtree_proxy;

template<typename T>
struct tree_hld{
    private:
        int node_count;
        int edge_count;
        vector<int> parent_,depth_;
        vector<int> parent_edge_,child_edge_;
        vector<int> subtree_,heavy_;
        vector<int> head_,pos_,inv_pos_;
        int root_=0;
        int cur_pos=0;
        bool hld_built=false;
        template<typename U> friend struct child_range;
        template<typename U> friend struct node_proxy;
        template<typename U> friend struct edge_proxy;
        template<typename U> friend struct path_proxy;
        template<typename U> friend struct subtree_proxy;

        void build_hld(){
            cur_pos=0;
            depth_[root_]=0;
            dfs_size(root_,-1);
            dfs_hld(root_,root_);
            hld_built=true;
        }

        void ensure_hld_built(){ if(!hld_built) build_hld(); }

        int dfs_size(int const root,int const p){
            vector<int> order;
            order.reserve(node_count);
            vector<int> st;
            st.reserve(node_count);
            vector<bool> visited(node_count,false);
            st.push_back(root);
            visited[root]=true;
            parent_[root]=p;
            subtree_.assign(node_count,1);
            fill(heavy_.begin(),heavy_.end(),-1);
            while(!st.empty()){
                int v=st.back();
                st.pop_back();
                order.push_back(v);
                for(auto [u,w,eid]:gr[v]){
                    if(visited[u]) continue;
                    visited[u]=true;
                    parent_[u]=v;
                    parent_edge_[u]=eid;
                    child_edge_[eid]=u;
                    depth_[u]=depth_[v]+1;
                    st.push_back(u);
                }
            }
            for(int i=static_cast<int>(order.size())-1;i>0;--i){
                int const v=order[i];
                int const par=parent_[v];
                subtree_[par]+=subtree_[v];
                if(heavy_[par]==-1||subtree_[v]>subtree_[heavy_[par]])heavy_[par]=v;
            }
            return subtree_[root];
        }

        void dfs_hld(int const root,int const h){
            vector<pair<int,int>> st;
            st.reserve(node_count);
            st.emplace_back(root,h);

            while(!st.empty()){
                auto [start, hd]=st.back();
                st.pop_back();
                for(int v=start;v!=-1;v=heavy_[v]){
                    head_[v]=hd;
                    inv_pos_[cur_pos]=v;
                    pos_[v]=cur_pos++;
                    for(auto it=gr[v].rbegin();it!=gr[v].rend();++it){
                        auto [u, w, eid]=*it;
                        if(u==parent_[v]||u==heavy_[v])continue;
                        st.emplace_back(u,u);
                    }
                }
            }
        }

        int tree_node_count(int const u,int const v){
            int const w=lca(u,v);
            return depth_[u]+depth_[v]-2*depth_[w]+1;
        }

        int kth_ancestor(int v,int k){
            ensure_hld_built();
            while(v!=-1){
                int const h=head_[v];
                int const d=depth_[v]-depth_[h];
                if(k<=d)return inv_pos_[pos_[v]-k];
                k-=d+1;
                v=parent_[h];
            }
            return -1;
        }

        int kth_node(int const u,int const v,int const k){
            if(k<0) return -1;
            ensure_hld_built();
            int const w=lca(u,v);
            int const d1=depth_[u]-depth_[w];
            int const d2=depth_[v]-depth_[w];
            if(k<0||k>d1+d2) return -1;
            if(k<=d1)return kth_ancestor(u,k);
            return kth_ancestor(v,d1+d2-k);
        }

        vector<int> path_vertices(int const u,int const v){
            ensure_hld_built();
            vector<int> left,right;
            int a=u,b=v;
            while(head_[a]!=head_[b]){
                if(depth_[head_[a]]>=depth_[head_[b]]){
                    for(int p=pos_[a];p>=pos_[head_[a]];--p) left.push_back(inv_pos_[p]);
                    a=parent_[head_[a]];
                } else{
                    for(int p=pos_[b];p>=pos_[head_[b]];--p) right.push_back(inv_pos_[p]);
                    b=parent_[head_[b]];
                }
            }
            if(depth_[a]>=depth_[b]){
                for(int p=pos_[a];p>=pos_[b];--p) left.push_back(inv_pos_[p]);
            } else{
                for(int p=pos_[b];p>pos_[a];--p) right.push_back(inv_pos_[p]);
                left.push_back(a);
            }
            reverse(right.begin(),right.end());
            left.insert(left.end(),right.begin(),right.end());
            return left;
        }

        int lca(int u,int v){
            ensure_hld_built();
            while(head_[u]!=head_[v]){
                if(depth_[head_[u]]<depth_[head_[v]]) swap(u,v);
                u=parent_[head_[u]];
            }
            return depth_[u]<depth_[v]?u:v;
        }

        vector<int> subtree_vertices(int const v){
            ensure_hld_built();
            vector<int> res;
            res.reserve(subtree_[v]);
            for(int p=pos_[v];p<pos_[v]+subtree_[v];++p){ res.push_back(inv_pos_[p]); }
            return res;
        }

    public:
        vector<vector<tuple<int,T,int>>> gr;

        tree_hld() : node_count(0), edge_count(0){}

        explicit tree_hld(int const n,int const root=0):
            node_count(n), edge_count(0), parent_(n,-1), depth_(n,0),
            parent_edge_(n,-1), child_edge_(), subtree_(n,0), heavy_(n,-1), head_(n,-1),
            pos_(n,-1), inv_pos_(n,-1), root_(root), gr(n){}

        explicit tree_hld(vector<int> const&pos): tree_hld(static_cast<int>(pos.size())){
            for(int i=0;i<static_cast<int>(pos.size());++i){
                if(pos[i]==-1) root_=i;
                else add_edge(i,pos[i]);
            }
        }

        void initialize(int n,int const root=0){
            node_count=n;
            edge_count=0;
            root_=root;

            parent_.assign(n,-1);
            depth_.assign(n,0);
            parent_edge_.assign(n,-1);
            child_edge_.clear();
            subtree_.assign(n,0);
            heavy_.assign(n,-1);
            head_.assign(n,-1);
            pos_.assign(n,-1);
            inv_pos_.assign(n,-1);

            cur_pos=0;
            hld_built=false;

            gr.assign(n,{});
        }

        // p: hld_pos[]
        // v: tree_pos()
        int hld_pos(int const v){ // v->p
            ensure_hld_built();
            return pos_[v];
        }

        int tree_pos(int const p){ // p->v
            ensure_hld_built();
            return inv_pos_[p];
        }

        int operator[](int const v){ return hld_pos(v); }  // v->p
        int operator()(int const p){ return tree_pos(p); } // p->v

        void add_edge(int u,int v,T w=1,bool const undirected=true){
            gr[u].emplace_back(v,w,edge_count);
            if(undirected)gr[v].emplace_back(u,w,edge_count);
            child_edge_.push_back(-1);
            edge_count++;
            hld_built=false;
        }

        pair<int,int> diameter_distance() const{
            auto farthest=[&](int const start){
                vector<int> dist(node_count,-1);
                vector<int> st;
                st.reserve(node_count);
                st.push_back(start);
                dist[start]=0;
                int far=start;
                while(!st.empty()){
                    int v=st.back();
                    st.pop_back();
                    if(dist[v]>dist[far]){ far=v; }
                    for(auto [u, w, eid]:gr[v]){
                        if(dist[u]!=-1) continue;
                        dist[u]=dist[v]+1;
                        st.push_back(u);
                    }
                }
                return far;
            };
            int u=farthest(0);
            int v=farthest(u);
            return {u,v};
        }

        pair<int,int> diameter_weight() const{
            auto farthest=[&](int const start){
                vector<T> dist(node_count,-1);
                vector<int> st;
                st.reserve(node_count);
                st.push_back(start);
                dist[start]=0;
                int far=start;
                while(!st.empty()){
                    int v=st.back();
                    st.pop_back();
                    if(dist[v]>dist[far]){ far=v; }
                    for(auto [u, w, eid]:gr[v]){
                        if(dist[u]!=-1) continue;
                        dist[u]=dist[v]+w;
                        st.push_back(u);
                    }
                }
                return far;
            };
            int u=farthest(0);
            int v=farthest(u);
            return {u,v};
        }

        node_proxy<T> node(int const v);

        edge_proxy<T> edge(int const eid);

        path_proxy<T> path(int const u,int const v);

        path_proxy<T> path(pair<int,int> const&p);

        subtree_proxy<T> subtree(int const v);

        template<typename U,typename M>
        friend struct tree_segtree;
};

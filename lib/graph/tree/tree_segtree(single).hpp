#pragma once
#include <bits/stdc++.h>
#include "../../segtree/normal/single.hpp"
#include "../tree/tree.hpp"
using namespace std;

template<typename U>
struct tree_segtree_monoid{
    using T=U;
    static T f(const T&a,const T&b){ return a+b; }
    static T e(){ return T(0); }
};

template<typename T,typename M=tree_segtree_monoid<T>>
struct tree_segtree{
    private:
        using Seg=segtree<M>;
        vector<T> initial_node;
        Seg node_seg,edge_seg;
        bool has_initial_node=false;
        bool node_seg_built=false;
        bool edge_seg_built=false;

        void build_node(vector<T> const&a){
            tree.ensure_hld_built();
            vector<T> b(tree.node_count,M::e());
            for(int v=0;v<tree.node_count;v++){ b[tree.pos_[v]]=a[v]; }
            node_seg=Seg(b);
            node_seg_built=true;
        }

        void build_node(){
            if(has_initial_node) build_node(initial_node);
            else build_node(vector<T>(tree.node_count,M::e()));
        }

        void build_edge(){
            tree.ensure_hld_built();
            int m=0;
            for(auto const&g:tree.gr){
                for(auto [u,w,eid]:g)m=max(m,eid+1);
            }
            vector<T> b(tree.node_count,M::e());
            vector<T> edge_value(m,M::e());
            for(int v=0;v<tree.node_count;v++){
                for(auto [u,w,eid]:tree.gr[v])edge_value[eid]=w;
            }

            for(int v=0;v<tree.node_count;v++){
                if(tree.parent_edge_[v]!=-1)b[tree.pos_[v]]=edge_value[tree.parent_edge_[v]];
            }
            edge_seg=Seg(b);
            edge_seg_built=true;
        }

        void ensure_node(){ if(!node_seg_built)build_node(); }
        void ensure_edge(){ if(!edge_seg_built)build_edge(); }

        struct node_segtree_proxy:node_proxy<T>{
            tree_segtree&st;
            int v;

            operator T() const{
                st.ensure_node();
                return st.node_seg(st.tree.pos_[v]);
            }

            struct update_proxy{
                tree_segtree&st;
                int v;
                update_proxy(tree_segtree&st_,int const v_): st(st_), v(v_){}

                update_proxy&operator=(T const&x){
                    st.ensure_node();
                    st.node_seg.update(st.tree.pos_[v])=x;
                    return *this;
                }

                update_proxy&operator+=(T const&x){
                    st.ensure_node();
                    st.node_seg.update(st.tree.pos_[v])+=x;
                    return *this;
                }

                update_proxy&operator-=(T const&x){ return *this+=-x; }
            };

            update_proxy update;
            node_segtree_proxy(tree_segtree&st_,int v_): node_proxy<T>(st_.tree,v_), st(st_), v(v_), update(st_,v_){}
        };

        struct edge_segtree_proxy:edge_proxy<T>{
            tree_segtree&st;
            int eid;

            operator T() const{
                st.ensure_edge();
                int v=st.tree.child_edge_[eid];
                return st.edge_seg(st.tree.pos_[v]);
            }

            struct update_proxy{
                tree_segtree&st;
                int eid;
                update_proxy(tree_segtree&st_,int const eid_): st(st_), eid(eid_){}

                update_proxy&operator=(T const&x){
                    st.ensure_edge();
                    int v=st.tree.child_edge_[eid];
                    st.edge_seg.update(st.tree.pos_[v])=x;
                    return *this;
                }

                update_proxy&operator+=(T const&x){
                    st.ensure_edge();
                    int v=st.tree.child_edge_[eid];
                    st.edge_seg.update(st.tree.pos_[v])+=x;
                    return *this;
                }

                update_proxy&operator-=(T const&x){ return *this+=-x; }
            };

            update_proxy update;

            edge_segtree_proxy(tree_segtree&st_,int eid_): edge_proxy<T>(st_.tree,eid_), st(st_), eid(eid_),
                                                           update(st_,eid_){}
        };

        struct path_segtree_proxy:path_proxy<T>{
            tree_segtree&st;
            int u,v;
            path_segtree_proxy(tree_segtree&st_,int u_,int v_): path_proxy<T>(st_.tree,u_,v_), st(st_), u(u_), v(v_){}

            T node_sum() const{ return st.path_node_query(u,v); }
            T edge_sum() const{ return st.path_edge_query(u,v); }
        };

        struct subtree_segtree_proxy:subtree_proxy<T>{
            tree_segtree&st;
            int v;
            subtree_segtree_proxy(tree_segtree&st_,int v_): subtree_proxy<T>(st_.tree,v_), st(st_), v(v_){}
            T node_sum() const{ return st.subtree_node_query(v); }
            T edge_sum() const{ return st.subtree_edge_query(v); }
        };

        T path_node_query(int u,int v){
            ensure_node();
            T ans=M::e();
            while(tree.head_[u]!=tree.head_[v]){
                if(tree.depth_[tree.head_[u]]<tree.depth_[tree.head_[v]])swap(u,v);
                ans=M::f(ans,node_seg(tree.pos_[tree.head_[u]],tree.pos_[u]));
                u=tree.parent_[tree.head_[u]];
            }
            if(tree.depth_[u]>tree.depth_[v])swap(u,v);
            ans=M::f(ans,node_seg(tree.pos_[u],tree.pos_[v]));
            return ans;
        }

        T path_edge_query(int u,int v){
            ensure_edge();
            T ans=M::e();
            while(tree.head_[u]!=tree.head_[v]){
                if(tree.depth_[tree.head_[u]]<tree.depth_[tree.head_[v]])swap(u,v);
                ans=M::f(ans,edge_seg(tree.pos_[tree.head_[u]],tree.pos_[u]));
                u=tree.parent_[tree.head_[u]];
            }
            if(tree.depth_[u]>tree.depth_[v])swap(u,v);
            ans=M::f(ans,edge_seg(tree.pos_[u]+1,tree.pos_[v]));
            return ans;
        }

        T subtree_node_query(int v){
            ensure_node();
            return node_seg(tree.pos_[v],tree.pos_[v]+tree.subtree_[v]-1);
        }

        T subtree_edge_query(int v){
            ensure_edge();
            if(tree.subtree_[v]<=1)return M::e();
            return edge_seg(tree.pos_[v]+1,tree.pos_[v]+tree.subtree_[v]-1);
        }

    public:
        tree_hld<T> tree;
        explicit tree_segtree(int n): tree(n){}

        tree_segtree(int n,vector<T> const&a): initial_node(a), has_initial_node(true), tree(n){
            assert((int)a.size()==n);
        }

        explicit tree_segtree(vector<int> const&parents): tree(parents){}

        void add_edge(int u,int v,T w=1){
            assert(!tree.hld_built);
            assert(!node_seg_built&&!edge_seg_built);
            tree.add_edge(u,v,w);
        }

        node_segtree_proxy node(int v){ return node_segtree_proxy(*this,v); }
        edge_segtree_proxy edge(int eid){ return edge_segtree_proxy(*this,eid); }

        path_segtree_proxy path(int u,int v){
            tree.ensure_hld_built();
            return path_segtree_proxy(*this,u,v);
        }

        path_segtree_proxy path(pair<int,int> const&p){
            return path(p.first,p.second);
        }

        subtree_segtree_proxy subtree(int v){
            tree.ensure_hld_built();
            return subtree_segtree_proxy(*this,v);
        }
};

#pragma once
#include <bits/stdc++.h>
using namespace std;

struct graph{
    int node_count;
    vector<vector<int>> next;
    vector<vector<int>> prev;
    int edge_count;
    vector<pair<int,int>> edges;
    vector<vector<int>> edge_id;

    explicit graph(int const n): node_count(n), next(n), prev(n), edge_count(0), edge_id(n){}

    void add_edge(int const u,int const v,bool const undirected=true){
        int const id=edge_count++;
        edges.emplace_back(u,v);
        next[u].emplace_back(v);
        prev[v].emplace_back(u);
        edge_id[u].emplace_back(id);
        if(undirected){
            next[v].emplace_back(u);
            prev[u].emplace_back(v);
            edge_id[v].emplace_back(id);
        }
    }

    void add_direct_edge(int const u,int const v){ add_edge(u,v,false); }
    int degree(int const n) const{ return next[n].size(); }

    int bfs(int const start,vector<int>&walk,vector<int>*parent=nullptr) const{
        if(start<0||start>=node_count) return -1;
        if(walk.size()!=node_count) return -1;
        if(walk[start]!=-1) return -1;
        if(parent!=nullptr){ if(parent->size()!=node_count)parent->assign(node_count,-1); }
        int res=0;
        queue<int> que;
        que.push(start);
        walk[start]=0;
        while(!que.empty()){
            int const u=que.front();
            que.pop();
            for(int const v:next[u]){
                if(walk[v]==-1){
                    walk[v]=walk[u]+1;
                    if(parent!=nullptr)(*parent)[v]=u;
                    res=max(res,walk[v]);
                    que.push(v);
                }
            }
        }
        return res;
    }

    void bfs_all(vector<int>&walk,vector<int>*parent=nullptr) const{
        if(walk.size()!=node_count)walk.assign(node_count,-1);
        if(parent!=nullptr)parent->assign(node_count,-1);
        for(int start=0;start<node_count;++start)bfs(start,walk,parent);
    }

    int dfs(int const start,vector<int>&walk,vector<int>*parent=nullptr) const{
        if(start<0||start>=node_count) return -1;
        if(walk.size()!=node_count) return -1;
        if(walk[start]!=-1) return -1;
        if(parent!=nullptr&&parent->size()!=node_count)parent->assign(node_count,-1);
        int res=0;
        vector<int> st;
        st.push_back(start);
        walk[start]=0;
        while(!st.empty()){
            int const u=st.back();
            st.pop_back();
            for(int const v:next[u]){
                if(walk[v]==-1){
                    walk[v]=walk[u]+1;
                    if(parent!=nullptr)(*parent)[v]=u;
                    res=max(res,walk[v]);
                    st.push_back(v);
                }
            }
        }
        return res;
    }

    void dfs_all(vector<int>&walk,vector<int>*parent=nullptr) const{
        if(walk.size()!=node_count)walk.assign(node_count,-1);
        if(parent!=nullptr)parent->assign(node_count,-1);
        for(int start=0;start<node_count;++start)dfs(start,walk,parent);
    }

    int walk_count(vector<int> const&walk) const{
        int count=0;
        for(int i=0;i<node_count;++i)if(walk[i]!=-1) count++;
        return count;
    }

    static vector<int> restore_path(int const start,int const goal,vector<int> const&parent){
        int const ps=parent.size();
        if(start<0||start>=ps) return {};
        if(goal<0||goal>=ps) return {};
        vector<int> path;
        int cur=goal;
        for(int i=0;i<ps&&cur!=-1;++i){
            path.emplace_back(cur);
            if(cur==start){
                reverse(path.begin(),path.end());
                return path;
            }
            cur=parent[cur];
        }
        return {};
    }

    vector<int> cycle_edges() const{
        vector<int> state(node_count,0);
        vector<int> parent(node_count,-1);
        vector<int> parent_edge(node_count,-1);
        for(int start=0;start<node_count;++start){
            if(state[start]!=0) continue;
            vector<pair<int,int>> st;
            state[start]=1;
            st.emplace_back(start,0);
            while(!st.empty()){
                int const u=st.back().first;
                int&i=st.back().second;
                if(i==static_cast<int>(next[u].size())){
                    state[u]=2;
                    st.pop_back();
                    continue;
                }
                int const v=next[u][i];
                int const eid=edge_id[u][i];
                ++i;
                if(eid==parent_edge[u]) continue;
                if(state[v]==0){
                    state[v]=1;
                    parent[v]=u;
                    parent_edge[v]=eid;
                    st.emplace_back(v,0);
                } else if(state[v]==1){
                    vector<int> res;
                    int cur=u;
                    while(cur!=v){
                        res.emplace_back(parent_edge[cur]);
                        cur=parent[cur];
                    }
                    reverse(res.begin(),res.end());
                    res.emplace_back(eid);
                    return res;
                }
            }
        }
        return {};
    }

    vector<int> restore_nodes_from_edges(vector<int> const&edges_vector) const{
        int const sz=edges_vector.size();
        if(sz==0) return {};
        for(int const eid:edges_vector){ if(eid<0||eid>=edge_count)return {}; }
        auto [a,b]=edges[edges_vector[0]];
        if(sz==1){
            if(a==b) return {a,a};
            return {};
        }
        auto [c,d]=edges[edges_vector[1]];
        int start,cur;
        if(c==a||d==a){
            start=b;
            cur=a;
        } else if(c==b||d==b){
            start=a;
            cur=b;
        } else return {};
        vector<int> res;
        res.emplace_back(start);
        res.emplace_back(cur);
        for(int i=1;i<sz;++i){
            auto [u,v]=edges[edges_vector[i]];
            int nxt;
            if(u==cur) nxt=v;
            else if(v==cur) nxt=u;
            else return {};
            res.emplace_back(nxt);
            cur=nxt;
        }
        if(cur!=start) return {};
        return res;
    }

    vector<int> restore_edges_from_nodes(vector<int> const&nodes_vector) const{
        int const sz=nodes_vector.size();
        if(sz<2) return {};
        vector<int> res;
        res.reserve(sz-1);
        for(int i=0;i<sz-1;++i){
            int const u=nodes_vector[i];
            int const v=nodes_vector[i+1];
            if(u<0||u>=node_count) return {};
            if(v<0||v>=node_count) return {};
            int eid=-1;
            for(int j=0;j<static_cast<int>(next[u].size());++j){
                if(next[u][j]==v){
                    eid=edge_id[u][j];
                    break;
                }
            }
            if(eid==-1) return {};
            res.emplace_back(eid);
        }
        return res;
    }

    void debug() const{
        for(int i=0;i<node_count;i++){
            for(int const v:next[i])cout<<v<<" ";
            cout<<endl;
        }
    }
};

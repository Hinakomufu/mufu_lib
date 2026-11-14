#pragma once
#include <bits/stdc++.h>
using namespace std;

struct graph{
    int node;
    vector<vector<pair<int,long>>> next;
    vector<vector<pair<int,long>>> prev;

    explicit graph(int const n): node(n), next(n), prev(n){}

    void add_edge(int u,int v,long w,bool const undirected=true){
        next[u].emplace_back(v,w);
        prev[v].emplace_back(u,w);
        if(undirected){
            next[v].emplace_back(u,w);
            prev[u].emplace_back(v,w);
        }
    }

    void add_direct_edge(int const u,int const v,long const w){add_edge(u,v,w,false);}
    int edge_count(int const n) const{return next[n].size();}

    vector<long> dijkstra_all(int start,vector<int>*parent=nullptr){
        vector<long> dist(node,1e18);
        dist[start]=0;
        if(parent!=nullptr)parent->assign(node,-1);
        priority_queue<pair<long,int>,vector<pair<long,int>>,greater<>> pq;
        pq.push({0,start});

        while(!pq.empty()){
            auto [d,u]=pq.top();
            pq.pop();
            if(d>dist[u]) continue;
            for(auto [v,w]:next[u]){
                if(dist[v]>dist[u]+w){
                    dist[v]=dist[u]+w;
                    if(parent!=nullptr)(*parent)[v]=u;
                    pq.push({dist[v],v});
                }
            }
        }
        return dist;
    }

    long dijkstra_one(int start,int const goal,vector<int>*parent=nullptr){
        vector<long> dist(node,1e18);
        dist[start]=0;
        if(parent!=nullptr) parent->assign(node,-1);
        priority_queue<pair<long,int>,vector<pair<long,int>>,greater<>> pq;
        pq.push({0,start});
        while(!pq.empty()){
            auto [d, u]=pq.top();
            pq.pop();
            if(d>dist[u]) continue;
            if(u==goal) return d;
            for(auto [v, w]:next[u]){
                if(dist[v]>dist[u]+w){
                    dist[v]=dist[u]+w;
                    if(parent!=nullptr)(*parent)[v]=u;
                    pq.push({dist[v],v});
                }
            }
        }
        return 1e18;
    }

    vector<long> bfs01(int const start,vector<int>*parent=nullptr) const{
        vector<long> dist(node,1e18);
        dist[start]=0;
        if(parent!=nullptr) parent->assign(node,-1);
        deque<int> dq;
        dq.push_front(start);
        while(!dq.empty()){
            int const u=dq.front();
            dq.pop_front();
            for(auto [v,w]:next[u]){
                if(dist[v]>dist[u]+w){
                    dist[v]=dist[u]+w;
                    if(parent!=nullptr)(*parent)[v]=u;
                    if(w==0)dq.push_front(v);
                    else dq.push_back(v);
                }
            }
        }
        return dist;
    }

    static vector<int> restore_path(int const start,int const goal,const vector<int>&parent){
        int const ps=parent.size();
        if(start<0||start>=ps) return {};
        if(goal<0||goal>=ps) return {};
        vector<int> path;
        int cur=goal;
        while(cur!=-1){
            path.push_back(cur);
            if(cur==start) break;
            cur=parent[cur];
        }
        if(path.back()!=start) return {};
        reverse(path.begin(),path.end());
        return path;
    }
};



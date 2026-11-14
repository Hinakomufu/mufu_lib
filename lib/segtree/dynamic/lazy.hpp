#pragma once
#include <bits/stdc++.h>
using namespace std;

template<typename M>
struct segtree{ // dynamic, lazy operation
    public:
        using T=typename M::T;
        using F=typename M::F;

    private:
        struct node{
            T val;
            F lazy;
            node*l=nullptr;
            node*r=nullptr;
            node(long len): val{0,len}, lazy(M::id()){}
        };

        long len;
        node*root=nullptr;

        T value(node*t,long const l,long const r){
            if(t) return t->val;
            return T{0,r-l};
        }

        void apply(node*t,F const&f){
            t->val=M::ap(f,t->val);
            t->lazy=M::cmp(t->lazy,f);
        }

        void push(node*t,long const l,long const r){
            if(!t||r-l==1) return;

            long const mid=l+(r-l)/2;

            if(!t->l) t->l=new node(mid-l);
            if(!t->r) t->r=new node(r-mid);

            if(t->lazy!=M::id()){
                apply(t->l,t->lazy);
                apply(t->r,t->lazy);
                t->lazy=M::id();
            }
        }

        void pull(node*t,long const l,long const r){
            long const mid=l+(r-l)/2;
            t->val=M::f(
                value(t->l,l,mid),
                value(t->r,mid,r)
            );
        }

        void range_apply(node*&t,long const l,long const r,long const a,long const b,F const&f){
            if(r<=a||b<=l) return;
            if(!t) t=new node(r-l);
            if(a<=l&&r<=b){
                apply(t,f);
                return;
            }
            push(t,l,r);
            long const mid=l+(r-l)/2;
            range_apply(t->l,l,mid,a,b,f);
            range_apply(t->r,mid,r,a,b,f);
            pull(t,l,r);
        }

        T calc(node*t,long const l,long const r,long const a,long const b,F const&carry) const{
            if(r<=a||b<=l) return M::e();
            if(a<=l&&r<=b){
                T v=t?t->val:T{0,r-l};
                return M::ap(carry,v);
            }
            F next=carry;
            if(t) next=M::cmp(t->lazy,carry);
            long const mid=l+(r-l)/2;
            return M::f(
                calc(t?t->l:nullptr,l,mid,a,b,next),
                calc(t?t->r:nullptr,mid,r,a,b,next)
            );
        }

        struct update_proxy{
            segtree&st;
            long l,r;
            update_proxy(segtree&st_,long const l_,long const r_): st(st_), l(l_), r(r_){}

            template<class...Args>
            update_proxy&apply(Args&&...args){
                st.range_apply(st.root,0,st.len,l,r,F{std::forward<Args>(args)...});
                return *this;
            }

            template<class...Args>
            update_proxy&operator()(Args&&...args){
                return apply(std::forward<Args>(args)...);
            }
        };

        void debug(node*t,long const l,long const r,F const&carry,function<string(const T&)> proj) const{
            if(!t) return;
            if(r-l==1){
                T v=M::ap(carry,t->val);
                cout<<l<<" "<<proj(v)<<"\n";
                return;
            }
            F next=M::cmp(t->lazy,carry);
            long const mid=l+(r-l)/2;
            debug(t->l,l,mid,next,proj);
            debug(t->r,mid,r,next,proj);
        }

    public:
        explicit segtree(long const n) : len(n){}

        update_proxy update_range(long l,long const r){
            return update_proxy(*this,l,r+1);
        }

        T calc(long const l,long const r) const{
            if(l>r) return M::e();
            return calc(root,0,len,l,r+1,M::id());
        }

        T get(long const p) const{
            return calc(p,p);
        }

        T operator()(long const p) const{
            return get(p);
        }

        T operator()(long const l,long const r) const{
            return calc(l,r);
        }

        T all() const{
            if(len==0) return M::e();
            return calc(0,len-1);
        }

        T all_except(long const pos) const{
            if(len==0) return M::e();
            T left=pos>0?calc(0,pos-1):M::e();
            T right=pos<len-1?calc(pos+1,len-1):M::e();
            return M::f(left,right);
        }

        void debug(function<string(const T&)> proj) const{
            if(len==0) return;
            debug(root,0,len,M::id(),proj);
        }
};

/*  template


*/



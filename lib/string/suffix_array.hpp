#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class T>
struct suffix_array{
    private:
        T s_;                  // original string/vector
        vector<int> position_; // suffix array
        vector<int> rank_;     // inverse suffix array
        vector<int> next_lcp_; // adjacent suffix LCP
        int n=0;               // length

        vector<int> calc_suffix_array(const T&s) const{
            if(s.empty()) return {};
            int const n=s.size();
            if constexpr(is_same_v<T,string>){ // string
                int first=256,last=-1;
                for(unsigned char const c:s){
                    first=min(first,static_cast<int>(c));
                    last=max(last,static_cast<int>(c));
                }
                vector<int> vec(n+1);
                for(int i=0;i<n;i++)vec[i]=static_cast<unsigned char>(s[i])-first+1;
                vec.back()=0;
                auto ret=SA_IS(vec,last-first+2);
                ret.erase(ret.begin());
                return ret;
            } else{ // vector<int>
                vector<int> ss=s;
                sort(ss.begin(),ss.end());
                ss.erase(unique(ss.begin(),ss.end()),ss.end());
                vector<int> vec(n+1);
                copy(begin(s),end(s),begin(vec));
                for(int i=0;i<n;i++)vec[i]=lower_bound(ss.begin(),ss.end(),s[i])-ss.begin()+1;
                vec.back()=0;
                auto ret=SA_IS(vec,*max_element(vec.begin(),vec.end())+2);
                ret.erase(ret.begin());
                return ret;
            }
        }

        void induced_sort(const vector<int>&vect,int const val_range,
                          vector<int>&SA,const vector<bool>&sl,
                          const vector<int>&lms_idx) const{
            std::vector<int> l(val_range,0),r(val_range,0);
            for(int const c:vect){
                if(c+1<val_range) l[c+1]++;
                r[c]++;
            }
            partial_sum(l.begin(),l.end(),l.begin());
            partial_sum(r.begin(),r.end(),r.begin());
            fill(SA.begin(),SA.end(),-1);
            for(int i=static_cast<int>(lms_idx.size())-1;i>=0;i--)
                SA[--r[vect[lms_idx[i]]]]=lms_idx[i];
            for(int const i:SA){ if(i>=1&&sl[i-1]) SA[l[vect[i-1]]++]=i-1; }
            fill(r.begin(),r.end(),0);
            for(int const c:vect) ++r[c];
            partial_sum(r.begin(),r.end(),r.begin());
            for(int k=static_cast<int>(SA.size())-1,i=SA[k];k>=1;--k,i=SA[k])
                if(i>=1&&!sl[i-1]){ SA[--r[vect[i-1]]]=i-1; }
        }

        vector<int> SA_IS(const vector<int>&vec,int const val_range) const{
            const int n=vec.size();
            vector<int> sa(n),lms_idx;
            vector<bool> sl(n);
            sl[n-1]=false;
            for(int i=n-2;i>=0;i--){
                sl[i]=(vec[i]>vec[i+1]||(vec[i]==vec[i+1]&&sl[i+1]));
                if(sl[i]&&!sl[i+1]) lms_idx.push_back(i+1);
            }
            reverse(lms_idx.begin(),lms_idx.end());
            induced_sort(vec,val_range,sa,sl,lms_idx);
            vector<int> new_lms_idx(lms_idx.size()),lms_vec(lms_idx.size());
            for(int i=0,k=0;i<n;i++)
                if(!sl[sa[i]]&&sa[i]>=1&&sl[sa[i]-1]){
                    new_lms_idx[k++]=sa[i];
                }
            int cur=0;
            sa[n-1]=cur;
            for(size_t k=1;k<new_lms_idx.size();k++){
                int const i=new_lms_idx[k-1];
                int const j=new_lms_idx[k];
                if(vec[i]!=vec[j]){
                    sa[j]=++cur;
                    continue;
                }
                bool flag=false;
                for(int a=i+1,b=j+1;;a++,b++){
                    if(vec[a]!=vec[b]){
                        flag=true;
                        break;
                    }
                    if((!sl[a]&&sl[a-1])||(!sl[b]&&sl[b-1])){
                        flag=!((!sl[a]&&sl[a-1])&&(!sl[b]&&sl[b-1]));
                        break;
                    }
                }
                sa[j]=(flag?++cur:cur);
            }
            for(size_t i=0;i<lms_idx.size();++i) lms_vec[i]=sa[lms_idx[i]];
            if(cur+1<static_cast<int>(lms_idx.size())){
                auto const lms_sa=SA_IS(lms_vec,cur+1);
                for(size_t i=0;i<lms_idx.size();++i)new_lms_idx[i]=lms_idx[lms_sa[i]];
            }
            induced_sort(vec,val_range,sa,sl,new_lms_idx);
            return sa;
        }

        void calc_lcp(const T&s){
            int const n=s.size();
            int k=0;
            rank_.resize(n);
            next_lcp_.resize(n);
            for(int i=0;i<n;i++) rank_[position_[i]]=i;
            for(int i=0;i<n;i++,k?k--:0){
                if(rank_[i]==n-1){
                    k=0;
                    continue;
                }
                int const j=position_[rank_[i]+1];
                while(i+k<n&&j+k<n&&s[i+k]==s[j+k]) k++;
                next_lcp_[rank_[i]]=k;
            }
            if(n>0) next_lcp_.resize(n-1);
        }

    public:
        suffix_array(const T&s): s_(s){
            n=s.size();
            position_=calc_suffix_array(s);
            calc_lcp(s);
        }

        const vector<int>&position() const{ return position_; }
        const vector<int>&rank() const{ return rank_; }
        const vector<int>&next_lcp() const{ return next_lcp_; }
        const T& str() const {return s_;}
        int position(int const i) const{ return position_[i]; }
        int rank(int const i) const{ return rank_[i]; }
        int next_lcp(int const i) const{ return next_lcp_[i]; }
        int size() const{ return n; }

        long number_of_substrings() const{
            long num=static_cast<long>(n)*(n+1)/2;
            for(long const lcp:next_lcp_)num-=lcp;
            return num;
        }

        T longest_repeated_substring() const{
            if(next_lcp_.empty()) return T{};
            int const i=max_element(next_lcp_.begin(),next_lcp_.end())-next_lcp_.begin();
            return T(s_.begin()+position_[i],s_.begin()+position_[i]+next_lcp_[i]);
        }

        bool contains(const string&p) const{
            int const sp=p.size();
            int l=0,r=n;
            while(l<r){
                int const mid=(l+r)/2;
                int const pos=position_[mid];
                int i=0;
                while(i<sp&&pos+i<n&&s_[pos+i]==p[i]) i++;
                if(i==sp) return true;
                if(pos+i==n||s_[pos+i]<p[i]) l=mid+1;
                else r=mid;
            }
            return false;
        }

        // number of distinct substrings added by the i-th suffix in SA order
        int new_substrings_count(int const i) const{
            int const lcp=(i==0?0:next_lcp_[i-1]);
            return n-position_[i]-lcp;
        }

        vector<int> new_substrings_count() const{
            vector<int> ret(n);
            for(int i=0;i<n;i++) ret[i]=new_substrings_count(i);
            return ret;
        }
};



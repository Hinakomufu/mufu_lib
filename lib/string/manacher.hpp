#pragma once
#include <bits/stdc++.h>
using namespace std;

struct manacher{
    private:
        string s;
        vector<int> odd;
        vector<int> even;
        /*
         0 1 2 3 4  <- odd pos
         a b c d e
        0 1 2 3 4 5 <- even pos
        */
    public:
        manacher(const string&s) : s(s){
            int const n=s.size();
            odd.resize(n);
            even.resize(n+1);
            for(int i=0,l=0,r=-1;i<n;i++){
                int k=(i>r?1:min(odd[l+r-i],r-i+1));
                while(i-k>=0&&i+k<n&&s[i-k]==s[i+k]) k++;
                odd[i]=k;
                if(i+k-1>r){
                    l=i-k+1;
                    r=i+k-1;
                }
            }

            for(int i=0,l=0,r=-1;i<=n;i++){
                int k=(i>r?0:min(even[l+r-i+1],r-i+1));
                while(i-k-1>=0&&i+k<n&&s[i-k-1]==s[i+k]) k++;
                even[i]=k;
                if(i+k-1>r){
                    l=i-k;
                    r=i+k-1;
                }
            }
        }

        int odd_radius(int const c) const{ return odd[c]; }
        int even_radius(int const c) const{ return even[c]; }
        int odd_diameter(int const c) const{ return 2*odd[c]-1; }
        int even_diameter(int const c) const{ return 2*even[c]; }

        bool is_palindrome(int const l,int const r) const{ // O(1)
            if(l>r)return is_palindrome(r,l);
            int const len=r-l+1;
            if(len%2==1){
                int const c=(l+r)/2;
                return odd[c]>=len/2+1;
            }
            int const c=(l+r+1)/2;
            return even[c]>=len/2;
        }

        int longest() const{
            int ans=0;
            for(int const x:odd) ans=max(ans,2*x-1);
            for(int const x:even) ans=max(ans,2*x);
            return ans;
        }
};

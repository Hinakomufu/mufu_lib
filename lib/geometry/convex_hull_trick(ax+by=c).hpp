#pragma once
#include <bits/stdc++.h>
#include "../geometry/line(ax+by=c).hpp"
using namespace std;

template<class T>
struct convex_hull_line{
    private:
        using Fr=fraction<T>;
        using Line_fr=line_fraction<T>;
        vector<Fr> starts;
        vector<Line_fr> lines;

    public:
        void add_line(const Line_fr&L){
            Fr s(-1,0);
            if(!lines.empty()&&lines.back().slope()==L.slope())return;
            while(!lines.empty()){
                Fr x=lines.back().intersect_x(L);
                if(x<=starts.back()){
                    lines.pop_back();
                    starts.pop_back();
                } else{
                    s=x;
                    break;
                }
            }
            lines.push_back(L);
            starts.push_back(s);
        }

        Line_fr get_hull(Fr const&x) const{
            int const idx=upper_bound(starts.begin(),starts.end(),x)-starts.begin()-1;
            return lines[idx];
        }

        int size() const{ return static_cast<int>(lines.size()); }
        const Line_fr&hull_line(int const i) const{ return lines[i]; }
        Fr hull_start(int const i) const{ return starts[i]; }
};



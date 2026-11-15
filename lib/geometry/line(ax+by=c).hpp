#pragma once
#include <bits/stdc++.h>
#include "../number_type/fraction.hpp"
using namespace std;

template<class T>
struct line_fraction{
    public:
        T a,b,c; // ax+by=c , y =(-a/b)x+(c/b)
        using Fr=fraction<T>;

        line_fraction() : a(T(0)), b(T(1)), c(T(0)){}

        line_fraction(T const a_,T const b_,T const c_) : a(a_), b(b_), c(c_){
            if(b<T(0)){
                a=-a;
                b=-b;
                c=-c;
            }
        }

        bool operator==(const line_fraction&other) const{
            if(!is_parallel(other)) return false;
            if(b!=T(0)||other.b!=T(0)){
                return static_cast<__int128>(c)*other.b==static_cast<__int128>(other.c)*b;
            }
            return static_cast<__int128>(c)*other.a==static_cast<__int128>(other.c)*a;
        }

        bool operator!=(const line_fraction&other) const{
            return !(*this==other);
        }

        bool operator<(const line_fraction&other) const{
            if((this)->slope()!=other.slope())return (this)->slope()<other.slope();
            return (this)->intercept_y()>other.intercept_y();
        }

        Fr slope() const{
            if(b==T(0))return numeric_limits<Fr>::infinity();
            return Fr(-a,b);
        }

        Fr intercept_y() const{ // x=0
            if(b==T(0))return numeric_limits<Fr>::infinity();
            return Fr(c,b);
        }

        Fr intercept_x() const{ // y=0
            if(b==T(0))return numeric_limits<Fr>::infinity();
            return Fr(c,a);
        }

        Fr y_val(Fr const&x) const{
            if(b==T(0))return numeric_limits<Fr>::infinity();
            return Fr(c-a*x,b);
        }

        Fr intersect_x(const line_fraction&other) const{
            if(is_parallel(other))return numeric_limits<Fr>::infinity();
            return Fr(other.intercept_y()-intercept_y(),slope()-other.slope());
        }

        Fr intersect_y(const line_fraction&other) const{
            if(is_parallel(other)) return numeric_limits<Fr>::infinity();
            return y_val(intersect_x(other));
        }

        static line_fraction through_line(T const&x1,T const&y1,T const&x2,T const&y2){
            T const a=y1-y2;
            T const b=x2-x1;
            T const c=a*x1+b*y1;
            return line_fraction(a,b,c);
        }

        static line_fraction perpendicular_bisector(T const&x1,T const&y1,T const&x2,T const&y2){
            T const dx=x2-x1;
            T const dy=y2-y1;

            T const a=T(2)*dx;
            T const b=T(2)*dy;
            T const c=dx*(x1+x2)+dy*(y1+y2);

            return line_fraction(a,b,c);
        }

        bool is_parallel(const line_fraction&other) const{
            return a*other.b==b*other.a;
        }

        bool is_perpendicular(const line_fraction&other) const{
            return a*other.a+b*other.b==0;
        }

        bool has_intersection(const line_fraction&other) const{
            if(*this==other) return true;
            return !is_parallel(other);
        }

        Fr calc_area_under_line(Fr const&fr_l,Fr const&fr_r) const{
            assert(b!=T(0));
            if(fr_l>fr_r)return Fr(0);
            return slope()*(fr_r*fr_r-fr_l*fr_l)/2+intercept_y()*(fr_r-fr_l);
        }

        T count_lattice_on_line(Fr const&fr_l,Fr const&fr_r,
                                bool const l_open=false,bool const r_open=false) const{
            T const l=fr_l.ceil(l_open);
            T const r=fr_r.floor(r_open);
            if(l>r)return T(0);
            if(b==T(0)){
                Fr const x(c,a);
                if((l_open?fr_l<x:fr_l<=x)&&(r_open?x<fr_r:x<=fr_r))return numeric_limits<T>::max();
                return T(0);
            }
            T const g=gcd(a,b);
            if(c%g!=T(0)) return T(0);
            T const a2=a/g;
            T const b2=b/g;
            T const c2=c/g;

            auto modinv=[&](T const A,T const M){
                auto eg=[&](auto&&self,T const a,T b)->pair<T,T>{
                    if(b==T(0)) return {T(1),T(0)};
                    auto [x1, y1]=self(self,b,a%b);
                    return {y1,x1-(a/b)*y1};
                };
                auto [x0, y0]=eg(eg,A,M);
                x0%=M;
                if(x0<T(0)) x0+=M;
                return x0;
            };

            T const inv=modinv(a2,b2);
            T x0=(c2*inv)%b2;
            if(x0<T(0)) x0+=b2;
            T const d=b2;
            T const k_min=(l<=x0?T(0):(l-x0+d-1)/d);
            T const k_max=(r<x0?T(-1):(r-x0)/d);
            if(k_min>k_max) return 0;
            return k_max-k_min+T(1);
        }

        T count_lattice_under_line(Fr fr_l,Fr fr_r,
                                   bool l_open=false,bool r_open=false,
                                   bool const line_open=false) const{ // floor_sum

            T const l=fr_l.ceil(l_open);
            T const r=fr_r.floor(r_open);
            if(l>r) return T(0);
            assert(b!=T(0));
            function<T(T,T)> div_floor=[&](T const p,T const q){
                if(q<T(0))return div_floor(-p,-q);
                if(p>=T(0)) return p/q;
                return -((-p+q-T(1))/q);
            };
            function<T(T,T)> mod_floor=[&](T const p,T const q){
                if(q<T(0)) return mod_floor(p,-q);
                T m=p%q;
                if(m<T(0)) m+=q;
                return m;
            };

            function<T(T,T,T,T)> floor_sum=
                    [&](T const x,T A,T B,T C)->T{
                if(B==T(0)) return T(0);
                if(B<T(0)) return floor_sum(x,-A,-B,-C);
                if(A<T(0)) return -floor_sum(x,-A,B,B-T(1)-C);
                if(A==T(0)) return (x+T(1))*div_floor(C,B);

                T res=T(0);
                if(A>=B){
                    res+=x*(x+T(1))*div_floor(A,B)/2;
                    A=mod_floor(A,B);
                }
                if(C>=B||C<T(0)){
                    res+=(x+T(1))*div_floor(C,B);
                    C=mod_floor(C,B);
                }
                T const y=div_floor(A*x+C,B);
                T const z=mod_floor(A*x+C,B);
                if(y!=T(0)) res+=y+floor_sum(y-T(1),B,A,z);
                return res;
            };
            T val=T(0);
            if(l<=r) val+=floor_sum(r,-a,b,c);
            if(l>T(0)) val-=floor_sum(l-T(1),-a,b,c);
            if(line_open){
                if(b>T(0)){
                    if(intercept_x()<=fr_r){
                        fr_r=intercept_x();
                        r_open=true;
                    }
                } else{
                    if(fr_l<=intercept_x()){
                        fr_l=intercept_x();
                        l_open=true;
                    }
                } // y>0 only
                if(fr_l<=fr_r)val-=count_lattice_on_line(fr_l,fr_r,l_open,r_open);
            }
            return val;
        }

        T count_lattice_under_line(T l,T r) const{
            return count_lattice_under_line(Fr(l),Fr(r),false,false,true);
        }

        void debug() const{ cout<<a<<" "<<b<<" "<<c<<endl; }
};



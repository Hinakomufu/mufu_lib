#pragma once
// sum in [0,x] of floor((an+b)/m) (m!=0)

// floor(p/q)
template<class T>
T div_floor(T p,T q){
    if(q<T(0)){
        p=-p;
        q=-q;
    }
    if(p>=T(0)) return p/q;
    return -((-p+q-T(1))/q);
}

// floor_mod(p,q)
template<class T>
T mod_floor(T p,T q){
    if(q<T(0)) q=-q;
    T r=p%q;
    if(r<T(0)) r+=q;
    return r;
}

// sum in [0,x] of floor((an+b)/m) (m!=0)
template<class T>
T floor_sum(T const&x,T a,T b,T const&m){
    if(m==T(0)) return T(0);
    if(m<T(0)) return floor_sum(x,-a,-b,-m);
    if(a<T(0)) return -floor_sum(x,-a,m-b-T(1),m);
    if(a==T(0)) return (x+T(1))*div_floor(b,m);

    T res=T(0);
    if(a>=m){
        res+=x*(x+T(1))*div_floor(a,m)/T(2);
        a=mod_floor(a,m);
    }
    if(b>=m||b<T(0)){
        res+=(x+T(1))*div_floor(b,m);
        b=mod_floor(b,m);
    }
    T const y=div_floor(a*x+b,m);
    T const z=mod_floor(a*x+b,m);
    if(y!=T(0))res+=y+floor_sum(y-T(1),m,z,a);
    return res;
}

// sum in [l,r] of floor((an+b)/m) (m!=0)
template<class T>
T floor_sum_range(T const&l,T const&r,
                  T const&a,T const&b,T const&m){
    T val=T(0);
    if(l<=r) val+=floor_sum(r,a,b,m);
    if(l>T(0)) val-=floor_sum(l-T(1),a,b,m);
    return val;
}



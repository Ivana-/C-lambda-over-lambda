#include <iostream>
#include <functional>
using namespace std;
 
//template <typename T>
//typedef std::pair<bool, T>
typedef std::pair<bool, int>  li_t;
typedef std::function<li_t()> fi_t;
 
int main() {
    fi_t nil  = []() -> li_t {return std::make_pair(false, 0);};
    auto null = [](li_t a) -> bool {return !a.first;};
    auto val  = [](li_t a) -> int  {return a.second;};
    
    auto cons = [](int a, const fi_t& l) -> fi_t {
        int p=0; return [=]() mutable -> li_t {
                    if (p) return l(); else {p=1; return std::make_pair(true, a);}};};
    
    auto map = [cons,nil,null,val](const auto& f, const fi_t& l) -> fi_t {
        auto go = [f,cons,nil,null,val](const fi_t& l, const auto& la) -> fi_t {
            li_t e=l(); return null(e) ? nil : cons(f(val(e)), la(l, la));};
        return go(l, go);};
        
    auto show = [null,val](const fi_t& l) -> int {
        auto go = [null,val](const fi_t& l, auto& la) -> int {
            li_t e=l(); if (null(e)) {cout<<'\n'; return 0;}
                        else {cout<<val(e)<<'\t'; return la(l, la);}};
        return go(l, go);};
     
    auto merge = [cons,nil,null,val](const fi_t& a, const fi_t& b) -> fi_t {
        auto go = [cons,nil,null,val](const fi_t& a, const fi_t& b, const auto& la) -> fi_t {
            li_t ea=a(), eb=b();
            return null(ea) && null(eb) ? nil :
                null(ea) ? cons(val(eb), b) : null(eb) ? cons(val(ea), a) :
                val(ea)<val(eb) ? cons(val(ea), la(a, cons(val(eb), b), la)) :
                val(ea)>val(eb) ? cons(val(eb), la(cons(val(ea), a), b, la)) :
                                  cons(val(ea), la(a, b, la));};
        return go(a, b, go);};
    
    auto take = [cons,nil,null,val](int n, const fi_t& l) -> fi_t {
        auto go = [cons,nil,null,val](int i, const fi_t& l, const auto& la) -> fi_t {
            li_t e=l(); return (!i) || null(e) ? nil : cons(val(e), la(i-1, l, la));};
        return go(n, l, go);};
        
    auto equal = [null,val](const fi_t& a, const fi_t& b) -> bool {
        auto go = [null,val](const fi_t& a, const fi_t& b, const auto& la) -> bool {
            li_t ea=a(), eb=b();
            return null(ea) && null(eb) ? true : null(ea) ? false : null(eb) ? false :
                val(ea)==val(eb) ? la(a, b, la) : false;};
        return go(a, b, go);};
   
    auto hammingList = [=](int n) -> fi_t {
    	
        auto step = [merge,map,cons](const fi_t& l) -> fi_t {
            auto sc = [map](int n, const fi_t& l) -> fi_t {
            	return map([n](int i)->int {return i*n;}, l);};
            fi_t l2=l, l3=l, l5=l;
            return cons(1, merge(sc(2, l2), merge(sc(3, l3), sc(5, l5))));};
            
        auto go = [n,step,take,equal](const fi_t& l, const auto& la) -> fi_t {
            fi_t l0=l, le0=l, l1=step(l), le1=l1;
            if (equal(take(n,le0), take(n,le1))) return take(n,l0);
            else return la(l1, la);};

        return go(cons(1,nil), go);};

    fi_t h = hammingList(100);
    show(h);
    return 0;
}

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
        
    auto filter = [cons,nil,null,val](const auto& p, const fi_t& l) -> fi_t {
        auto go = [p,cons,nil,null,val](const fi_t& l, const auto& la) -> fi_t {
            li_t e=l(); return null(e) ? nil : p(val(e)) ? cons(val(e), la(l, la)) : la(l, la);};
        return go(l, go);};
        
    auto foldl = [cons,nil,null,val](const auto& f, auto a, const fi_t& l) -> fi_t {
        auto go = [f,cons,nil,null,val](auto a, const fi_t& l, const auto& la) -> fi_t {
            li_t e=l(); return null(e) ? a : la(f(val(e),a), l, la);};
        return go(a, l, go);};
        
    auto foldr = [cons,nil,null,val](const auto& f, auto a, const fi_t& l) -> fi_t {
        auto go = [f,cons,nil,null,val](auto a, const fi_t& l, const auto& la) -> fi_t {
            li_t e=l(); return null(e) ? a : f(val(e), la(a, l, la));};
        return go(a, l, go);};
        
    auto reverse = [cons,nil,foldl](const fi_t& l) -> fi_t {return foldl(cons, nil, l);};
      
    auto show = [null,val](const fi_t& l) -> int {
        auto go = [null,val](const fi_t& l, auto& la) -> int {
            li_t e=l(); if (null(e)) {cout<<'\n'; return 0;}
                        else {cout<<val(e)<<'\t'; return la(l, la);}};
        return go(l, go);};

    auto num2list = [cons, nil](int n, const auto& la) -> fi_t {
        return n ? cons(n%10, la(n/10, la)) : nil;};
        
    auto listFromTo = [cons,nil](int i, int n) -> fi_t {
        auto go = [n,cons,nil](int i, const auto& la) -> fi_t {
            return i<=n ? cons(i, la(i+1, la)) : nil;};
        return go(i, go);};
 
    fi_t s0 = listFromTo(5, 15);
    fi_t s1 = cons(9, s0);  
    fi_t s2 = reverse(s1);
    show(s2);   
    fi_t s3 = foldr(cons, nil, listFromTo(1, 5));
    show(s3);
    
    cout << val(foldr(
        [cons,nil,val](int a, const fi_t& b) -> fi_t {return cons(a+val(b()), nil);},
        cons(0,nil), listFromTo(1, 5)) ()) << '\n';

    show(filter([](int i)->bool {return i%2==0;}, s0));
    show(map([](int i)->int {return i+10;}, cons(9,listFromTo(5, 15))));

    return 0;
}

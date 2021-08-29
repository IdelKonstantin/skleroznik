#include <stdio.h>
#include <iostream>

namespace STD {

    template<class A, class B>
    class myPair {
    
        public:
        
        myPair(){};
        myPair(A a, B b) : first(a), second(b){};
        A first;
        B second;
    };   

    template<class A, class B>
    auto make_Pair(A a, B b) {
        
        return STD::myPair(a, b);
    }
}


int main()
{
    auto mp1(STD::make_Pair("Fe", "Be"));
    std::cout << "Mp1 -> " << mp1.first << " " << mp1.second << std::endl; 

    STD::myPair<std::string, std::string> mp2;
    mp2 = STD::make_Pair(std::string("Ku"), std::string("Booo"));

    std::cout << "Mp2 -> " << mp2.first << " " << mp2.second << std::endl;

    return 0;
}
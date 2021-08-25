/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <utility>

template <class T>
class bar {
    
    private:
        
        T* worker{};
    
    public:
    
        bar (T* obj) : worker{std::forward<T*>(obj)} {
            
            worker->showI();
        }
        
        ~bar () {
            
            worker->showJ();
        }
};

template <class T>
class fizz {

public:
    static void doSomething(T* obj) {

        bar<T> worker{std::forward<T*>(obj)};
    }
};

class foo {
    
    private:
    
        uint8_t i{};
        uint8_t j{};
        
        void showI() {
            
            std::cout << "I showing you I = " << static_cast<int>(this->i) << std::endl;
        }

        void showJ() {
            
            std::cout << "I showing you J = " << static_cast<int>(this->j) << std::endl;
        }

    public:
    
        foo(uint8_t i, uint8_t j) : i(i), j(j) {};
        
        friend class bar<foo>;
};

class baz {
    
    private:
    
        uint8_t i{};
        uint8_t j{};
        uint8_t k{};
        
        void showI() {
            
            std::cout << "I showing you I = " << static_cast<int>(this->i) << std::endl;
        }

        void showJ() {
            
            std::cout << "I showing you J = " << static_cast<int>(this->j) << std::endl;
        }

    public:
    
        void sayHiFromBaz() {
            
            std::cout << "Hello World" << std::endl;
        }
    
        baz(uint8_t i, uint8_t j, uint8_t k) : i(i), j(j), k(k) {};
        
        friend class bar<baz>;
};


int main()
{
    foo f(42, 77);
    
    bar<decltype(f)>{&f};
    
    baz b(43, 78, 15);
    
    bar<decltype(b)>{&b};

    fizz<decltype(b)>::doSomething(&b);

    return 0;
}

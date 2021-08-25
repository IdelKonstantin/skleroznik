#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <memory>

template<typename T>
class service3 : public T {
    
    private:
    
        uint32_t k{};
    
    public:
        
        template<typename... Args>
        service3(uint32_t k, Args ...args) : k(k), T(std::forward<Args>(args)...) {};

        void doSomethingElse() {

            std::cout << "Bzdyak!!!" << std::endl;
        }
};


template<typename T>
class service2 : public T {
    
    private:
    
        uint8_t i{};
        uint16_t j{};
    
    public:
        
        template<typename... Args>
        service2(uint8_t i, uint16_t j, Args ...args) : i(i), j(j), T(std::forward<Args>(args)...) {};

    	void doSomething() {

    		std::cout << "Bryak!!!" << std::endl;
    	}
};

class service1 {
  
    private:
  
        std::string text{};
  
    public:  
    
        service1 (const std::string& text) : text(text) {};
    
        void sayHello() {
            
            std::cout << text << std::endl;
        }
};


int main()
{
    service2<service1> myService {42, 777, "Hello world"};
    service3<service2<service1>> myAnotherService {65600, 55, 888, "By world"};
    
    myService.sayHello();
    myService.doSomething();

    myAnotherService.sayHello();
    myAnotherService.doSomething();
    myAnotherService.doSomethingElse();

    return 0;
}

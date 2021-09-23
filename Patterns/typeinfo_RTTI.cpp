#include <iostream>
#include <memory>
#include <typeinfo>
   
class Base {
    
    public:
    virtual void showMsg() = 0;
    virtual ~Base(){};
};

class Deriv1 : public Base {
    
    std::string msg{"Initial message Deriv1"};
    
    public:
    void setMsg(const std::string& msg) {
        
        this->msg = msg;
    }
    void showMsg() override {
        
        std::cout << msg << std::endl;
    }
};

class Deriv2 : public Base {
   
    std::string msg{"Initial message Deriv2"};

    public:
    void showMsg() override {

        std::cout << msg << std::endl;
    }
};


void checkAndSet(Base* bptr) {
    
    if(typeid(*bptr) == typeid(Deriv1)) {
        
        dynamic_cast<Deriv1*>(bptr)->setMsg("Derivative message Deriv1");
    }
}

int main() {

    std::unique_ptr<Base> ptr{new Deriv1};
    
    dynamic_cast<decltype(ptr.get())>(ptr.get())->showMsg();
    checkAndSet(ptr.get());
    dynamic_cast<decltype(ptr.get())>(ptr.get())->showMsg();
}
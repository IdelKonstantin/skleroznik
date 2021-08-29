#include <thread>
#include <iostream> 

/* Это самопальная RAII обертка для того, что бы поток существовал только до выхода из области видимости */


class scoped_thread {

    std::thread t;

public:
    
    explicit scoped_thread(std::thread t_) : t(std::move(t_)) {

        std::cout << "CTOR scoped_thread called " << std::endl;

        if(!t.joinable()) {
            
            throw std::logic_error{"No thread"};
        }
    };

    ~scoped_thread() {
        
        t.join();
        std::cout << "DTOR scoped_thread called " << std::endl;
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};

void do_something(int& i)
{
    ++i;
}

void func(int i)
{
    std::cout << "Before i = " << i << std::endl;

    for(size_t j = 0; j < 1000000; ++j)
    {
        do_something(i);
    }

    std::cout << "After i = " << i << std::endl;
}

int main() {

    std::thread th(func, 42);

    try {

        scoped_thread st(std::move(th));
        // or 
        // scoped_thread st(std::thread{func, 42});
    }
    catch(const std::logic_error& le) {

        std::cout << le.what() << std::endl;
    }
    //do some client code here...

    //here scoped thread will be destructed
}
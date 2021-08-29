#include <thread>
#include <iostream> 

/* Это самопальная RAII обертка аля std::lock_guard */
// thread::detach() - можно вызвать сразу после создания потока
// за thread::join() надо следить, поэтому создается такая "владеющая"
// обертка, в конструкторе захватывающая ссылку на поток, а в деструкторе
// вызывающая метод "дождаться завершения"

class thread_guard {

    std::thread& t;

public:
    
    explicit thread_guard(std::thread& t_) : t(t_){

        std::cout << "CTOR thread_guard called " << std::endl;
    };

    ~thread_guard() {

        if(t.joinable())
        {
            t.join();
        }

        std::cout << "DTOR thread_guard called " << std::endl;
    }
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
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
    thread_guard tg(th);

    //do some client code here...

    //here thread guard will be destructed
}
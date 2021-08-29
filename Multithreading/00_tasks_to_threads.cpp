#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>    //std::for_each
#include <functional>   //std::mem_fn (wrapper for callable units)

/***************************************/
void func(const std::string& userName) {

    std::cout<<"Hello from func [" << userName << "]\n";
}

/***************************************/
class task {

public:
    void operator()() const {

        std::cout<<"Hello from class operator()\n";
    }
    void someMethod() {

        std::cout<<"Hello from class method\n";
    }
};

int main() {

    std::cout << "There are " << std::thread::hardware_concurrency() << " concurrent threads are supported on that machine.\n\n";

    /***************************************/
    std::cout << "Thread creation var.1" << std::endl; 

    // ВНИМАНИЕ!
    // 1) Аккуратнее с передачей потокам указателей на массивы в случае, если требуется 
    // передать массив в стиле Си, лучше обернуть его в какую-нибудь структуру из STL, 
    // например в std::string если передается const char*
    // 2) Все аргументы при создании потока передаются в функцию/метод по значению,
    // если требуется передача по ссылке, то необходимо использовать bind-механизмы
    // т.е. std::ref/std::cref
    // 3) Потоки можно присваивать путем перемещения. std::thread th2 = std::move(th1);
    // при этом, если потом вызвать th1.join() программа упадет или кинет исключение (!)

    std::string name{"Nikita"};
    std::thread t1(func, std::cref(name));

    std::thread t2(task{});

    task taskObj{};
    std::thread t3(&task::someMethod, taskObj /* and optional parameters */);

    std::thread t4([](){

        std::cout<<"Hello from lambda\n";
    });
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // t1.detach();
    // t2.detach();
    // t3.detach();
    // t4.detach();

    /***************************************/
    std::cout << "\nThread creation var.2" << std::endl;

    std::vector<std::thread> threadsArray{};

    threadsArray.push_back(std::thread(func, std::cref(name)));
    threadsArray.push_back(std::thread(task{}));
    threadsArray.push_back(std::thread(&task::someMethod, taskObj));    
    threadsArray.push_back(std::thread([](){std::cout<<"Hello from lambda\n";}));

    for(auto& currentThread : threadsArray) {

        currentThread.join();
        //currentThread.detach();
    }

    // or 
    //std::for_each(threadsArray.begin(), threadsArray.end(), std::mem_fn(&std::thread::join));
}
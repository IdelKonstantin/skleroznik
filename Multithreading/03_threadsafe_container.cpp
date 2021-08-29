#include <list>
#include <mutex>        
#include <algorithm>
#include <iostream>
#include <exception>

struct emptyList : std::exception
{
    const char* what() const throw()
    {
        return "Empty list";
    }   
};

template<class T>
class safeList {

    std::list<T> m_List{};
    std::mutex m_Mutex;

public:
    
    safeList() = default;
    safeList(std::list<T> List) : m_List(List) {};

    void addToList(T Value) {

        std::lock_guard<std::mutex> guard(m_Mutex); 

        // Можно использовать std::lock() & std::lock_guard<std::mutex>(mtx, std::adopt_lock) для блокировки нескольких мьютексов
        // Так же можно принудительно разблокировать захваченный lock_guard-ом мьютекс с помощью std::lock_guard<>::unlock()
        m_List.push_back(Value);
    }

    bool listContains(T valueToFind) {

        std::lock_guard<std::mutex> guard(m_Mutex);
        return std::find(m_List.begin(), m_List.end(), valueToFind) != m_List.end();
    }

    void printOutList() {

        std::lock_guard<std::mutex> guard(m_Mutex);

        if(m_List.empty()) {

            throw emptyList();
        }

        for(const auto& item : m_List) {

            std::cout << item << std::endl;
        }
    }
};


int main() {

    std::list<std::string> myList{"Hola" , "Adios"};

    safeList<std::string> mySafeList{myList};

    mySafeList.addToList(std::string("Hello"));
    
    std::cout << "contains(Hola) = " << std::boolalpha << mySafeList.listContains("Hola") << std::endl;
    std::cout << "contains(Privet) = " << std::boolalpha << mySafeList.listContains("Privet") << std::endl;

    mySafeList.printOutList();
}

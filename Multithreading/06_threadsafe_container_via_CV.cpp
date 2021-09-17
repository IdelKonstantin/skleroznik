#include <list>
#include <thread>
#include <mutex>        
#include <algorithm>
#include <iostream>
#include <exception>
#include <condition_variable>

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
    std::mutex m_Mutex{}; // так же std::recursive_mutex, boost::shared_mutex...
    std::condition_variable cv{};

public:
    
    safeList() = default;
    safeList(std::list<T> List) : m_List(List) {};

    void addToList(T Value) {

        std::unique_lock<std::mutex> guard(m_Mutex); 
        m_List.push_back(Value);
        cv.notify_one();
    }

    bool listContainsWithWait(T valueToFind) {

        std::unique_lock<std::mutex> guard(m_Mutex);
        cv.wait(guard,[this]{return !m_List.empty();});
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

    auto checkSome = [&mySafeList](const std::string& msg) {

        std::cout << "\nSafeList contains [" << msg << "] -> " << std::boolalpha 
        << mySafeList.listContainsWithWait(msg) << std::endl;
    };

    auto addSome = [&mySafeList](const std::string& msg) {

        mySafeList.addToList(msg);
    };

    std::thread th1(addSome, std::string("Hello"));
    std::thread th2(checkSome, std::string("Hola"));
    std::thread th3(checkSome, std::string("Privet"));

    th1.join();
    th2.join();
    th3.join();    

    mySafeList.printOutList();
}

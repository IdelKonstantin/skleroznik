#include <mutex>
#include <condition_variable>
#include <thread>

std::mutex mut;
std::condition_variable cv;

void data_preparation_thread() {

    std::lock_guard<std::mutex> lk(mut);
    //some code here (data preparation)
    cv.notify_one();
}

void data_processing_thread() {

    std::unique_lock<std::mutex> lk(mut);
    cv.wait(lk,[]{return true /* or something */;});
    //some code here (data execution)

    lk.unlock();
    //some code outside of critical section
}

int main() {

    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);
    
    t1.join();
    t2.join();
}

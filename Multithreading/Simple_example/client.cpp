#include <iostream>
#include <memory>
#include <zmq.hpp>
#include <thread>
#include <mutex>
#include "zhelpers.h"

void receiveMessages(std::shared_ptr<zmq::socket_t> receiver, const std::string myName) {

    while (true) {

        auto zmqMessage = s_recv(*receiver);

        if(zmqMessage.find(myName) == std::string::npos) {

            std::cout << zmqMessage << std::endl;
        }
    }
}

void sendChatMessages(std::shared_ptr<zmq::socket_t> sender, const std::string myName) {
    
    std::string enteredText{};

    while(true) {

        std::string prefix{myName + std::string(" >>> ")};
        std::getline(std::cin, enteredText);
        s_send(*sender, prefix.append(enteredText));
    }
}

int main (int argc, char *argv[]) {

    if(argc < 4) {

        std::cerr << "Мало аргументов [argc = " << argc << " < 4]. Пример использования:" << std::endl;
        std::cerr << argv[0] << " [номер_порта для приема] [номер_порта для отправки] [имя в чате]" << std::endl;
        return -1;
    }

    zmq::context_t context (1);
    std::shared_ptr<zmq::socket_t> publisher{};
    std::shared_ptr<zmq::socket_t> receiver{};

    try {
        
        const std::string subAddr{std::string("tcp://localhost:") + std::string(argv[1])};
        const std::string pushAddr{std::string("tcp://localhost:") + std::string(argv[2])};

        std::cout << "Прием: " << subAddr << std::endl;
        std::cout << "Передача: " << pushAddr << std::endl;

        publisher = std::make_shared<zmq::socket_t>(zmq::socket_t{context, ZMQ_PUSH});
        publisher->connect(pushAddr);
        
        receiver = std::make_shared<zmq::socket_t>(zmq::socket_t{context, ZMQ_SUB});
        receiver->setsockopt(ZMQ_SUBSCRIBE, "", 0);
        receiver->connect(subAddr);
    }
    catch(const zmq::error_t ze) {

        std::cerr << "Не удалось инициировать сокет (connect). Причина: " << ze.what() << std::endl;
        return -1;
    }

    const std::string myName{argv[3]};

    std::cout << "Чат запущен от имени прользвоателя [" << myName << "]" << std::endl;

    std::thread getter{receiveMessages, receiver, myName};
    std::thread pooker{sendChatMessages, publisher, myName};

    getter.join();
    pooker.join();
    return 0;
}
#include <iostream>
#include <zmq.hpp>

#include "zhelpers.h"

int main (int argc, char *argv[]) {

    if(argc < 3) {

        std::cerr << "Мало аргументов [argc = " << argc << " < 3]. Пример использования:" << std::endl;
        std::cerr << argv[0] << " [номер_порта для отправки] [номер_порта для приема]" << std::endl;
        return -1;
    }

    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    zmq::socket_t receiver (context, ZMQ_PULL);

    try {
        
        const std::string pubAddr{std::string("tcp://*:") + std::string(argv[1])};
        const std::string pullAddr{std::string("tcp://*:") + std::string(argv[2])};

        std::cout << "Передача: " << pubAddr << std::endl;
        std::cout << "Прием: " << pullAddr << std::endl;

        publisher.bind(pubAddr);
        receiver.bind(pullAddr);
    }
    catch(const zmq::error_t ze) {

        std::cerr << "Не удалось инициировать сокеты (bind). Причина: " << ze.what() << std::endl;
        return -1;
    }

    std::cout << "Сервер стартовал..." << std::endl;

    while (true) {

        auto zmqMessage = s_recv(receiver);
        std::cout << zmqMessage << std::endl;
        s_send(publisher, zmqMessage);
    }
    return 0;
}
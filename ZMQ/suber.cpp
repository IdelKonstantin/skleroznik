#include <iostream>
#include <zmq.hpp>
#include <memory>

#include "zhelpers.h"

int main (int argc, char *argv[]) {

    zmq::context_t context (1);
    std::shared_ptr<zmq::socket_t> receiver{};

    try {
        
        const std::string subAddr{std::string("tcp://localhost:5555")};
        
        receiver = std::make_shared<zmq::socket_t>(zmq::socket_t{context, ZMQ_SUB});
        receiver->setsockopt(ZMQ_SUBSCRIBE, "", 0);
        receiver->connect(subAddr);
    }
    catch(const zmq::error_t ze) {

        std::cerr << "Не удалось инициировать сокет (connect). Причина: " << ze.what() << std::endl;
        return -1;
    }

    while (true) {

        auto zmqMessage = s_recv(*receiver);

        if(!zmqMessage.empty()) {

            std::cout << zmqMessage << std::endl;
        }
    }
}
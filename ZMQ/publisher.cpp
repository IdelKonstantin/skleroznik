#include <iostream>
#include <zmq.hpp>

#include "zhelpers.h"

int main (int argc, char *argv[]) {

	zmq::context_t context (1);
	zmq::socket_t publisher (context, ZMQ_PUB);

	try {

		const std::string pubAddr{std::string("tcp://*:5555")};
		publisher.bind(pubAddr);
	}
	catch(const zmq::error_t ze) {

		std::cerr << "Не удалось инициировать сокеты (bind). Причина: " << ze.what() << std::endl;
		return -1;
	}

	while (true) {

		std::string zmqMessage = "Hello world!";
		s_send(publisher, zmqMessage, ZMQ_DONTWAIT);
	}
}
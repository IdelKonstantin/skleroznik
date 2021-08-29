#include <zmq.h>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>
#include <mutex>
#include "zhelpers.h"
#include "handshakes.h"

class taskLaunchToSIUsender final {

private:
	const int zpollTimeout{100};

	std::mutex mtx{};
	std::queue<std::string> unsendedMessages{};

	std::string sendAddr{};
	std::string receiveAddr{};
 
    std::string receiveMessage{};

	zmq::context_t context{1};
    std::shared_ptr<zmq::socket_t> send{nullptr};
	std::shared_ptr<zmq::socket_t> receive{nullptr};

public:
	taskLaunchToSIUsender() : sendAddr(std::string("tcp://*:8898")), receiveAddr(std::string("tcp://*:8897")) {};
	
	taskLaunchToSIUsender(const std::string& sendAddr, const std::string& receiveAddr) 
	: sendAddr(sendAddr), receiveAddr(receiveAddr) {};

	bool zmqInit();
	bool sendMessage(const std::string&& message) const;
	std::string& getReceivedMessage();
	void addUnsendedMessageToQueue(const std::string&& message);
	auto messageQueueSize();
	void pubHandshake() const;
	void sendAllSuspendedMessages();
};

//////////////////////////////////////////////////////////////////////////////

bool taskLaunchToSIUsender::zmqInit() {

	try {

		send = std::make_shared<zmq::socket_t>(context, ZMQ_PUB);
		send->bind(sendAddr);

		receive = std::make_shared<zmq::socket_t>(context, ZMQ_PULL);
		receive->bind(receiveAddr);
	}
	catch(const zmq::error_t& ex) {

		std::cerr << ex.what() << std::endl;
		return false;
	}
	return true;
}

bool taskLaunchToSIUsender::sendMessage(const std::string&& message) const {

	std::cout << "Отправляю: " << message << std::endl;
	s_send(*send, message, ZMQ_DONTWAIT);
}

std::string& taskLaunchToSIUsender::getReceivedMessage() {

	receiveMessage.clear();

    zmq::pollitem_t pollItems [] = {

        { static_cast<void*>(*(receive.get())), 0, ZMQ_POLLIN, 0 }
    };

    zmq::poll (&pollItems[0], 1, zpollTimeout);

	if(pollItems[0].revents & ZMQ_POLLIN) {
               
        receiveMessage = s_recv(*receive);
        std::cout << "Have got messsage: " << receiveMessage << std::endl;
	}
	return receiveMessage;
}

void taskLaunchToSIUsender::pubHandshake() const {

	std::cout << "Отправляю: " << siu_task::HANDSHAKE_TO_SIU << std::endl;
	s_send(*send, siu_task::HANDSHAKE_TO_SIU, ZMQ_DONTWAIT);
}

void taskLaunchToSIUsender::addUnsendedMessageToQueue(const std::string&& message) {

	std::cout << "Добавляю в очередь: " << message << std::endl;

	std::lock_guard<std::mutex> lg{mtx}; 
	unsendedMessages.push(std::move(message));
}

auto taskLaunchToSIUsender::messageQueueSize() {

	std::lock_guard<std::mutex> lg{mtx};
	return unsendedMessages.size();
}

void taskLaunchToSIUsender::sendAllSuspendedMessages() {

	std::lock_guard<std::mutex> lg{mtx};
	
	while(!unsendedMessages.empty()) {
		
		auto suspendedMessage = unsendedMessages.front();
		std::cout << "Отправляю из очереди: " << suspendedMessage << std::endl;
		s_send(*send, suspendedMessage, ZMQ_DONTWAIT);
		unsendedMessages.pop();
	}
}

//////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv)
{
	std::unique_ptr<taskLaunchToSIUsender> worker{new taskLaunchToSIUsender};
	
	if(!worker || (!worker->zmqInit())) {

		std::cerr << "Ошибка в работе ZMQ. Завершение работы." << std::endl; 
		return 1;
	}

    while(true) {

		const std::string answerMessage{"recoo-coo"};
		std::string siuMessage{};

		siuMessage = worker->getReceivedMessage();

    	if(siuMessage == siu_task::HANDSHAKE_TO_TASK) {

    		//Если пришел хэндшейк, отправить ответное рукопожатие
    		//и ответное сообщение со всеми необработанными
    		worker->pubHandshake();
    		worker->sendMessage(std::move(answerMessage));
    		worker->sendAllSuspendedMessages();
    	}
		else if (siuMessage.empty()) {

			//Если получено пустое сообщение, то сервер отсутствует,
			//добавляем неотправленные сообщения в очередь на отправку
			worker->addUnsendedMessageToQueue(std::move(answerMessage));
		}
		else { 	

			//Пришло сообщение с содержимым отличным от нуля
			//здесь могла бы быть ваша рекл..., то есть ваш обработчик
			std::cout << "Типа обрабатываю это: " << siuMessage << std::endl;
		}
    }

    return 0;
}
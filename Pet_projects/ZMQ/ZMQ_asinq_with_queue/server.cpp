#include <iostream>
#include <thread>
#include <memory>
#include <queue>
#include <mutex>
#include "zhelpers.h"
#include "CThreadPool.h"
#include "handshakes.h"

class SIUToTaskLauncherReplier final {

private:
	//const int Linger{10};
	const int waitTimeout{1};
	const int zpollTimeout{100};
	const int maxThreads{4};

	zmq::context_t context{1};

	std::string subAddr{};
	std::string pushAddr{};

	std::shared_ptr<zmq::socket_t> serverSubscriber{nullptr};
	std::shared_ptr<zmq::socket_t> serverPusher{nullptr};

	std::mutex mtx{};
	std::queue<std::string> unprocessedAnswers{};

	threadpool::CThreadPool threadPool{maxThreads};

	std::string clientMessage{};

	void taskProcesser(const std::string& taskMessage);
	
public:
	SIUToTaskLauncherReplier() : subAddr("tcp://localhost:8898"), pushAddr("tcp://localhost:8897") {};
	SIUToTaskLauncherReplier(const std::string& subAddr, const std::string& pushAddr) : subAddr(subAddr), pushAddr(pushAddr) {};

	bool zmqInit();
	void threadPoolInfo();
	std::string& getSubscribedMessage();
	void pushIntoThreadPool(const std::string& taskMessage);
	void pushHandshake() const;
	void addToUnprocessedAnswersQueue(const std::string&& clientMessage);
	void sendMessages(const std::string&& clientMessage);
};

///////////////////////////////////////////////////////////////////////////////////

void SIUToTaskLauncherReplier::taskProcesser(const std::string& taskMessage) {

	std::cout << "Работаю в потоке [" << std::this_thread::get_id() << "] с сообщением: [" << taskMessage << "]" << std::endl;
}

void SIUToTaskLauncherReplier::pushHandshake() const {

	std::cout << "Отправляю: " << siu_task::HANDSHAKE_TO_TASK << std::endl;
 	s_send(*serverPusher, siu_task::HANDSHAKE_TO_TASK, ZMQ_DONTWAIT);
}

bool SIUToTaskLauncherReplier::zmqInit() {

	try {
		
		serverPusher = std::make_shared<zmq::socket_t>(context, ZMQ_PUSH);
		serverPusher->connect(pushAddr);

		serverSubscriber = std::make_shared<zmq::socket_t>(context, ZMQ_SUB);
		serverSubscriber->setsockopt(ZMQ_RCVTIMEO, &waitTimeout, sizeof(waitTimeout));
		//serverSubscriber->setsockopt(ZMQ_LINGER, &Linger, sizeof(Linger));    	
		serverSubscriber->setsockopt(ZMQ_SUBSCRIBE, "", 0);
		serverSubscriber->connect(subAddr);
	}
	catch(const zmq::error_t& ex) {

		std::cerr << ex.what() << std::endl;
		return false;
	}
	return true;
}

void SIUToTaskLauncherReplier::threadPoolInfo() {

	std::cout << "\nСообщений в очереди   [" << threadPool.qsize() << "]" << std::endl;
	std::cout << "Используюциеся потоки [" << threadPool.n_idle() << "]" << std::endl;
	std::cout << "Всего потоков в пуле  [" << threadPool.size() << "]" << std::endl;			
}

void SIUToTaskLauncherReplier::pushIntoThreadPool(const std::string& taskMessage) {

	threadPool.push([this, &taskMessage](int) {

		taskProcesser(std::cref(taskMessage));
	});			
}

std::string& SIUToTaskLauncherReplier::getSubscribedMessage() {

	clientMessage.clear();

    zmq::pollitem_t pollItems [] = {

        { static_cast<void*>(*(serverSubscriber.get())), 0, ZMQ_POLLIN, 0 }
    };

    zmq::poll (&pollItems[0], 1, zpollTimeout);

	if(pollItems[0].revents & ZMQ_POLLIN) {
               
		clientMessage = s_recv(*serverSubscriber);
        std::cout << "Получил: " << clientMessage << std::endl;
	}

	return clientMessage;
}

void SIUToTaskLauncherReplier::addToUnprocessedAnswersQueue(const std::string&& clientMessage) {

	std::cout << "Добавляю в очередь: " << clientMessage << std::endl;

	std::lock_guard<std::mutex> lg{mtx};
	unprocessedAnswers.push(clientMessage);
}

void SIUToTaskLauncherReplier::sendMessages(const std::string&& clientMessage) {

	std::cout << "Отправляю (1) " << clientMessage << std::endl;
	s_send(*serverPusher, clientMessage, ZMQ_DONTWAIT);

	std::lock_guard<std::mutex> lg{mtx};
	
	while(!unprocessedAnswers.empty()) {

		auto dummy = unprocessedAnswers.front();
		std::cout << "Отправляю (2) " << dummy << std::endl;
		s_send(*serverPusher, dummy, ZMQ_DONTWAIT);
		unprocessedAnswers.pop();	
	}	
}

///////////////////////////////////////////////////////////////////////////////////
int main() {

	std::unique_ptr<SIUToTaskLauncherReplier> worker{new SIUToTaskLauncherReplier};

	if(!worker || (!worker->zmqInit())) {

		std::cerr << "Ошибка в работе ZMQ. Завершение работы." << std::endl; 
		return 1;
	}

	worker->pushHandshake(); //Отправка стартового рукопожатия ("Прогрев", так сказать...)

	while(true) {

		const std::string clientMessage{"Coo-coo"};

		worker->pushHandshake();
		auto taskMessage = worker->getSubscribedMessage();
		
		if(taskMessage == siu_task::HANDSHAKE_TO_SIU) { //От Таска пришел хендшейк

			//Если пришел хэндшейк, отправить сообщение и все, что в очереди
			worker->sendMessages(std::move(clientMessage));
		}
		else if(taskMessage.empty()){ //От таска ничего не пришло (предполагаем, что он отключен)

			//Если хэндшейк не получен, то добавить все отправляемые сообщения в очередь
			//и запустить обработку полученного на стороне сервера
			worker->addToUnprocessedAnswersQueue(std::move(clientMessage));

		}
		else { //От таска пришло сообщение с задачей, принимаем в обработку

			worker->pushIntoThreadPool(taskMessage);
			worker->threadPoolInfo();
		}
	}

	return 0;
}

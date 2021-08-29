#include <zmq.h>
#include <cstring>
#include <string>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <experimental/filesystem>
#include "ini_file_parser.h"

#define MAX_BUFFER_SIZE 0xFFF
char buffer [MAX_BUFFER_SIZE];

namespace fs = std::experimental::filesystem;

int main (int argc, char **argv)
{
    if(argc < 2) {

        std::cerr << "No CLI args. Exiting... " << std::endl;
        return -1;
    }

    auto currentWorkingPath = fs::current_path().string();
    currentWorkingPath.append("/client.ini");

    std::unique_ptr<iniFileParser> iniWorker{new iniFileParser(currentWorkingPath)};

    if(!iniWorker->parseIniFile()) {

        std::cerr << "Wrong client.ini. Exiting... " << std::endl;
        return -1;        
    }

    auto host = iniWorker->getParamValue("host");
    auto port = iniWorker->getParamValue("port");

    if(host.empty() || port.empty()) {

        std::cerr << "Empty host or port values. Exiting... " << std::endl;
        return -1;        
    }

    std::string zmqAddr = "tcp://" + host + ":" + port;

    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);

    //zmq_connect (requester, "tcp://localhost:5555");
    zmq_connect (requester, zmqAddr.c_str());

    std::string arg{argv[1]};

    //Здесь передаются данные в формате JSON (как аргумент коммандной строки)
    zmq_send (requester, arg.c_str(), arg.length(), 0);

    //Получение ответа от решателя
    zmq_recv (requester, ::buffer, MAX_BUFFER_SIZE, 0);

    //Этот поток потом можно будет перенаправить конвейером в другое приложение
    std::cout << ::buffer << std::endl;

    zmq_close (requester);
    zmq_ctx_destroy (context);
    
    return 0;
}
#include <zmq.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <assert.h>
#include "server_logger.h"
#include "json_to_s2.h"
#include "trajectory_solver.h"

#define MAX_BUFFER_SIZE 0xFFF
char buffer [MAX_BUFFER_SIZE];

int main (void)
{
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    
    if (rc != 0) {

      std::cerr << "S2 service is allready working..." << std::endl;      
      return -1;
    }

    std::cerr << "S2 service has been started..." << std::endl;
    
    std::unique_ptr<serverLogger> logWorker{new serverLogger};
    std::unique_ptr<jsonToS2helper> jsonWorker{new jsonToS2helper};

    while (true) {
      
      /* Получение запроса сервисом (Json с входными данными) */
      zmq_recv (responder, ::buffer, MAX_BUFFER_SIZE, 0);
      logWorker->logoutData(::buffer, logType::REQUEST);

      /* Парсинг JSONа с входными данными, в случае успеха -> передача данных решателю, 
         в случае ошибки -> отправка json-а с ее описанием клиенту */

      json_error_t parsingResult = jsonWorker->parseJsonRequest(::buffer);

      if(parsingResult != json_err::DONE) {

        jsonWorker->getErrorMessage(::buffer);
      }
      else {

        /* Запуск баллистического решателя */
        trajectorySolver(
          
          jsonWorker->getMeteo(), 
          jsonWorker->getBullet(), 
          jsonWorker->getRifle(), 
          jsonWorker->getScope(), 
          jsonWorker->getInputs(), 
          jsonWorker->getOptions(), 
          jsonWorker->getResults()
        );

        /* Серилизация расчетных данных в Json */
        jsonWorker->serializeResults(::buffer);
      }

      /* Отправка ответа клиенту (Json с расчетными поправками или с ошибкой) */
      zmq_send (responder, ::buffer, std::strlen(::buffer), 0);
      logWorker->logoutData(::buffer, logType::RESPONSE);
    }
    return 0;
}

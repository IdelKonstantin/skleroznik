#ifndef SERVER_WORKER_H
#define SERVER_WORKER_H

#include "commonStuff.h"
#include "CUUIDGenerator.h"

class serverWorker {

    uuid::CUUIDGenerator m_uuidGen{};

    const char* m_host;
    const char* m_port;

    struct addrinfo m_hints;
    struct addrinfo* m_bind_address;

    cool_chat::socket_t m_socket_listen;

public:
    serverWorker(const char* host, const char* port);
    ~serverWorker();
    
    bool init();
    void mainProcessingLoop();
};

#endif /* SERVER_WORKER_H */

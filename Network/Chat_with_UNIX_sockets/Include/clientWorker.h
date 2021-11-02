#ifndef CLIENT_WORKER_H
#define CLIENT_WORKER_H

#include "commonStuff.h"
#include "CUUIDGenerator.h"

class clientWorker {

    uuid::CUUIDGenerator m_uuidGen{};
    std::string m_sessionUUID{};

    const char* m_host;
    const char* m_port;

    struct addrinfo m_hints;
    struct addrinfo* m_peer_address;

    cool_chat::socket_t m_socket_peer;

public:
    clientWorker(const char* host, const char* port);
    ~clientWorker();
    
    bool init();
    void mainProcessingLoop();
};

#endif /* CLIENT_WORKER_H */

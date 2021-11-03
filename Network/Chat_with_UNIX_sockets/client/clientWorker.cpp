#include "clientWorker.h"

clientWorker::clientWorker(const char* host, const char* port) : m_host(host), m_port(port) {}

clientWorker::~clientWorker() {

    if(cool_chat::isValidSocket(m_socket_peer)) {

        cool_chat::closeSocket(m_socket_peer);
    }
}

bool clientWorker::init() {

    memset(&m_hints, 0, sizeof(m_hints));
    m_hints.ai_socktype = SOCK_STREAM;
    
    if (getaddrinfo(m_host, m_port, &m_hints, &m_peer_address)) {

        std::cerr << "getaddrinfo() failed. Reason: " << cool_chat::getSocketError() << std::endl;
        return false;
    }

    m_socket_peer = socket(m_peer_address->ai_family, m_peer_address->ai_socktype, m_peer_address->ai_protocol);

    if (!cool_chat::isValidSocket(m_socket_peer)) {

        std::cerr << "socket() failed. Reason: " << cool_chat::getSocketError() << std::endl; 
        return false;
    }

    //std::cout << "Connecting..."  << std::endl;

    if (connect(m_socket_peer, m_peer_address->ai_addr, m_peer_address->ai_addrlen)) {

        std::cerr << "connect() failed. Reason: " << cool_chat::getSocketError() << std::endl; 
        return false;
    }
    freeaddrinfo(m_peer_address);
    m_sessionUUID = m_uuidGen.getNewUUID();
    return true;
}

void clientWorker::mainProcessingLoop() {

    std::cout << "Client with UUID [" << m_sessionUUID << "] has been started" << std::endl;
    std::cout << "Input text ended by enter..."  << std::endl;

    while(true) {

        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(m_socket_peer, &reads);
        FD_SET(0, &reads);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;

        if (select(m_socket_peer + 1, &reads, 0, 0, &timeout) < 0) {
            
            std::cerr << "select() failed. Reason: " << cool_chat::getSocketError() << std::endl;
            exit(1);
        }

        if (FD_ISSET(m_socket_peer, &reads)) {
            
            char read[cool_chat::MAX_BUFF_SIZE];

            auto bytes_received = recv(m_socket_peer, read, cool_chat::MAX_BUFF_SIZE, 0);
            if (bytes_received < 1) {
                
                std::cerr << "Connection closed by peer" << std::endl;
                break;
            }
            
            //std::cout << "Have got: " << read;
            std::cout << m_messageSerializer.deserializeToString(std::string(read));
        }

        if(FD_ISSET(0, &reads)) {

            char read[cool_chat::MAX_BUFF_SIZE];
            
            if (!fgets(read, cool_chat::MAX_BUFF_SIZE, stdin)) {
            
                break;
            }

            std::string messageEntered{read};
            std::string jsonToSend{m_messageSerializer.serializeToJson({m_sessionUUID, messageEntered})};
            send(m_socket_peer, jsonToSend.c_str(), strlen(jsonToSend.c_str()), 0);
        }
    }
}
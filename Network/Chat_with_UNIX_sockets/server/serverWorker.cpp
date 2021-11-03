#include "serverWorker.h"

serverWorker::serverWorker(const char* host, const char* port) : m_host(host), m_port(port) {}

serverWorker::~serverWorker() {

    if(cool_chat::isValidSocket(m_socket_listen)) {

        cool_chat::closeSocket(m_socket_listen);
    }
}

bool serverWorker::init() {

    memset(&m_hints, 0, sizeof(m_hints));
    m_hints.ai_family = AF_INET;
    m_hints.ai_socktype = SOCK_STREAM;
    m_hints.ai_flags = AI_PASSIVE;
    
    if (getaddrinfo(0, m_port, &m_hints, &m_bind_address)) {

        std::cerr << "getaddrinfo() failed. Reason: " << cool_chat::getSocketError() << std::endl;
        return false;
    }

    m_socket_listen = socket(m_bind_address->ai_family, m_bind_address->ai_socktype, m_bind_address->ai_protocol);

    if (!cool_chat::isValidSocket(m_socket_listen)) {

        std::cerr << "socket() failed. Reason: " << cool_chat::getSocketError() << std::endl; 
        return false;
    }

    //std::cout << "Binding..."  << std::endl;

    if (bind(m_socket_listen, m_bind_address->ai_addr, m_bind_address->ai_addrlen)) {

        std::cerr << "connect() failed. Reason: " << cool_chat::getSocketError() << std::endl; 
        return false;
    }
    freeaddrinfo(m_bind_address);

    //std::cout << "Listening..." << std::endl;

    if (listen(m_socket_listen, cool_chat::MAX_CLIENTS) < 0) {

        std::cerr << "listen() failed. Reason: " << cool_chat::getSocketError() << std::endl; 
        return false;
    }
    return true;
}

void serverWorker::mainProcessingLoop() {

    fd_set master;
    FD_ZERO(&master);
    FD_SET(m_socket_listen, &master);
    cool_chat::socket_t max_socket = m_socket_listen;

    std::cout << "Waiting for connections..."  << std::endl; 

    while(true) {

        fd_set reads;
        reads = master;
        
        if (select(max_socket + 1, &reads, 0, 0, 0) < 0) {
            
            std::cerr << "select() failed. Reason: " << cool_chat::getSocketError() << std::endl;
            exit(1);
        }

        cool_chat::socket_t i;
        for(i = 1; i <= max_socket; ++i) {
            
            if (FD_ISSET(i, &reads)) {

                if (i == m_socket_listen) {
                    
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    cool_chat::socket_t socket_client = accept(m_socket_listen, (struct sockaddr*) &client_address, &client_len);
                    
                    if (!cool_chat::isValidSocket(socket_client)) {
                        
                        std::cerr << "accept() failed. Reason: " << cool_chat::getSocketError() << std::endl;
                        exit(1);
                    }

                    FD_SET(socket_client, &master);
                    
                    if (socket_client > max_socket) {

                        max_socket = socket_client;
                    }

                    char address_buffer[cool_chat::MAX_BUFF_SIZE];
                    
                    getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
                    std::cout << "New connection from: " << address_buffer << std::endl;

                    m_sockSessions.insert(std::make_pair(i, m_uuidGen.getNewUUID()));

                    std::cout << "Total connections: " << m_sockSessions.size() << std::endl;
                    
                    for(const auto session : m_sockSessions) {

                        std::cout << "Socket Id= " << session.first 
                        << ", socket session UUID = " << session.second << std::endl;
                    }

                } else {
                    
                    char read[cool_chat::MAX_BUFF_SIZE];
                    int bytes_received = recv(i, read, cool_chat::MAX_BUFF_SIZE, 0);
                    
                    if (bytes_received < 1) {
                        
                        FD_CLR(i, &master);
                        cool_chat::closeSocket(i);
                        continue;
                    }

                    cool_chat::socket_t j;
                    for (j = 1; j <= max_socket; ++j) {
                        
                        if (FD_ISSET(j, &master)) {
                            
                            if (j == m_socket_listen || j == i) {

                                continue;
                            }
                            else {

                                send(j, read, bytes_received, 0);
                            }
                        }
                    }
                }
            }
        }
    }
}
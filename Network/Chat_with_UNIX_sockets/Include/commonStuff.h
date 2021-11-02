#ifndef COMMON_STUFF_H
#define COMMON_STUFF_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <string>
#include <iostream>

namespace cool_chat {

    using socket_t = int;

    const size_t MAX_BUFF_SIZE{0xFFF};
    const int MAX_CLIENTS{10};

    bool isValidSocket(const socket_t socket);
    void closeSocket(const socket_t socket);
    std::string getSocketError();
}

#endif /* COMMON_STUFF_H */

#include "commonStuff.h"

bool cool_chat::isValidSocket(const socket_t socket) {

    return socket >= 0;
}

void cool_chat::closeSocket(const socket_t socket) {

    close(socket);
}

std::string cool_chat::getSocketError() {

    return std::string(strerror(errno));
}
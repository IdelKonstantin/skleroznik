#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/unix_socket_example"
#define BUFFER_SIZE 256

int main() {
    int client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];
    
    // 1. Создаем сокет
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // 2. Готовим адрес сервера
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // 3. Подключаемся к серверу
    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Подключено к UNIX-сокет серверу. Вводи сообщения:\n");
    printf("(пустая строка для выхода)\n");
    
    // 4. Общаемся с сервером
    while (1) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        // Проверка на выход (пустая строка)
        if (buffer[0] == '\n') {
            break;
        }
        
        // Отправляем сообщение серверу
        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("send");
            break;
        }
        
        // Ждем ответ от сервера
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("Сервер закрыл соединение\n");
            } else {
                perror("recv");
            }
            break;
        }
        
        printf("Ответ сервера: %s", buffer);
    }
    
    close(client_fd);
    printf("Отключились от сервера\n");
    
    return 0;
}
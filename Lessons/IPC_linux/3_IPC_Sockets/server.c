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
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];
    
    // 1. Создаем сокет
    // AF_UNIX - сокет файловой системы
    // SOCK_STREAM - потоковый (надёжный, с установкой соединения)
    // 0 - протокол по умолчанию (для AF_UNIX это единственный протокол)
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // 2. Удаляем старый файл сокета, если он существует
    unlink(SOCKET_PATH);
    
    // 3. Готовим адресную структуру
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // 4. Привязываем сокет к адресу (файлу)
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // 5. Начинаем слушать входящие соединения
    // 5 - размер очереди ожидающих соединений
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        unlink(SOCKET_PATH);
        exit(EXIT_FAILURE);
    }
    
    printf("UNIX-сокет сервер запущен и слушает %s\n", SOCKET_PATH);
    
    // 6. Принимаем соединения в цикле
    while (1) {
        printf("Ожидаем подключения клиента...\n");
        
        // Принимаем соединение
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }
        
        printf("Клиент подключился!\n");
        
        // 7. Общаемся с клиентом
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
            
            if (bytes_received <= 0) {
                if (bytes_received == 0) {
                    printf("Клиент отключился\n");
                } else {
                    perror("recv");
                }
                break;
            }
            
            printf("Получено от клиента: %s", buffer);
            
            // Отправляем подтверждение
            const char* response = "Сообщение доставлено!\n";
            send(client_fd, response, strlen(response), 0);
            
            // Проверка на выход
            if (strcmp(buffer, "exit\n") == 0) {
                printf("Клиент завершил сессию\n");
                break;
            }
        }
        
        close(client_fd);
        printf("Соединение закрыто\n\n");
    }
    
    // Сюда никогда не дойдем в этом примере, но для полноты:
    close(server_fd);
    unlink(SOCKET_PATH);
    
    return 0;
}
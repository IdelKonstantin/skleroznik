#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define FIFO_PATH "/tmp/my_fifo"
#define BUFFER_SIZE 256

int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    
    // 1. Проверяем, существует ли FIFO
    if (access(FIFO_PATH, F_OK) == -1) {
        printf("Клиент: FIFO %s не существует. Запусти сначала сервер!\n", FIFO_PATH);
        exit(EXIT_FAILURE);
    }
    
    printf("Клиент: подключаемся к серверу...\n");
    
    // 2. Открываем FIFO для записи
    // open() блокируется, пока сервер не откроет FIFO на чтение
    // Если сервер уже запущен и ждет, соединение произойдет сразу
    fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    printf("Клиент: подключились к серверу. Вводи сообщения (exit для выхода):\n");
    
    // 3. Читаем строки с клавиатуры и отправляем серверу
    while (1) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        // Отправляем данные серверу
        ssize_t bytes_written = write(fd, buffer, strlen(buffer));
        if (bytes_written == -1) {
            perror("write");
            break;
        }
        
        // Если ввели "exit", завершаем работу
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }
    }
    
    // 4. Закрываем соединение
    close(fd);
    printf("Клиент: отключились от сервера\n");
    
    return 0;
}
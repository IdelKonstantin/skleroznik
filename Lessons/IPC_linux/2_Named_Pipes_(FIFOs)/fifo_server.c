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
    
    // 1. Создаем FIFO (именованный канал)
    // Права доступа: 0666 (чтение и запись для всех)
    // Важно: mkfifo() создает только файл, но не открывает его
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (errno != EEXIST) {
            // Если ошибка не "файл уже существует" - это проблема
            perror("mkfifo");
            exit(EXIT_FAILURE);
        } else {
            printf("FIFO %s уже существует, используем его\n", FIFO_PATH);
        }
    }
    
    printf("Сервер: ожидаем подключения клиента...\n");
    
    // 2. Открываем FIFO для чтения
    // Внимание: open() на FIFO блокируется, пока кто-то не откроет этот же FIFO на запись
    fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        unlink(FIFO_PATH); // Удаляем FIFO при ошибке
        exit(EXIT_FAILURE);
    }
    
    printf("Сервер: клиент подключился, начинаем чтение...\n");
    
    // 3. Читаем данные от клиента
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Гарантированно завершаем строку
        printf("Сервер получил: %s", buffer);
        
        // Проверяем на команду выхода
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Сервер: получена команда на завершение\n");
            break;
        }
    }
    
    if (bytes_read == 0) {
        printf("Сервер: клиент закрыл соединение (EOF)\n");
    } else if (bytes_read == -1) {
        perror("read");
    }
    
    // 4. Закрываем и удаляем FIFO
    close(fd);
    unlink(FIFO_PATH);
    printf("Сервер: FIFO удален, завершаем работу\n");
    
    return 0;
}
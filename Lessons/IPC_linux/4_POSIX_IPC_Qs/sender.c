#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define QUEUE_NAME "/my_posix_queue"
#define MAX_SIZE 1024
#define PRIORITY 1

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE];
    
    // 1. Настраиваем атрибуты очереди
    attr.mq_flags = 0;          // Блокирующий режим
    attr.mq_maxmsg = 10;        // Максимум сообщений в очереди
    attr.mq_msgsize = MAX_SIZE; // Максимальный размер сообщения
    attr.mq_curmsgs = 0;        // Текущее количество сообщений (не используется при создании)
    
    // 2. Создаем или открываем очередь
    // Права: 0666 (чтение/запись для всех)
    // O_CREAT - создать если не существует
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    
    printf("Очередь %s открыта для отправки\n", QUEUE_NAME);
    printf("Вводи сообщения (или 'exit' для выхода):\n");
    
    // 3. Отправляем сообщения в цикле
    while (1) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(buffer, MAX_SIZE, stdin) == NULL) {
            break;
        }
        
        // Убираем символ новой строки
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Проверка на выход
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        
        // Отправляем сообщение с приоритетом 1
        // Приоритет может быть от 0 (низший) до sysconf(_SC_MQ_PRIO_MAX) (обычно 32 или 32768)
        if (mq_send(mq, buffer, strlen(buffer) + 1, PRIORITY) == -1) {
            perror("mq_send");
            continue;
        }
        
        printf("Отправлено: %s (приоритет %d)\n", buffer, PRIORITY);
    }
    
    // 4. Закрываем очередь
    mq_close(mq);
    printf("Отправитель завершил работу\n");
    
    return 0;
}
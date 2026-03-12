#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <signal.h>

#define QUEUE_NAME "/my_posix_queue"
#define MAX_SIZE 1024

// Флаг для корректного завершения по сигналу
volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig) {
    keep_running = 0;
}

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE];
    unsigned int priority;
    
    // Устанавливаем обработчик сигнала для Ctrl+C
    signal(SIGINT, handle_signal);
    
    // 1. Открываем существующую очередь (только для чтения)
    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        printf("Очередь не существует. Сначала запусти отправителя?\n");
        exit(EXIT_FAILURE);
    }
    
    // 2. Получаем атрибуты очереди (чтобы знать максимальный размер сообщения)
    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        mq_close(mq);
        exit(EXIT_FAILURE);
    }
    
    printf("Очередь %s открыта для чтения\n", QUEUE_NAME);
    printf("Макс. сообщений: %ld, макс. размер: %ld байт\n", 
           attr.mq_maxmsg, attr.mq_msgsize);
    printf("Ожидаем сообщения (Ctrl+C для выхода)...\n\n");
    
    // 3. Читаем сообщения в цикле
    while (keep_running) {
        // Очищаем буфер
        memset(buffer, 0, MAX_SIZE);
        
        // Принимаем сообщение
        // mq_receive блокируется, пока не появится сообщение
        ssize_t bytes_read = mq_receive(mq, buffer, attr.mq_msgsize, &priority);
        
        if (bytes_read >= 0) {
            printf("Получено [приоритет %u]: %s\n", priority, buffer);
        } else {
            if (errno != EINTR) { // EINTR возникает при сигнале (Ctrl+C)
                perror("mq_receive");
            }
            break;
        }
    }
    
    // 4. Закрываем очередь
    mq_close(mq);
    printf("\nПолучатель завершил работу\n");
    
    return 0;
}
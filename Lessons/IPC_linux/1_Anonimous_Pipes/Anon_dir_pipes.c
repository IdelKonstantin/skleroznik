#define _GNU_SOURCE // Для получения O_DIRECT из fcntl.h
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main() {
    int pipefd[2];
    char buffer[128];

    // 1. Создаем pipe с флагом O_DIRECT с помощью pipe2()
    // Внимание: O_DIRECT для pipe требует достаточно больших буферов (по умолчанию 4096 на пакет)
    if (pipe2(pipefd, O_DIRECT) == -1) {
        perror("pipe2 with O_DIRECT");
        // Проверяем, поддерживает ли ядро (должно быть 2.6.35+)
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // ---- Дочерний процесс (читатель) ----
        close(pipefd[1]); // Закрываем запись

        // Пытаемся читать пакеты
        for (int i = 0; i < 3; i++) {
            ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
            if (bytes_read > 0) {
                printf("[Дочерний] Получил пакет %d: \"%s\" (%ld байт)\n", 
                       i+1, buffer, bytes_read);
            } else if (bytes_read == 0) {
                printf("[Дочерний] Канал закрыт (EOF)\n");
                break;
            } else {
                perror("[Дочерний] read");
                break;
            }
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);

    } else {
        // ---- Родительский процесс (писатель) ----
        close(pipefd[0]); // Закрываем чтение

        // Отправляем два отдельных сообщения
        // ВАЖНО: С O_DIRECT каждое сообщение должно помещаться в буфер канала
        // Иначе write() может заблокироваться или вернуть ошибку, если буфер переполнен
        
        const char *msg1 = "Пакет 1";
        const char *msg2 = "Пакет 2, который чуть длиннее";
        
        printf("[Родитель] Отправляем: \"%s\"\n", msg1);
        if (write(pipefd[1], msg1, strlen(msg1) + 1) == -1) { // +1 чтобы отправить \0
            perror("write msg1");
        }

        // Маленькая пауза, чтобы подчеркнуть раздельность пакетов
        usleep(100000); // 100 мс

        printf("[Родитель] Отправляем: \"%s\"\n", msg2);
        if (write(pipefd[1], msg2, strlen(msg2) + 1) == -1) {
            perror("write msg2");
        }

        // Третий пакет - демонстрация блокировки при переполнении
        // Размер буфера по умолчанию для pipe можно узнать через fcntl(F_GETPIPE_SZ)
        int pipe_size = fcntl(pipefd[1], F_GETPIPE_SZ);
        printf("[Родитель] Размер буфера канала: %d байт\n", pipe_size);
        
        // Пробуем отправить пакет больше половины буфера, чтобы увидеть эффект
        char big_msg[pipe_size / 2 + 100];
        memset(big_msg, 'X', sizeof(big_msg) - 1);
        big_msg[sizeof(big_msg) - 1] = '\0';
        
        printf("[Родитель] Пробуем отправить большой пакет (%ld байт)...\n", sizeof(big_msg));
        
        // В неблокирующем режиме мы бы получили EAGAIN, здесь же просто подождем
        ssize_t written = write(pipefd[1], big_msg, sizeof(big_msg));
        if (written == -1) {
            perror("[Родитель] Большой пакет не влез");
        } else {
            printf("[Родитель] Отправлено %ld байт\n", written);
        }

        close(pipefd[1]); // Закрываем запись, читатель получит EOF после последнего пакета
        wait(NULL);
        printf("[Родитель] Готово\n");
    }

    return 0;
}
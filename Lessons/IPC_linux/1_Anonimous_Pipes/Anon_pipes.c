#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2]; // Массив из двух файловых дескрипторов:
                   // pipefd[0] - для чтения из канала
                   // pipefd[1] - для записи в канал
    pid_t pid;
    char buffer[128];

    // 1. Создаем канал
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 2. Создаем процесс
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        // ---- Дочерний процесс (читатель) ----
        // Закрываем записывающий конец, он ему не нужен
        close(pipefd[1]);

        // Читаем данные из канала (ждем, пока родитель что-то напишет)
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Гарантированно завершаем строку
            printf("Дочерний процесс получил: %s", buffer);
        }

        // Закрываем читающий конец
        close(pipefd[0]);
        exit(EXIT_SUCCESS);

    } else { 
        // ---- Родительский процесс (писатель) ----
        // Закрываем читающий конец, он ему не нужен
        close(pipefd[0]);

        // Данные для отправки
        const char *message = "Привет из родительского процесса!\n";

        // Пишем в канал
        write(pipefd[1], message, strlen(message));

        // Закрываем записывающий конец (важно! дочерний read увидит EOF, если это закроется)
        close(pipefd[1]);

        // Ждем завершения дочернего процесса, чтобы избежать зомби
        wait(NULL);
        printf("Родитель: дочерний процесс завершен.\n");
    }

    return 0;
}
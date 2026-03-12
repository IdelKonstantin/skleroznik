#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Простой обработчик сигнала
void sigint_handler(int sig) {
    // NB: в обработчиках сигналов можно использовать только async-signal-safe функции
    // write() - безопасна, printf() - нет!
    write(STDOUT_FILENO, "\nА вот хрен!\n", 22);
}

int main() {
    // Устанавливаем обработчик для SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);
    
    printf("Программа запущена. Нажми Ctrl+C...\n");
    
    // Бесконечный цикл
    int counter = 0;
    while (1) {
        printf("Работаю... %d\n", ++counter);
        sleep(1);
    }
    
    return 0;
}
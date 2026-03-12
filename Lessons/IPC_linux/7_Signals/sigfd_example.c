#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/signalfd.h>

int main() {
    sigset_t mask;
    int sfd;
    struct signalfd_siginfo fdsi;
    
    // Блокируем сигналы, которые хотим получать через signalfd
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);
    
    // Блокируем эти сигналы (чтобы они не обрабатывались стандартно)
    sigprocmask(SIG_BLOCK, &mask, NULL);
    
    // Создаем signalfd
    sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        perror("signalfd");
        exit(EXIT_FAILURE);
    }
    
    printf("signalfd создан. Ждем сигналы (PID: %d)\n", getpid());
    printf("Отправь SIGINT, SIGQUIT или SIGTERM\n");
    
    while (1) {
        // Читаем сигнал как обычные данные из файлового дескриптора
        ssize_t s = read(sfd, &fdsi, sizeof(fdsi));
        if (s != sizeof(fdsi)) {
            perror("read");
            break;
        }
        
        printf("Получен сигнал %d от процесса %d\n", 
               fdsi.ssi_signo, fdsi.ssi_pid);
        
        if (fdsi.ssi_signo == SIGQUIT || fdsi.ssi_signo == SIGTERM) {
            printf("Выходим по сигналу\n");
            break;
        }
    }
    
    close(sfd);
    return 0;
}
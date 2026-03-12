#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define SHM_NAME "/my_simple_counter"

typedef struct {
    pthread_mutex_t mutex;
    int counter;
    pid_t last_writer;
} shared_data_t;

shared_data_t *shared = NULL;
int shm_fd = -1;

void cleanup(int sig) {
    printf("\nЧитатель %d завершается\n", getpid());
    if (shared != MAP_FAILED) {
        munmap(shared, sizeof(shared_data_t));
    }
    close(shm_fd);
    exit(0);
}

int main() {
    signal(SIGINT, cleanup);
    
    // Открываем существующую память (только чтение)
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open. Сначала запусти основной процесс");
        exit(EXIT_FAILURE);
    }
    
    shared = mmap(NULL, sizeof(shared_data_t), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    printf("Читатель %d: наблюдаю за счетчиком\n", getpid());
    
    int last_value = -1;
    
    while (1) {
        // Пытаемся захватить мьютекс (неблокирующий режим для чтения)
        if (pthread_mutex_trylock(&shared->mutex) == 0) {
            if (shared->counter != last_value) {
                printf("[%d] Счетчик = %d (последний писатель: %d)\n", 
                       getpid(), shared->counter, shared->last_writer);
                last_value = shared->counter;
            }
            pthread_mutex_unlock(&shared->mutex);
        }
        
        sleep(1);
    }
    
    return 0;
}
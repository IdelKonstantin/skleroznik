#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

#define SHM_NAME "/my_simple_counter"

// Структура в разделяемой памяти
typedef struct {
    pthread_mutex_t mutex;    // Мьютекс для синхронизации
    int counter;              // Общий счетчик
    pid_t last_writer;        // Кто последний писал
} shared_data_t;

shared_data_t *shared = NULL;
int shm_fd = -1;

void cleanup(int sig) {
    printf("\nПроцесс %d: завершаюсь...\n", getpid());
    
    if (shared != NULL && shared != MAP_FAILED) {
        munmap(shared, sizeof(shared_data_t));
    }
    
    if (shm_fd != -1) {
        close(shm_fd);
    }
    
    exit(0);
}

int main(int argc, char *argv[]) {
    // Устанавливаем обработчик Ctrl+C
    signal(SIGINT, cleanup);
    
    // 1. Создаем/открываем разделяемую память
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    // 2. Устанавливаем размер под нашу структуру
    if (ftruncate(shm_fd, sizeof(shared_data_t)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    
    // 3. Отображаем память в наш процесс
    shared = mmap(NULL, sizeof(shared_data_t), 
                  PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    // 4. Инициализируем мьютекс (только если мы первые)
    // Используем атомарную операцию для проверки флага инициализации
    static pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    // Пытаемся инициализировать мьютекс с атрибутом process-shared
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    
    // Пробуем инициализировать мьютекс в разделяемой памяти
    // Если он уже инициализирован - получим ошибку
    int ret = pthread_mutex_init(&shared->mutex, &attr);
    if (ret == 0) {
        // Мы первые - инициализируем счетчик
        shared->counter = 0;
        shared->last_writer = 0;
        printf("Первоначальная инициализация структуры (процесс %d)\n", getpid());
    } else if (ret == EBUSY) {
        printf("Мьютекс уже инициализирован, подключаемся к существующему (процесс %d)\n", getpid());
    } else {
        fprintf(stderr, "Ошибка инициализации мьютекса: %d\n", ret);
        cleanup(0);
    }
    
    pthread_mutexattr_destroy(&attr);
    
    printf("Процесс %d запущен. Нажми Ctrl+C для выхода.\n", getpid());
    printf("Текущее значение счетчика: %d\n\n", shared->counter);
    
    // 5. Работаем со счетчиком
    int local_counter = 0;
    
    while (1) {
        // Захватываем мьютекс
        pthread_mutex_lock(&shared->mutex);
        
        // --- Критическая секция ---
        shared->counter++;
        shared->last_writer = getpid();
        printf("Процесс %d: увеличил счетчик до %d (локально %d)\n", 
               getpid(), shared->counter, ++local_counter);
        // --- Конец критической секции ---
        
        pthread_mutex_unlock(&shared->mutex);
        
        // Спим случайное время
        sleep(rand() % 3 + 1);
    }
    
    return 0;
}
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/my_simple_counter"

int main() {
    if (shm_unlink(SHM_NAME) == 0) {
        printf("Разделяемая память %s удалена\n", SHM_NAME);
    } else {
        perror("shm_unlink");
    }
    
    return 0;
}
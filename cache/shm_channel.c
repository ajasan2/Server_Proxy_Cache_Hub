#include "shm_channel.h"

int shm_init(char* seg_name, size_t segsize, int new) {
    int shm_fd = -1;
    if (new) {
        shm_fd = shm_open(seg_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    } else {
        shm_fd = shm_open(seg_name, O_RDWR, S_IRUSR | S_IWUSR);
    }
    if(shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (new) {
        if(ftruncate(shm_fd, segsize) == -1) {
            perror("ftruncate");
            exit(EXIT_FAILURE);
        }
    }
    return shm_fd;
}


void* shm_attach(int shm_fd, size_t segsize) {
    void *shmp = mmap(NULL, segsize, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shmp == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    return shmp;
}


void shm_detach(void* shmp, size_t segsize) {
    if(munmap(shmp, segsize) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
}


void shm_destroy(char* seg_name) {
    if(shm_unlink(seg_name) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
}
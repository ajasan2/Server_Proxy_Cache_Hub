#ifndef _SHM_CHANNEL_H_
#define _SHM_CHANNEL_H_

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "pthread.h"

int shm_init(char* seg_name, size_t segsize, int new);
void* shm_attach(int shm_fd, size_t segsize);
void shm_detach(void *shmp, size_t segsize);
void shm_destroy(char* seg_name);

#endif
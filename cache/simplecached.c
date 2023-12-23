#include <stdio.h>
#include <unistd.h>
#include <printf.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <mqueue.h>

#include "cache-student.h"
#include "shm_channel.h"
#include "simplecache.h"


// CACHE_FAILURE
#if !defined(CACHE_FAILURE)
    #define CACHE_FAILURE (-1)
#endif 

#define MAX_CACHE_REQUEST_LEN 6200
#define MAX_SIMPLE_CACHE_QUEUE_SIZE 824  

// Globals
unsigned long int cache_delay;
mqd_t msgq;
steque_t work_q;
pthread_mutex_t work_mutex;
pthread_cond_t work_cond;

static void _sig_handler(int signo){
	if (signo == SIGTERM || signo == SIGINT) {
		/*IPC cleanup*/
        mq_close(msgq);
        mq_unlink("/request_channel");
        pthread_mutex_destroy(&work_mutex);
        pthread_cond_destroy(&work_cond);
        steque_destroy(&work_q);
        simplecache_destroy();
		exit(signo);
	}
}


void* worker_func(void* arg) {
    while(1) {

        // Redacted acquiring request

        // Access shared memory
        int shm_fd = shm_init(rqst->shm_name, rqst->shm_size, 0);
        request_t* shmp = shm_attach(shm_fd, rqst->shm_size);

        // Check if file exists
        int file_fd = simplecache_get((char *)(rqst->path));
        // Redacted cleanup if not a valid file

        // Send file size to proxy
        struct stat file_stat;
        if (fstat(file_fd, &file_stat) < 0) {
            perror("Find file size");
            exit(EXIT_FAILURE);
        }
        size_t file_len = (size_t)file_stat.st_size;

        sem_wait(&shmp->wsem);
        shmp->file_len = file_len;
        sem_post(&shmp->rsem);

        // Redacted sending file chunks through shared memory
    }
}

int main(int argc, char **argv) {
	int nthreads = 11;
	char *cachedir = "locals.txt";
	char option_char;

    // Redacted given command-line argument parsing
    
	/*Initialize cache*/
	if (simplecache_init(cachedir) != EXIT_SUCCESS) {
        fprintf(stderr, "Failed to initialize the cache.\n");
        return EXIT_FAILURE;
    }

	// Multithread setup
    pthread_t thread_pool[nthreads];
    steque_init(&work_q);
    pthread_mutex_init(&work_mutex, NULL);
    pthread_cond_init(&work_cond, NULL);
    for (int i = 0; i < nthreads; i++) {
        pthread_create(&thread_pool[i], NULL, worker_func, NULL);
    }

    // Redacted setting up message queue

    // Boss thread adds work as requests
    while(1) {
        
        // Redacted receiving request

        steque_node_t *node = malloc(sizeof(steque_node_t));
        node->item = rqst;
        
        pthread_mutex_lock(&work_mutex);
        steque_enqueue(&work_q, node);
        pthread_mutex_unlock(&work_mutex);
        pthread_cond_signal(&work_cond);
    }
}

#include <stdio.h>
#include <string.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <printf.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <stdlib.h>

#include "cache-student.h"
#include "gfserver.h"
#include "shm_channel.h"

// Globals
mqd_t msgq;
steque_t rqst_q;
pthread_mutex_t rq_mutex;
pthread_cond_t rq_cond;

// Redacted some global declarations

static void _sig_handler(int signo){
	if (signo == SIGTERM || signo == SIGINT){
		// Cleanup
        mq_close(msgq);
        mq_unlink("/request_channel");
        
        // Redacted shared memory cleanup

		gfserver_stop(&gfs);
		exit(signo);
	}
}

int main(int argc, char **argv) {
	int option_char = 0;
	char *server = "https://raw.githubusercontent.com/gt-cs6200/image_data";
	nsegments = 9;
	unsigned short port = 25466;
	unsigned short nworkerthreads = 35;
	size_t segsize = 5712;

    // Redacted given command-line argument parsing and memory size configurations

	// Initialize synchronization variables
	steque_init(&rqst_q);
	pthread_mutex_init(&rq_mutex, NULL);
	pthread_cond_init(&rq_cond, NULL);

    // Initialize shared memory setup
	for(int i = 0; i < nsegments; i++) {
		
        // Redacted segment initializations
        
        segment_arr[i] = rqst;
		steque_node_t* node = (steque_node_t *) malloc(sizeof(steque_node_t));
		node->item = rqst;
		steque_enqueue(&rqst_q, node);
	}

    // Open message queue with cache server
    do {
        msgq = mq_open("/request_channel", O_RDWR);
        if (msgq < 0) {
            sleep(5);
        }
    } while (msgq == -1);

	// Initialize server structure here
	gfserver_init(&gfs, nworkerthreads);
	gfserver_setopt(&gfs, GFS_PORT, port);
	gfserver_setopt(&gfs, GFS_WORKER_FUNC, handle_with_cache);

	// Invoke the framework - this is an infinite loop and will not return
	gfserver_serve(&gfs);
}

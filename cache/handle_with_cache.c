#include "gfserver.h"
#include "cache-student.h"
#include "shm_channel.h"

request_t* get_request() {
    pthread_mutex_lock(&rq_mutex);
    while(steque_isempty(&rqst_q)) {
        pthread_cond_wait(&rq_cond, &rq_mutex);
    }
    steque_node_t* node = steque_pop(&rqst_q);
    pthread_mutex_unlock(&rq_mutex);
    request_t* rqst = node->item;
    free(node);
    return rqst;
}

void return_shm_seg(request_t* rqst) {
    pthread_mutex_lock(&rq_mutex);
    steque_node_t* node = (steque_node_t *) malloc(sizeof(steque_node_t));
    node->item = rqst;
    steque_enqueue(&rqst_q, node);
    pthread_mutex_unlock(&rq_mutex);
    pthread_cond_signal(&rq_cond);
}

ssize_t handle_with_cache(gfcontext_t *ctx, const char *path, void* arg){
    request_t* rqst = get_request();
    request_t* shmp = rqst->shmp;
    snprintf(rqst->path, MAX_REQUEST_LEN, "%s", path);
    
    // send request to cache
    if (mq_send(msgq, (char *)rqst, sizeof(request_t), 0) == -1) {
        perror("Send request");
        exit(EXIT_FAILURE);
    }

    // find out file size
    sem_wait(&shmp->rsem);
    size_t file_len = shmp->file_len;
    sem_post(&shmp->wsem);

    // Redacted request validation
    
    // Redacted sending file chunks to client through shared memory

    return_shm_seg(rqst);
	return tot_rbytes;
}


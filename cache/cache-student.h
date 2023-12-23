#ifndef __CACHE_STUDENT_H__836
#define __CACHE_STUDENT_H__836

#include <mqueue.h>
#include <semaphore.h>
#include "steque.h"
#include "gfserver.h"

extern mqd_t msgq;
extern steque_t rqst_q;
extern pthread_mutex_t rq_mutex;
extern pthread_cond_t rq_cond;

typedef struct {
    int shm_fd;
    void* shmp;
    char shm_name[8];
    char path[MAX_REQUEST_LEN];
    size_t file_len;
    sem_t rsem;
    sem_t wsem;

    // Redacted part of struct definition

} request_t;

request_t* get_request();
void return_shm_seg(request_t* rqst);


#endif // __CACHE_STUDENT_H__836
#ifndef TASKS_H
#define TASKS_H

#include "mandel-tiles-graphic.h"
#include <memory>
#include <pthread.h>
#include <queue>

#define QUEUE_TYPE std::queue<std::shared_ptr<fractal_param_t>>
#define MAX_QUEUE_WORKERS_RATIO 4

// Globals for thread sync and control
extern pthread_cond_t cook_needed;
extern pthread_cond_t pot_filled;
extern pthread_mutex_t queue_access;



typedef struct cook_data {
    std::shared_ptr<QUEUE_TYPE> task_queue;     
     
    int n_threads;      // How many worker threads will be created
    int queue_size;     // How many slots the queue will have
    std::string filename;

    int created_tasks;
    int completed_tasks;
} cook_data;

void* cook_thread(void* data);

#endif /* TASKS_H */
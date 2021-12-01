#ifndef WORKER_H
#define WORKER_H

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "cook.h"

typedef struct worker_data {
    std::shared_ptr<QUEUE_TYPE> task_queue;
    int qtd_worker_jobs;    // Statistics

    // Constructor
    worker_data(std::shared_ptr<QUEUE_TYPE> queue_ptr);
} worker_data;

void* worker_thread(void* data);

#endif /* WORKER_H */
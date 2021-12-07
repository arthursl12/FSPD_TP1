#ifndef WORKER_H
#define WORKER_H

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "cook.h"

typedef struct worker_data {
    std::shared_ptr<QUEUE_TYPE> task_queue;

    // Statistics
    int qtd_worker_jobs;    
    double total_job_time;
    int empty_queue;
    std::shared_ptr<std::vector<double>> job_times;

    // Constructor
    worker_data(std::shared_ptr<QUEUE_TYPE> queue_ptr);
} worker_data;

void* worker_thread(void* data);

#endif /* WORKER_H */
#ifndef COOK_H
#define COOK_H

#include "mandel-tiles-graphic.h"
#include <memory>
#include <pthread.h>
#include <queue>

#define QUEUE_TYPE std::deque<std::shared_ptr<fractal_param_t>>
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

    // Constructor
    cook_data(int n_threads, int queue_size, std::string filename);
} cook_data;

void pthread_structs_init();
void pthread_structs_destroy();

void* cook_thread(void* data);
void createEOWList(std::vector<std::shared_ptr<fractal_param_t>>& output, 
                   int n_threads);
bool equalCookData(cook_data& cd1, cook_data& cd2);
bool equalQueues(QUEUE_TYPE& q1, QUEUE_TYPE& q2);

double stdev(std::vector<double>& numbers);
double mean(std::vector<double>& numbers);


#endif /* COOK_H */
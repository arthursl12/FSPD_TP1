#include "worker.h"

worker_data::worker_data(std::shared_ptr<QUEUE_TYPE> queue_ptr)
    : qtd_worker_jobs(0)
{
    this->task_queue = queue_ptr;
}

void* worker_thread(void* data){
    // Getting the parameters passed
    worker_data* args = (worker_data*) data;

    bool foundEOW = false;
    while (!foundEOW){
        // Get task from first position of queue
        pthread_mutex_lock(&queue_access);
        while (args->task_queue->empty()){
            // Empty queue, must wake cook
            pthread_cond_signal(&cook_needed);
            pthread_cond_wait(&pot_filled, &queue_access);
        }
        std::shared_ptr<fractal_param_t> task = args->task_queue->front();
        args->task_queue->pop_front();
        pthread_mutex_unlock(&queue_access);

        // Do work out of critical section
        if (isEOW(task)){
            foundEOW = true;
        }else{
            // Do work
            // TODO: get times and store it somehow
            fractal(task.get());
        }
    }

    // Thread can end now, as it found EOW task
    int* myInt = (int*) malloc(sizeof(int));
    *myInt = EXIT_SUCCESS;
    return (void*) myInt;
    
}

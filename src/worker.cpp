#include "worker.h"
#include <sys/time.h>
#include <iostream>

worker_data::worker_data(std::shared_ptr<QUEUE_TYPE> queue_ptr)
    : qtd_worker_jobs(0), total_job_time(0)
{
    this->task_queue = queue_ptr;
    this->job_times = std::make_shared<std::vector<double>>();
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
            std::cout << "Found EOW " << pthread_self() << std::endl;
        }else{
            // Do work
            struct timeval  tv1, tv2;

            gettimeofday(&tv1, NULL);
            fractal(task.get());
            gettimeofday(&tv2, NULL);

            args->qtd_worker_jobs += 1;
            double total_time = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                                (double) (tv2.tv_sec - tv1.tv_sec);
            args->total_job_time += total_time;
            args->job_times->push_back(total_time);
            std::cout << "Task completed in " << total_time << " seconds." << std::endl;
        }
    }

    // Thread can end now, as it found EOW task
    int* myInt = (int*) malloc(sizeof(int));
    *myInt = EXIT_SUCCESS;
    return (void*) myInt;
    
}

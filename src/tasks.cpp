#include "tasks.h"
#include "parsing.h"

#include <iostream>

/*

pthread_cond_t condition;
pthread_cond_init ( &condition, &attr );
pthread_cond_destroy ( &condition); 
pthread_cond_wait ( &condition, &mutex );
pthread_cond_signal ( &condition );        /* sinaliza um   */

pthread_cond_t cook_needed;
pthread_cond_t pot_filled;
pthread_mutex_t queue_access;

void* cook_thread(void* data){
    // Getting the parameters passed
    cook_data* args = (cook_data*) data;

    // Check for ratio workers-queue
    if (args->n_threads > 0 && (args->queue_size > 4*args->n_threads)){
        std::cout << "Queue too long for the amount of threads created";
        std::cout << std::endl;

        int* myInt = (int*) malloc(sizeof(int));
        *myInt = EXIT_FAILURE;
        return (void*) myInt;
    }

    // Clear queue
    args->task_queue = std::make_shared<QUEUE_TYPE>();

    // Open file
    std::ifstream infile(args->filename);
    std::vector<std::shared_ptr<fractal_param_t>> output;

    // First fill of the pot
    pthread_mutex_lock(&queue_access);      // Lock queue acess
    bool eof = readFromFile(output, infile, args->queue_size);
    for (auto fp_ptr : output){
        args->task_queue->push(fp_ptr);
        args->created_tasks++;
    }

    bool finishied = false;
    while (args->n_threads > 0 && !finishied){
        // TODO: handle eof
        if (eof){
            throw "EOF reached!";
        }
        
        pthread_cond_wait(&cook_needed, &queue_access); // Waits and release mutex
        // Awakes and mutex is locked in this thread again
        

        eof = readFromFile(output, infile, args->queue_size);


        pthread_cond_signal(&pot_filled); 
    }

    // Release queue for other uses later
    pthread_mutex_unlock(&queue_access);
    return EXIT_SUCCESS;
}

void createEOWList(std::vector<std::shared_ptr<fractal_param_t>>& output, 
                   int n_threads){
                       /* TODO */
}
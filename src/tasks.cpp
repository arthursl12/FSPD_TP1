#include "tasks.h"
#include "parsing.h"

#include <iostream>

pthread_cond_t cook_needed;
pthread_cond_t pot_filled;
pthread_mutex_t queue_access;

void pthread_structs_init(){
    // Initialize pthread structures
    pthread_cond_init (&cook_needed, NULL);
    pthread_cond_init (&pot_filled, NULL);
    pthread_mutex_init(&queue_access, NULL);
}

void pthread_structs_destroy(){
    // Destroy pthread structures
	pthread_cond_destroy(&pot_filled);
	pthread_cond_destroy(&cook_needed);
	pthread_mutex_destroy(&queue_access);
}

void* cook_thread(void* data){
    // Getting the parameters passed
    cook_data* args = (cook_data*) data;

    // Check for ratio workers-queue
    if ((args->n_threads > 0) && 
        (args->queue_size > MAX_QUEUE_WORKERS_RATIO*args->n_threads))
    {
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
                   int n_threads)
{
    for (int i = 0; i < n_threads; i++){
        output.push_back(generateEOW());
    }
}

/*
Fills attributes of cook_data struct using passed parameters. 

If 'queue_size' is -1, it will be set to 4 times the qtd of threads
(factor defined in 'tasks.h')
*/
void cookDataConstructor(cook_data& out, int n_threads, 
                         int queue_size, std::string filename)
{
    out.completed_tasks = 0;
    out.created_tasks = 0;
    out.filename = filename;
    out.n_threads = n_threads;
    if (queue_size == -1){
        out.queue_size = MAX_QUEUE_WORKERS_RATIO*n_threads;
    }else{
        out.queue_size = queue_size;
    }
    out.task_queue = std::make_shared<QUEUE_TYPE>();
}

/*
Checks if two cook_data structures are equal, i.e., they have the same 
attribute's values. For complex structures (like STL queues), they are compared
element-to-element.
*/
bool equalCookData(cook_data& cd1, cook_data& cd2){
    // Verify primitive attributes
    bool simpleEqual = true;
    if ((cd1.completed_tasks != cd2.completed_tasks) || 
        (cd1.created_tasks != cd2.created_tasks) ||
        (cd1.filename != cd2.filename) || 
        (cd1.n_threads != cd2.n_threads) ||
        (cd1.queue_size != cd2.queue_size))
    {
        simpleEqual = false;
    }
    if (!simpleEqual){
        return false;
    }

    // Verify queues
    // 1) Sizes must be same
    bool queuesEqual = true;
    queuesEqual = queuesEqual && \
                    (cd1.task_queue->size() == cd2.task_queue->size());
    if (!queuesEqual){
        return false;
    }
    
    // 2) Element-wise must be same
    // The sizes must be the same to get here
    /* TODO */
    return true;
    

}
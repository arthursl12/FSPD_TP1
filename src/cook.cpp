#include "cook.h"
#include "parsing.h"
#include "worker.h"

#include <assert.h>     
#include <iostream>
#include <cmath>

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

double stdev(std::vector<double>& numbers){
    double _mean = mean(numbers);

    double stdev = 0;
    for (double i : numbers){
        stdev += pow(i - _mean, 2);
    }
    stdev = stdev / numbers.size();
    stdev = sqrt(stdev);
    return stdev;
}


double mean(std::vector<double>& numbers){
    double sum = 0;
    for (double i : numbers){
        sum += i;
    }
    return sum / numbers.size();
}

/*
Fills attributes of cook_data struct using passed parameters. 

If 'queue_size' is -1, it will be set to 4 times the qtd of threads
(factor defined in 'cook.h')
*/
cook_data::cook_data(int n_threads, int queue_size, std::string filename)
    : n_threads(n_threads), filename(filename), 
      completed_tasks(0), created_tasks(0)
{
    if (queue_size == -1){
        this->queue_size = MAX_QUEUE_WORKERS_RATIO*n_threads;
    }else{
        this->queue_size = queue_size;
    }
    this->task_queue = std::make_shared<QUEUE_TYPE>();
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
        args->task_queue->push_back(fp_ptr);
        args->created_tasks++;
    }
    pthread_mutex_unlock(&queue_access);

    // Create worker threads
    std::vector<std::shared_ptr<pthread_t>> workers;
    std::vector<std::shared_ptr<worker_data>> workers_args;
    for (int i = 0; i < args->n_threads; i++){
        std::shared_ptr<pthread_t> worker;
        worker = std::make_shared<pthread_t>();
        workers.push_back(worker);

        std::shared_ptr<worker_data> data;
        data = std::make_shared<worker_data>(args->task_queue);
        workers_args.push_back(data);
    }

    // Start threads
    int ret;
    for (int i = 0; i < args->n_threads; i++){
        ret = pthread_create(workers[i].get(), NULL, worker_thread, 
                             (void*)workers_args[i].get());
        if(ret){ throw "Failed to create thread"; }
    }

    bool finishied = false;
    bool eof_lastfill = eof;          // if last fill reached eof
    int EOW_left = args->n_threads;   // how many EOW must be put into queue yet
    while (args->n_threads > 0 && !finishied){
        pthread_mutex_lock(&queue_access);
        pthread_cond_wait(&cook_needed, &queue_access); // Waits and release mutex
        // Awakes and mutex is locked in this thread again
        if (!eof_lastfill){
            // There are still tasks in the file
            eof = readFromFile(output, infile, args->queue_size);
            // If output is empty, it will skip the loop
            for (auto task : output){
                // Guaranteed: output.size() <= queue.size() 
                args->task_queue->push_back(task);
                args->created_tasks++;
            }

            if (eof){
                // We reached eof this fill, no need to read file more
                eof_lastfill = true;
            }
        }
        int queue_free_spaces = args->queue_size - args->task_queue->size();
        if (queue_free_spaces >= EOW_left){
            // Ideal situation: we can put all EOW's left in this refill
            // The ideal case is when EOW_left == args->n_threads but works 
            // it's not true as well
            for (int i = 0; i < EOW_left; i++){
                args->task_queue->push_back(generateEOW());
                EOW_left -= 1;
            }
        }else if (queue_free_spaces > 0){
            // We can put only some EOW's left in this refill
            // We'll do it and wait for other runs
            for (int i = 0; i < queue_free_spaces; i++){
                args->task_queue->push_back(generateEOW());
                EOW_left -= 1;
            }
        }

        pthread_cond_broadcast(&pot_filled);
        pthread_mutex_unlock(&queue_access);

        if (EOW_left == 0){
            // The EOWs in queue are the last objective for the cook, we can
            // end the loop
            finishied = true;
        }
    }

    // Wait for worker threads to end
    int total_works_done = 0;
    for (int i = 0; i < args->n_threads; i++){
        total_works_done += workers_args[i]->qtd_worker_jobs;
        std::cout << *workers[i] << ": " << workers_args[i]->qtd_worker_jobs << std::endl;
    }
    args->completed_tasks = total_works_done;

    // TODO: compute statistics (remember to ignore 0 threads)
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
Checks if two task queues (STL deque) are equal, content-wise as well.  
*/
bool equalQueues(QUEUE_TYPE& q1, QUEUE_TYPE& q2){
    // Verify queues
    // 1) Sizes must be same
    bool queuesEqual = true;
    queuesEqual = queuesEqual && \
                    (q1.size() == q2.size());
    if (!queuesEqual){
        return false;
    }
    
    // 2) Element-wise must be same
    // The sizes must be the same to get here
    for (uint i = 0; i < q1.size(); i++){
        // Deque access time is constant
        if(!fractalParamComparator(q1.at(i), 
                                   q2.at(i)))
        {
            return false;
        }
    }
    return true;
}

/*
Checks if two cook_data structures are equal, i.e., they have the same 
attribute's values. For complex structures (like STL queues), they are compared
element-to-element.
*/
bool equalCookData(cook_data& cd1, cook_data& cd2){
    // Verify primitive attributes are equal
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

    // Verify queues are equal
    if (!equalQueues(*cd1.task_queue, *cd2.task_queue)){
        return false;
    }

    return true;
    

}
#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "cook.h"

#include <pthread.h>
#include <queue>


// TEST_CASE("Cook and N workers - general case "){
//     pthread_structs_init();

//     int ret;
//     pthread_t cook;
//     cook_data cook_args;

//     // Fill arguments (with mutex)
//     pthread_mutex_lock(&queue_access);
//     cook_args.task_queue = std::make_shared<QUEUE_TYPE>();
//     cook_args.n_threads = 2;
//     cook_args.queue_size = 8;
//     cook_args.filename = "mandelbrot_tasks/t";
//     pthread_mutex_unlock(&queue_access);

//     // Create cook thread
//     ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
//     if(ret){ throw "Failed to create thread"; }
    
//     // Wait for it to finish
//     ret = pthread_join(cook, NULL);

//     // Test results (with mutex)
//     pthread_mutex_lock(&queue_access);
//     CHECK(cook_args.task_queue->empty());
//     CHECK(cook_args.task_queue->size() == 0);
//     CHECK(cook_args.created_tasks == 64);
//     CHECK(cook_args.completed_tasks == 64);
//     pthread_mutex_unlock(&queue_access);

//     pthread_structs_destroy();
// }

// TEST_CASE("Cook and N workers - equal size queue "){
//     pthread_structs_init();

//     int ret;
//     pthread_t cook;
//     cook_data cook_args;

//     // Fill arguments (with mutex)
//     pthread_mutex_lock(&queue_access);
//     cook_args.task_queue = std::make_shared<QUEUE_TYPE>();
//     cook_args.n_threads = 4;
//     cook_args.queue_size = 4;
//     cook_args.filename = "mandelbrot_tasks/t";
//     pthread_mutex_unlock(&queue_access);

//     // Create cook thread
//     ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
//     if(ret){ throw "Failed to create thread"; }
    
//     // Wait for it to finish
//     ret = pthread_join(cook, NULL);

//     // Test results (with mutex)
//     pthread_mutex_lock(&queue_access);
//     CHECK(cook_args.task_queue->empty());
//     CHECK(cook_args.task_queue->size() == 0);
//     CHECK(cook_args.created_tasks == 64);
//     CHECK(cook_args.completed_tasks == 64);
//     pthread_mutex_unlock(&queue_access);

//     pthread_structs_destroy();
// }

TEST_CASE("Worker/Queue ratio test "){
    pthread_structs_init();

    int ret;
    pthread_t cook;

    // Fill arguments (with mutex)
    pthread_mutex_lock(&queue_access);
    cook_data cook_args(1, 5, "mandelbrot_tasks/t");
    CHECK(cook_args.task_queue->empty());
    pthread_mutex_unlock(&queue_access);

    // Create cook thread
    ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    void* tmp_ret;
    ret = pthread_join(cook, &tmp_ret);
    ret = *((int*) tmp_ret);
    free(tmp_ret);

    // Check failure exit (error ratio)
    CHECK(ret == EXIT_FAILURE);

    pthread_structs_destroy();
}
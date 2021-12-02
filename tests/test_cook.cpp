#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "cook.h"

#include <pthread.h>
#include <queue>

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

TEST_CASE("Cook and N workers - File EOW equal to size "){
    /*
    After refill, only EOW will be found in queue
    */
    pthread_structs_init();

    int ret;
    pthread_t cook;

    // Fill arguments (with mutex)
    pthread_mutex_lock(&queue_access);
    cook_data cook_args(2,8,"mandelbrot_tasks/z2");
    pthread_mutex_unlock(&queue_access);

    // Create cook thread
    ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    ret = pthread_join(cook, NULL);

    // Test results (with mutex)
    pthread_mutex_lock(&queue_access);
    CHECK(cook_args.task_queue->empty());
    CHECK(cook_args.task_queue->size() == 0);
    CHECK(cook_args.created_tasks == 8);
    CHECK(cook_args.completed_tasks == 8);
    pthread_mutex_unlock(&queue_access);

    pthread_structs_destroy();
}

TEST_CASE("Cook and N workers - Refills and then EOW "){
    /*
    Some refills and then an EOW
    */
    pthread_structs_init();

    int ret;
    pthread_t cook;

    // Fill arguments (with mutex)
    pthread_mutex_lock(&queue_access);
    cook_data cook_args(2,3,"mandelbrot_tasks/z2");
    pthread_mutex_unlock(&queue_access);

    // Create cook thread
    ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    ret = pthread_join(cook, NULL);

    // Test results (with mutex)
    pthread_mutex_lock(&queue_access);
    CHECK(cook_args.task_queue->empty());
    CHECK(cook_args.task_queue->size() == 0);
    CHECK(cook_args.created_tasks == 8);
    CHECK(cook_args.completed_tasks == 8);
    pthread_mutex_unlock(&queue_access);

    pthread_structs_destroy();
}

TEST_CASE("Cook and N workers - Refills and then EOW 2 "){
    /*
    Some refills and then an EOW
    */
    pthread_structs_init();

    int ret;
    pthread_t cook;

    // Fill arguments (with mutex)
    pthread_mutex_lock(&queue_access);
    cook_data cook_args(2,4,"mandelbrot_tasks/z2");
    pthread_mutex_unlock(&queue_access);

    // Create cook thread
    ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    ret = pthread_join(cook, NULL);

    // Test results (with mutex)
    pthread_mutex_lock(&queue_access);
    CHECK(cook_args.task_queue->empty());
    CHECK(cook_args.task_queue->size() == 0);
    CHECK(cook_args.created_tasks == 8);
    CHECK(cook_args.completed_tasks == 8);
    pthread_mutex_unlock(&queue_access);

    pthread_structs_destroy();
}

TEST_CASE("Cook and N workers - Some EOW, more after refill "){
    /*
    After refill, some EOW will be found in queue, another refill is needed
    */
    pthread_structs_init();

    int ret;
    pthread_t cook;

    // Fill arguments (with mutex)
    pthread_mutex_lock(&queue_access);
    cook_data cook_args(3,3,"mandelbrot_tasks/z2");
    pthread_mutex_unlock(&queue_access);

    // Create cook thread
    ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    ret = pthread_join(cook, NULL);

    // Test results (with mutex)
    pthread_mutex_lock(&queue_access);
    CHECK(cook_args.task_queue->empty());
    CHECK(cook_args.task_queue->size() == 0);
    CHECK(cook_args.created_tasks == 8);
    CHECK(cook_args.completed_tasks == 8);
    pthread_mutex_unlock(&queue_access);

    pthread_structs_destroy();
}


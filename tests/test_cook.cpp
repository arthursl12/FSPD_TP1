#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "tasks.h"

#include <pthread.h>
#include <queue>

TEST_CASE("Cook and 0 workers "){
    // Initialize pthread structures
    pthread_cond_init (&cook_needed, NULL);
    pthread_cond_init (&pot_filled, NULL);
    pthread_mutex_init(&queue_access, NULL);

    int ret;
    pthread_t cook;
    cook_data cook_args;

    // Fill arguments (with mutex)
    pthread_mutex_lock(&queue_access);
    cook_args.task_queue = std::make_shared<QUEUE_TYPE>();
    cook_args.n_threads = 0;
    cook_args.queue_size = 5;
    cook_args.filename = "mandelbrot_tasks/t";
    CHECK(cook_args.task_queue->empty());
    pthread_mutex_unlock(&queue_access);

    // Create cook thread
    ret = pthread_create(&cook, NULL, cook_thread, (void *)&cook_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    ret = pthread_join(cook, NULL);

    // Test results (with mutex)
    pthread_mutex_lock(&queue_access);
    CHECK_FALSE(cook_args.task_queue->empty());
    CHECK(cook_args.task_queue->size() == 5);
    CHECK(cook_args.created_tasks == 5);
    CHECK(cook_args.completed_tasks == 0);
    pthread_mutex_unlock(&queue_access);

    // Destroy pthread structures
	pthread_cond_destroy(&pot_filled);
	pthread_cond_destroy(&cook_needed);
	pthread_mutex_destroy(&queue_access);
}

TEST_CASE("Worker/Queue ratio test "){
    // Initialize pthread structures
    pthread_cond_init (&cook_needed, NULL);
    pthread_cond_init (&pot_filled, NULL);
    pthread_mutex_init(&queue_access, NULL);

    int ret;
    pthread_t cook;
    cook_data cook_args;

    // Fill arguments (with mutex)
    pthread_mutex_lock(&queue_access);
    cook_args.task_queue = std::make_shared<QUEUE_TYPE>();
    cook_args.n_threads = 1;
    cook_args.queue_size = 5;
    cook_args.filename = "mandelbrot_tasks/t";
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
    CHECK(ret == EXIT_FAILURE);

    // Destroy pthread structures
	pthread_cond_destroy(&pot_filled);
	pthread_cond_destroy(&cook_needed);
	pthread_mutex_destroy(&queue_access);
}

TEST_CASE("Create EOWList"){
    std::vector<std::shared_ptr<fractal_param_t>> output;

    SUBCASE("0"){
        CHECK(output.empty());
        createEOWList(output, 0);
        CHECK(output.empty());
    }
    SUBCASE("1"){
        CHECK(output.empty());
        createEOWList(output, 1);
        CHECK_FALSE(output.empty());
        CHECK(output.empty());

    }
    


}
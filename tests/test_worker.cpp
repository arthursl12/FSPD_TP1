#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "cook.h"
#include "worker.h"

#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <sys/time.h>

TEST_CASE("Worker - EOW only queue"){
    pthread_structs_init();
    
    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(generateEOW());
    worker_data worker_args(q1);
    pthread_mutex_unlock(&queue_access);

    // Create worker thread
    int ret;
    pthread_t worker;
    ret = pthread_create(&worker, NULL, worker_thread, (void *)&worker_args);
    if(ret){ throw "Failed to create thread"; }

    // Wait for it to finish
    void* tmp_ret;
    ret = pthread_join(worker, &tmp_ret);
    ret = *((int*) tmp_ret);
    free(tmp_ret);

    // Check sucess exit
    CHECK(ret == EXIT_SUCCESS);

    // Check equal queues
    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    CHECK(equalQueues(*worker_args.task_queue, q2));
    pthread_mutex_unlock(&queue_access);

    CHECK(worker_args.qtd_worker_jobs == 0);

    pthread_structs_destroy();
}

TEST_CASE("Worker - Simple queue"){
    pthread_structs_init();

    // Fill queue
    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(generateEOW());
    worker_data worker_args(q1);
    pthread_mutex_unlock(&queue_access);
    
    // Create worker thread
    int ret;
    pthread_t worker;
    ret = pthread_create(&worker, NULL, worker_thread, (void *)&worker_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    void* tmp_ret;
    ret = pthread_join(worker, &tmp_ret);
    ret = *((int*) tmp_ret);
    free(tmp_ret);

    // Check sucess exit
    CHECK(ret == EXIT_SUCCESS);

    // Check equal queues
    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    CHECK(equalQueues(*worker_args.task_queue, q2));
    pthread_mutex_unlock(&queue_access);

    CHECK(worker_args.qtd_worker_jobs == 1);

    pthread_structs_destroy();
}

TEST_CASE("Worker - Empty queue"){
    pthread_structs_init();

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    worker_data worker_args(q1);
    pthread_mutex_unlock(&queue_access);
    
    // Create cook thread
    int ret;
    pthread_t worker;
    ret = pthread_create(&worker, NULL, worker_thread, (void *)&worker_args);
    if(ret){ throw "Failed to create thread"; }
    
    // 'Cook' refilling pot with EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    CHECK(q1->empty());
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    q1->push_back(generateEOW());
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);
    
    // Wait for it to finish
    sleep(1);

    // Check if it has finishied, 
    // to avoid infinite loops in testing environment
    ret = pthread_kill(worker, 0);
    if (ret == ESRCH){
        // No such process, i.e., thread has finishied
    }else{
        throw "Thread still running";
    }

    // void* tmp_ret;
    // ret = pthread_join(worker, &tmp_ret);
    // ret = *((int*) tmp_ret);
    // free(tmp_ret);

    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    CHECK(equalQueues(*worker_args.task_queue, q2));
    pthread_mutex_unlock(&queue_access);

    CHECK(worker_args.qtd_worker_jobs == 0);

    pthread_structs_destroy();
}

TEST_CASE("Worker - Refill queue"){
    pthread_structs_init();

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    worker_data worker_args(q1);
    pthread_mutex_unlock(&queue_access);
    
    // Create cook thread
    int ret;
    pthread_t worker;
    ret = pthread_create(&worker, NULL, worker_thread, (void *)&worker_args);
    if(ret){ throw "Failed to create thread"; }
    
    // 'Cook' refilling pot with more tasks
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    CHECK(q1->empty());
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    q1->push_back(string2fractalparam("160 120 80 60 0.2709202500 0.0047492500 0.2709203750 0.0047493750"));
    q1->push_back(string2fractalparam("160 120 80 60 0.2709202500 0.0047492500 0.2709203750 0.0047493750"));
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);

    // 'Cook' refilling pot with EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    CHECK(q1->empty());
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    q1->push_back(generateEOW());
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);
    
    // Wait for it to finish
    sleep(1);

    // Check if it has finishied, 
    // to avoid infinite loops in testing environment
    ret = pthread_kill(worker, 0);
    if (ret == ESRCH){
        // No such process, i.e., thread has finishied
    }else{
        throw "Thread still running";
    }

    // void* tmp_ret;
    // ret = pthread_join(worker, &tmp_ret);
    // ret = *((int*) tmp_ret);
    // free(tmp_ret);

    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(q1->empty());
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    CHECK(equalQueues(*worker_args.task_queue, q2));
    pthread_mutex_unlock(&queue_access);

    CHECK(worker_args.qtd_worker_jobs == 5);

    pthread_structs_destroy();
}

TEST_CASE("Worker - Refill queue including EOW"){
    pthread_structs_init();

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    worker_data worker_args(q1);
    pthread_mutex_unlock(&queue_access);
    
    // Create cook thread
    int ret;
    pthread_t worker;
    ret = pthread_create(&worker, NULL, worker_thread, (void *)&worker_args);
    if(ret){ throw "Failed to create thread"; }
    
    // 'Cook' refilling pot with more tasks and a final EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    CHECK(q1->empty());
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    q1->push_back(string2fractalparam("160 120 80 60 0.2709202500 0.0047492500 0.2709203750 0.0047493750"));
    q1->push_back(string2fractalparam("160 120 80 60 0.2709202500 0.0047492500 0.2709203750 0.0047493750"));
    q1->push_back(generateEOW());
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);

    // Wait for it to finish
    sleep(1);

    // Check if it has finishied, 
    // to avoid infinite loops in testing environment
    ret = pthread_kill(worker, 0);
    if (ret == ESRCH){
        // No such process, i.e., thread has finishied
    }else{
        throw "Thread still running";
    }

    // void* tmp_ret;
    // ret = pthread_join(worker, &tmp_ret);
    // ret = *((int*) tmp_ret);
    // free(tmp_ret);

    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(q1->empty());
    CHECK(equalQueues(*worker_args.task_queue, *q1));
    CHECK(equalQueues(*worker_args.task_queue, q2));
    pthread_mutex_unlock(&queue_access);

    CHECK(worker_args.qtd_worker_jobs == 5);

    pthread_structs_destroy();
}

/* TODO: statistics testing */
TEST_CASE("Worker - Statistics 1 task"){
    pthread_structs_init();

    // Fill queue
    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(generateEOW());
    worker_data worker_args(q1);
    pthread_mutex_unlock(&queue_access);
    
    // Create worker thread
    int ret;
    pthread_t worker;
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    ret = pthread_create(&worker, NULL, worker_thread, (void *)&worker_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    void* tmp_ret;
    ret = pthread_join(worker, &tmp_ret);
    gettimeofday(&tv2, NULL);
    ret = *((int*) tmp_ret);
    free(tmp_ret);

    CHECK(worker_args.qtd_worker_jobs == 1);

    double total_time = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                        (double) (tv2.tv_sec - tv1.tv_sec);
    CHECK(worker_args.total_job_time < total_time);
    CHECK(worker_args.total_job_time > 0);
    pthread_structs_destroy();
}

TEST_CASE("Worker - Statistics"){
    pthread_structs_init();

    // Fill queue
    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(generateEOW());
    worker_data worker_args(q1);
    pthread_mutex_unlock(&queue_access);
    
    // Create worker thread
    int ret;
    pthread_t worker;
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);
    ret = pthread_create(&worker, NULL, worker_thread, (void *)&worker_args);
    if(ret){ throw "Failed to create thread"; }
    
    // Wait for it to finish
    void* tmp_ret;
    ret = pthread_join(worker, &tmp_ret);
    gettimeofday(&tv2, NULL);
    ret = *((int*) tmp_ret);
    free(tmp_ret);

    CHECK(worker_args.qtd_worker_jobs == 3);

    double total_time = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                        (double) (tv2.tv_sec - tv1.tv_sec);
    CHECK(worker_args.total_job_time < total_time);
    CHECK(worker_args.total_job_time > 0);
    pthread_structs_destroy();
}
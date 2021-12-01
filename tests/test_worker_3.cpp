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

TEST_CASE("4 Workers - Queue 16 - Two refills for EOW"){
    /*
    2 threads working
    queue of size 8
    when refilled, queue has more jobs and one last EOW
    when refilled again, queue has the others EOW's
    */
    pthread_structs_init();

    int N = 4;
    int queue_size = MAX_QUEUE_WORKERS_RATIO*N;

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    for (int i = 0; i < queue_size; i++){
        q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    }
    pthread_mutex_unlock(&queue_access);

    // Create worker threads
    int ret;
    pthread_t* workers = (pthread_t*) malloc(N*sizeof(pthread_t));
    if (workers == NULL){ throw "malloc workers"; exit(1); }
    worker_data* workers_args = (worker_data*) malloc(N*sizeof(worker_data));
    if (workers_args == NULL){ throw "malloc args"; exit(1); }

    for (int i = 0; i < N; i++){
        std::cout << "i = " << i << std::endl;

        workers_args[i] = worker_data(q1);
    }
    for (int i = 0; i < N; i++){
        ret = pthread_create(&workers[i], NULL, worker_thread, 
                             (void*)&workers_args[i]);
        if(ret){ throw "Failed to create thread"; }
    }

    // 'Cook' refilling pot with more tasks and a final EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    CHECK(q1->empty());
    for (int i = 0; i < N; i++){
        CHECK(equalQueues(*(workers_args[i].task_queue), *q1));
    }
    for (int i = 0; i < queue_size-1; i++){
        q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    }
    q1->push_back(generateEOW());
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);



    // Second fill, only with EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    for (int i = 0; i < N-1; i++){
        q1->push_back(generateEOW());
    }
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);

    // Wait for them to finish
    sleep(1);

    for (int i = 0; i < N; i++){
        void* tmp_ret;
        ret = pthread_join(workers[i], &tmp_ret);
        ret = *((int*) tmp_ret);
        CHECK(ret == EXIT_SUCCESS);
        free(tmp_ret);
    }
    

    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(q1->empty());
    for (int i = 0; i < N; i++){
        CHECK(equalQueues(*(workers_args[i].task_queue), *q1));
        CHECK(equalQueues(*(workers_args[i].task_queue), q2));
    }
    pthread_mutex_unlock(&queue_access);

    int total_works_done = 0;
    for (int i = 0; i < N; i++){
        total_works_done += workers_args[i].qtd_worker_jobs;
        std::cout << workers[i] << ": " << workers_args[i].qtd_worker_jobs << std::endl;
        CHECK(workers_args[i].qtd_worker_jobs <= 31);
    }
    CHECK(total_works_done == 31);

    pthread_structs_destroy();
    free(workers);
    free(workers_args);
}

TEST_CASE("2 Workers - Queue 8 - Only EOW Refill"){
    /*
    2 threads working
    queue of size 8
    when refilled, queue has only EOW's
    */
    pthread_structs_init();

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    pthread_mutex_unlock(&queue_access);

    // Create worker threads
    int ret;
    int N = 2;
    pthread_t* workers = (pthread_t*) malloc(N*sizeof(pthread_t));
    worker_data* workers_args = (worker_data*) malloc(N*sizeof(worker_data));
    for (int i = 0; i < N; i++){
        workers_args[i] = worker_data(q1);
    }
    for (int i = 0; i < N; i++){
        ret = pthread_create(&workers[i], NULL, worker_thread, 
                             (void*)&workers_args[i]);
        if(ret){ throw "Failed to create thread"; }
    }

    // 'Cook' refilling pot with EOW's
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    CHECK(q1->empty());
    for (int i = 0; i < N; i++){
        CHECK(equalQueues(*(workers_args[i].task_queue), *q1));
    }
    for (int i = 0; i < N; i++){
        q1->push_back(generateEOW());
    }
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);

    // Wait for it to finish
    sleep(1);


    for (int i = 0; i < N; i++){
        void* tmp_ret;
        ret = pthread_join(workers[i], &tmp_ret);
        ret = *((int*) tmp_ret);
        CHECK(ret == EXIT_SUCCESS);
        free(tmp_ret);
    }
    

    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(q1->empty());
    for (int i = 0; i < N; i++){
        CHECK(equalQueues(*(workers_args[i].task_queue), *q1));
        CHECK(equalQueues(*(workers_args[i].task_queue), q2));
    }
    pthread_mutex_unlock(&queue_access);

    int total_works_done = 0;
    for (int i = 0; i < N; i++){
        total_works_done += workers_args[i].qtd_worker_jobs;
        std::cout << workers[i] << ": " << workers_args[i].qtd_worker_jobs << std::endl;
        CHECK(workers_args[i].qtd_worker_jobs <= 10);
    }
    CHECK(total_works_done == 8);

    pthread_structs_destroy();
    free(workers);
    free(workers_args);
}

TEST_CASE("2 Workers - Queue 8 - Generic Refill"){
    /*
    2 threads working
    queue of size 8
    when refilled, queue has 2 more jobs and the EOW
    */
    pthread_structs_init();

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    pthread_mutex_unlock(&queue_access);

    // Create worker threads
    int ret;
    int N = 2;
    pthread_t* workers = (pthread_t*) malloc(N*sizeof(pthread_t));
    worker_data* workers_args = (worker_data*) malloc(N*sizeof(worker_data));
    for (int i = 0; i < N; i++){
        workers_args[i] = worker_data(q1);
    }
    for (int i = 0; i < N; i++){
        ret = pthread_create(&workers[i], NULL, worker_thread, 
                             (void*)&workers_args[i]);
        if(ret){ throw "Failed to create thread"; }
    }

    // 'Cook' refilling pot with more tasks and a final EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    CHECK(q1->empty());
    for (int i = 0; i < N; i++){
        CHECK(equalQueues(*(workers_args[i].task_queue), *q1));
    }   
    q1->push_back(string2fractalparam("160 120 80 60 0.2709202500 0.0047492500 0.2709203750 0.0047493750"));
    q1->push_back(string2fractalparam("160 120 80 60 0.2709202500 0.0047492500 0.2709203750 0.0047493750"));
    for (int i = 0; i < N; i++){
        q1->push_back(generateEOW());
    }
    pthread_cond_broadcast(&pot_filled);
    pthread_mutex_unlock(&queue_access);

    // Wait for it to finish
    sleep(1);

    // Check if it has finishied, 
    // to avoid infinite loops in testing environment
    // ret = pthread_kill(worker, 0);
    // if (ret == ESRCH){
    //     // No such process, i.e., thread has finishied
    // }else{
    //     throw "Thread still running";
    // }

    for (int i = 0; i < N; i++){
        void* tmp_ret;
        ret = pthread_join(workers[i], &tmp_ret);
        ret = *((int*) tmp_ret);
        CHECK(ret == EXIT_SUCCESS);
        free(tmp_ret);
    }
    

    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(q1->empty());
    for (int i = 0; i < N; i++){
        CHECK(equalQueues(*(workers_args[i].task_queue), *q1));
        CHECK(equalQueues(*(workers_args[i].task_queue), q2));
    }
    pthread_mutex_unlock(&queue_access);

    int total_works_done = 0;
    for (int i = 0; i < N; i++){
        total_works_done += workers_args[i].qtd_worker_jobs;
        std::cout << workers[i] << ": " << workers_args[i].qtd_worker_jobs << std::endl;
        CHECK(workers_args[i].qtd_worker_jobs <= 10);
    }
    CHECK(total_works_done == 10);

    pthread_structs_destroy();
    free(workers);
    free(workers_args);
}


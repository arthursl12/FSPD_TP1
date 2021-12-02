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

TEST_CASE("4 Workers - Queue 16 - Unbalanced"){
    /*
    4 threads working
    queue of size 8
    when refilled, queue has more jobs and one last EOW
    when refilled again, queue has the others EOW's
    */
    pthread_structs_init();

    int N = 4;
    int queue_size = MAX_QUEUE_WORKERS_RATIO*N;

    pthread_mutex_lock(&queue_access);
    std::shared_ptr<QUEUE_TYPE> q1 = std::make_shared<QUEUE_TYPE>();
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("0 0 80 60 -1.5 -1.0 0.5 1.0"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    pthread_mutex_unlock(&queue_access);

    // Create worker threads
    std::vector<std::shared_ptr<pthread_t>> workers;
    std::vector<std::shared_ptr<worker_data>> workers_args;
    for (int i = 0; i < N; i++){
        std::shared_ptr<pthread_t> worker;
        worker = std::make_shared<pthread_t>();
        workers.push_back(worker);

        std::shared_ptr<worker_data> data;
        data = std::make_shared<worker_data>(q1);
        workers_args.push_back(data);
    }

    // Start threads
    int ret;
    for (int i = 0; i < N; i++){
        ret = pthread_create(workers[i].get(), NULL, worker_thread, 
                             (void*)workers_args[i].get());
        if(ret){ throw "Failed to create thread"; }
    }

    // 'Cook' refilling pot with more tasks and a final EOW
    pthread_mutex_lock(&queue_access);
    pthread_cond_wait(&cook_needed, &queue_access);
    CHECK(q1->empty());
    for (int i = 0; i < N; i++){
        CHECK(equalQueues(*(workers_args[i]->task_queue), *q1));
    }
    q1->push_back(string2fractalparam("0 0 80 60 -1.5 -1.0 0.5 1.0"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
    q1->push_back(string2fractalparam("160 60 80 60 0.2709202500 0.0047491250 0.2709203750 0.0047492500"));
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
        ret = pthread_join(*workers[i], &tmp_ret);
        ret = *((int*) tmp_ret);
        CHECK(ret == EXIT_SUCCESS);
        free(tmp_ret);
    }
    

    pthread_mutex_lock(&queue_access);
    QUEUE_TYPE q2;
    CHECK(q1->empty());
    for (int i = 0; i < N; i++){
        CHECK(equalQueues(*(workers_args[i]->task_queue), *q1));
        CHECK(equalQueues(*(workers_args[i]->task_queue), q2));
    }
    pthread_mutex_unlock(&queue_access);

    int total_works_done = 0;
    for (int i = 0; i < N; i++){
        total_works_done += workers_args[i]->qtd_worker_jobs;
        std::cout << *workers[i] << ": " << workers_args[i]->qtd_worker_jobs << std::endl;
        CHECK(workers_args[i]->qtd_worker_jobs <= 14);
    }
    CHECK(total_works_done == 14);

    pthread_structs_destroy();
}



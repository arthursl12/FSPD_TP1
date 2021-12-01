#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "cook.h"

#include <pthread.h>
#include <queue>

TEST_CASE("cook_data Comparator"){
    cook_data cook_args;
    cook_args.task_queue = std::make_shared<QUEUE_TYPE>();
    cook_args.n_threads = 0;
    cook_args.queue_size = 5;
    cook_args.completed_tasks = 3;
    cook_args.created_tasks = 15;
    cook_args.filename = "mandelbrot_tasks/t";

    SUBCASE("Equal"){
        cook_data cook_args2;
        cook_args2.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args2.n_threads = 0;
        cook_args2.queue_size = 5;
        cook_args2.completed_tasks = 3;
        cook_args2.created_tasks = 15;
        cook_args2.filename = "mandelbrot_tasks/t";

        CHECK(equalCookData(cook_args, cook_args2));
    }
    SUBCASE("N_threads"){
        cook_data cook_args2;
        cook_args2.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args2.n_threads = 2;
        cook_args2.queue_size = 5;
        cook_args2.completed_tasks = 3;
        cook_args2.created_tasks = 15;
        cook_args2.filename = "mandelbrot_tasks/t";

        CHECK_FALSE(equalCookData(cook_args, cook_args2));
    }
    SUBCASE("queue_size"){
        cook_data cook_args2;
        cook_args2.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args2.n_threads = 0;
        cook_args2.queue_size = 10;
        cook_args2.completed_tasks = 3;
        cook_args2.created_tasks = 15;
        cook_args2.filename = "mandelbrot_tasks/t";

        CHECK_FALSE(equalCookData(cook_args, cook_args2));
    }
    SUBCASE("Queue content"){
        cook_args.task_queue->push_front(string2fractalparam("0 0 640 480 0.268 0.002 0.272 0.006"));
        cook_args.task_queue->push_front(string2fractalparam("0 0 640 480 0.2 0.0 0.45 0.25"));

        cook_data cook_args2;
        cook_args2.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args2.task_queue->push_front(string2fractalparam("0 0 640 480 0.268 0.002 0.272 0.006"));
        cook_args2.task_queue->push_front(string2fractalparam("0 0 640 480 0.268 0.002 0.272 0.006"));

        cook_args2.n_threads = 0;
        cook_args2.queue_size = 5;
        cook_args2.completed_tasks = 3;
        cook_args2.created_tasks = 15;
        cook_args2.filename = "mandelbrot_tasks/t";

        CHECK_FALSE(equalCookData(cook_args, cook_args2));
    }
    SUBCASE("Queue size"){
        cook_data cook_args2;
        cook_args2.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args2.task_queue->push_front(string2fractalparam("0 0 640 480 0.268 0.002 0.272 0.006"));
        cook_args2.n_threads = 0;
        cook_args2.queue_size = 5;
        cook_args2.completed_tasks = 3;
        cook_args2.created_tasks = 15;
        cook_args2.filename = "mandelbrot_tasks/t";

        CHECK_FALSE(equalCookData(cook_args, cook_args2));
    }
    SUBCASE("File"){
        cook_data cook_args2;
        cook_args2.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args2.n_threads = 0;
        cook_args2.queue_size = 5;
        cook_args2.completed_tasks = 3;
        cook_args2.created_tasks = 15;
        cook_args2.filename = "mandelbrot_tasks/z1";

        CHECK_FALSE(equalCookData(cook_args, cook_args2));
    }
    SUBCASE("Completed tasks"){
        cook_data cook_args2;
        cook_args2.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args2.n_threads = 0;
        cook_args2.queue_size = 5;
        cook_args2.completed_tasks = 15;
        cook_args2.created_tasks = 15;
        cook_args2.filename = "mandelbrot_tasks/z1";

        CHECK_FALSE(equalCookData(cook_args, cook_args2));
    }
    SUBCASE("Created Tasks"){
        cook_data cook_args2;
        cook_args2.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args2.n_threads = 0;
        cook_args2.queue_size = 5;
        cook_args2.completed_tasks = 3;
        cook_args2.created_tasks = 3;
        cook_args2.filename = "mandelbrot_tasks/z1";

        CHECK_FALSE(equalCookData(cook_args, cook_args2));
    }
}

TEST_CASE("cook_data Constructor"){
    SUBCASE("Base"){
        cook_data cook_args;
        cook_args.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args.n_threads = 0;
        cook_args.queue_size = 5;
        cook_args.filename = "mandelbrot_tasks/t";
        cook_args.completed_tasks = 0;
        cook_args.created_tasks = 0;

        cook_data cook_args2;
        cookDataConstructor(cook_args2, 0, 5, "mandelbrot_tasks/t");
        CHECK(equalCookData(cook_args, cook_args2));
    }
    SUBCASE("Default queue_size"){
        cook_data cook_args;
        cook_args.task_queue = std::make_shared<QUEUE_TYPE>();
        cook_args.n_threads = 4;
        cook_args.queue_size = 16;
        cook_args.filename = "mandelbrot_tasks/t";
        cook_args.completed_tasks = 0;
        cook_args.created_tasks = 0;
        
        cook_data cook_args2;
        cookDataConstructor(cook_args2, 4, -1, "mandelbrot_tasks/t");
        CHECK(equalCookData(cook_args, cook_args2));
    }
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
        std::shared_ptr<fractal_param_t> zeroParam = generateEOW();
        for (auto ptr : output){
            CHECK(fractalParamComparator(ptr, zeroParam));
        }
    }
    SUBCASE("More"){
        CHECK(output.empty());
        createEOWList(output, 4);
        CHECK_FALSE(output.empty());
        std::shared_ptr<fractal_param_t> zeroParam = generateEOW();
        for (auto ptr : output){
            CHECK(fractalParamComparator(ptr, zeroParam));
        }
    }
}

TEST_CASE("Cook and 0 workers "){
    pthread_structs_init();

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

    pthread_structs_destroy();
}



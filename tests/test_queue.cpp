#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "vector_ops.h"
#include "cook.h"

#include <queue>


TEST_CASE("readFromFile N - Continue where stopped"){
    std::ifstream infile("mandelbrot_tasks/t");
    std::vector<std::shared_ptr<fractal_param_t>> output;
    std::vector<std::shared_ptr<fractal_param_t>> correct;
    std::shared_ptr<QUEUE_TYPE> myQueue;
    myQueue = std::make_shared<QUEUE_TYPE>();

    fillVector(correct, "240 240 80 60 0.2709203750 0.0047495000 0.2709205000 0.0047496250");
    fillVector(correct, "80 240 80 60 0.2709201250 0.0047495000 0.2709202500 0.0047496250");
    fillVector(correct, "320 180 80 60 0.2709205000 0.0047493750 0.2709206250 0.0047495000");
    fillVector(correct, "320 420 80 60 0.2709205000 0.0047498750 0.2709206250 0.0047500000");
    fillVector(correct, "80 360 80 60 0.2709201250 0.0047497500 0.2709202500 0.0047498750");

    CHECK(myQueue->empty());

    bool res = readFromFile(output, infile, 5);
    // "lock"
    for (auto fp_ptr : output){
        myQueue->push_back(fp_ptr);
    }
    // "unlock"

    CHECK(myQueue->size() == 5);
    output.clear();

    output.push_back(myQueue->front());
    myQueue->pop_front();
    output.push_back(myQueue->front());
    myQueue->pop_front();
    output.push_back(myQueue->front());
    myQueue->pop_front();
    output.push_back(myQueue->front());
    myQueue->pop_front();
    output.push_back(myQueue->front());
    myQueue->pop_front();

    CHECK(myQueue->empty());
    CHECK(equalVector(output, correct));
    CHECK_FALSE(res);
}
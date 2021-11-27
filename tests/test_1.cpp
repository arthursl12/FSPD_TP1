#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "mandel-tiles-graphic.h"

#include <queue>
#include <memory>

#define QUEUE_FULL_TYPE std::queue<std::shared_ptr<fractal_param_t>>

TEST_CASE("Fractal0"){
    std::shared_ptr<QUEUE_FULL_TYPE> myQueue;
    myQueue = std::make_shared<QUEUE_FULL_TYPE>();
    CHECK(myQueue->empty());
}
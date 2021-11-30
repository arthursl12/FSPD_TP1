#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "tasks.h"

#include <queue>
#include <memory>

TEST_CASE("Fractal0"){
    std::shared_ptr<QUEUE_TYPE> myQueue;
    myQueue = std::make_shared<QUEUE_TYPE>();
    CHECK(myQueue->empty());
}
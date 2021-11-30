#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"
#include "vector_ops.h"

#include <fstream>

// typedef struct {
// 	int left; int low;  // lower left corner in the screen
// 	int ires; int jres; // resolution in pixels of the area to compute
// 	double xmin; double ymin;   // lower left corner in domain (x,y)
// 	double xmax; double ymax;   // upper right corner in domain (x,y)
// } fractal_param_t;

TEST_CASE("readFromFile N - General"){
    std::ifstream infile("mandelbrot_tasks/t");
    std::vector<std::shared_ptr<fractal_param_t>> output;
    std::vector<std::shared_ptr<fractal_param_t>> correct;

    fillVector(correct, "240 240 80 60 0.2709203750 0.0047495000 0.2709205000 0.0047496250");
    fillVector(correct, "80 240 80 60 0.2709201250 0.0047495000 0.2709202500 0.0047496250");
    fillVector(correct, "320 180 80 60 0.2709205000 0.0047493750 0.2709206250 0.0047495000");
    fillVector(correct, "320 420 80 60 0.2709205000 0.0047498750 0.2709206250 0.0047500000");
    fillVector(correct, "80 360 80 60 0.2709201250 0.0047497500 0.2709202500 0.0047498750");

    bool res = readFromFile(output, infile, 5);
    CHECK(equalVector(output, correct));
    CHECK_FALSE(res);
}

TEST_CASE("readFromFile N - Continue where stopped"){
    std::ifstream infile("mandelbrot_tasks/t");
    std::vector<std::shared_ptr<fractal_param_t>> output;
    std::vector<std::shared_ptr<fractal_param_t>> correct;

    fillVector(correct, "480 0 80 60 0.2709207500 0.0047490000 0.2709208750 0.0047491250");
    fillVector(correct, "80 180 80 60 0.2709201250 0.0047493750 0.2709202500 0.0047495000");
    fillVector(correct, "240 0 80 60 0.2709203750 0.0047490000 0.2709205000 0.0047491250");
    fillVector(correct, "80 300 80 60 0.2709201250 0.0047496250 0.2709202500 0.0047497500");
    fillVector(correct, "480 240 80 60 0.2709207500 0.0047495000 0.2709208750 0.0047496250");

    bool res = readFromFile(output, infile, 5);
    output.clear();
    res = readFromFile(output, infile, 5);

    CHECK(equalVector(output, correct));
    CHECK_FALSE(res);
}

TEST_CASE("readFromFile N - try reading already EOF'd file"){
    std::ifstream infile("mandelbrot_tasks/z1");
    std::vector<std::shared_ptr<fractal_param_t>> output;
    std::vector<std::shared_ptr<fractal_param_t>> correct;

    bool res = readFromFile(output, infile, 5);
    output.clear();
    res = readFromFile(output, infile, 5);
    std::cout << "res: " << res << std::endl;

    std::cout << "L(out): " << output.size() << "; L(in): " << correct.size() << std::endl;
    CHECK(equalVector(output, correct));
    CHECK(res);
}

TEST_CASE("readFromFile - more than necessary"){
    std::ifstream infile("mandelbrot_tasks/b");
    std::vector<std::shared_ptr<fractal_param_t>> output;
    std::vector<std::shared_ptr<fractal_param_t>> correct;

    fillVector(correct, "0 0 640 480 0.2 0.0 0.45 0.25");

    bool res = readFromFile(output, infile, 5);

    CHECK(equalVector(output, correct));
    CHECK(res);
}

TEST_CASE("readFromFile - 1 and stop "){
    std::ifstream infile("mandelbrot_tasks/b");
    std::vector<std::shared_ptr<fractal_param_t>> output;
    std::vector<std::shared_ptr<fractal_param_t>> correct;

    fillVector(correct, "0 0 640 480 0.2 0.0 0.45 0.25");

    bool res = readFromFile(output, infile, 1);

    CHECK(equalVector(output, correct));
    CHECK(res);
}
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"

#include <fstream>

// typedef struct {
// 	int left; int low;  // lower left corner in the screen
// 	int ires; int jres; // resolution in pixels of the area to compute
// 	double xmin; double ymin;   // lower left corner in domain (x,y)
// 	double xmax; double ymax;   // upper right corner in domain (x,y)
// } fractal_param_t;


void printFractalParam(std::shared_ptr<fractal_param_t> fp){
    std::cout << fp->left << "," << fp->low << ",";
    std::cout << fp->ires << "," << fp->jres << ",";
    std::cout << fp->xmin << "," << fp->ymin << ",";
    std::cout << fp->xmax << "," << fp->ymax;
    std::cout << std::endl;
    
}

bool equalVector(std::vector<std::shared_ptr<fractal_param_t>>& vec1,
                 std::vector<std::shared_ptr<fractal_param_t>>& vec2)
{
    if (vec1.size() != vec2.size()){
        std::cout << "len(1)=" << vec1.size() << "; len(2)=" << vec2.size() << std::endl;
        return false;
    }

    for (unsigned int i = 0; i < vec1.size(); i++){
        if (!fractalParamComparator(vec1.at(i), vec2.at(i))){
            printFractalParam(vec1.at(i));
            printFractalParam(vec2.at(i));
            return false;
        }
    }
    return true;
}

void fillVector(std::vector<std::shared_ptr<fractal_param_t>>& correct, 
                std::string str)
{
    correct.push_back(string2fractalparam(str));
}

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
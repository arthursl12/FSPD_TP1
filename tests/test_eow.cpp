#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"


TEST_CASE("generateEOW"){
    SUBCASE("Base"){
        CHECK_NOTHROW(generateEOW());
    }
    SUBCASE("generate correctly"){
        std::shared_ptr<fractal_param_t> fp1;
        fp1 = std::make_shared<fractal_param_t>();
        fp1->left = 0; fp1->low = 0; fp1->ires = 0; fp1->jres = 0;
        fp1->xmin = 0; fp1->ymin = 0;
        fp1->xmax = 0; fp1->ymax = 0;

        std::shared_ptr<fractal_param_t> fp2;
        fp2 = generateEOW();
        CHECK(fractalParamComparator(fp1,fp2));
    }
}

TEST_CASE("isEOW"){
    SUBCASE("correct"){
        std::shared_ptr<fractal_param_t> fp1;
        fp1 = std::make_shared<fractal_param_t>();
        fp1->left = 0; fp1->low = 0; fp1->ires = 0; fp1->jres = 0;
        fp1->xmin = 0; fp1->ymin = 0;
        fp1->xmax = 0; fp1->ymax = 0;

        CHECK(isEOW(fp1));
    }
    SUBCASE("correct"){
        std::shared_ptr<fractal_param_t> fp1;
        fp1 = std::make_shared<fractal_param_t>();
        fp1->left = 1; fp1->low = 0; fp1->ires = 0; fp1->jres = 0;
        fp1->xmin = 0; fp1->ymin = 0;
        fp1->xmax = 0; fp1->ymax = 0;

        CHECK_FALSE(isEOW(fp1));
    }
}
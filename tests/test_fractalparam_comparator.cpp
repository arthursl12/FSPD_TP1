#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"

TEST_CASE("Fractalparam comparator -- not pointer"){
    fractal_param_t fp1;
    fp1.left = 0; fp1.low = 0; fp1.ires = 640; fp1.jres = 480;
    fp1.xmin = 0.270920; fp1.ymin = 0.004749;
    fp1.xmax = 0.270921; fp1.ymax = 0.004750;

    fractal_param_t fp2;
    fp2.left = 0; fp2.low = 0; fp2.ires = 640; fp2.jres = 480;
    fp2.xmin = 0.270920; fp2.ymin = 0.004749;
    fp2.xmax = 0.270921; fp2.ymax = 0.004750;

    SUBCASE("Base"){
        CHECK_NOTHROW(fractalParamComparator(fp1, fp2));
    }
    SUBCASE("Equal"){
        CHECK(fractalParamComparator(fp1, fp2));
    }
    SUBCASE("Not Equal"){
        CHECK(fractalParamComparator(fp1, fp2));
        fp2.left = 1;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp2.left = 0; fp1.low = 1;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp1.low = 0; fp2.ires = 641;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp2.ires = 640; fp1.jres = 479;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp1.jres = 480; fp2.xmin = 0.230920; 
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp2.xmin = 0.270920; fp1.ymin = 0.005749;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp1.ymin = 0.004749; fp2.xmax = 0.370921;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp2.xmax = 0.270921; fp1.ymax = 0.104750;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp1.ymax = 0.004750;
        CHECK(fractalParamComparator(fp1, fp2));
    }
}

TEST_CASE("Fractalparam comparator -- pointer"){
    std::shared_ptr<fractal_param_t> fp1;
    fp1 = std::make_shared<fractal_param_t>();
    fp1->left = 0; fp1->low = 0; fp1->ires = 640; fp1->jres = 480;
    fp1->xmin = 0.270920; fp1->ymin = 0.004749;
    fp1->xmax = 0.270921; fp1->ymax = 0.004750;

    std::shared_ptr<fractal_param_t> fp2;
    fp2 = std::make_shared<fractal_param_t>();
    fp2->left = 0; fp2->low = 0; fp2->ires = 640; fp2->jres = 480;
    fp2->xmin = 0.270920; fp2->ymin = 0.004749;
    fp2->xmax = 0.270921; fp2->ymax = 0.004750;

    SUBCASE("Base"){
        CHECK_NOTHROW(fractalParamComparator(fp1, fp2));
    }
    SUBCASE("Equal"){
        CHECK(fractalParamComparator(fp1, fp2));
    }
    SUBCASE("Not Equal"){
        CHECK(fractalParamComparator(fp1, fp2));
        fp2->left = 1;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp2->left = 0; fp1->low = 1;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp1->low = 0; fp2->ires = 641;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp2->ires = 640; fp1->jres = 479;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp1->jres = 480; fp2->xmin = 0.230920; 
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp2->xmin = 0.270920; fp1->ymin = 0.005749;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp1->ymin = 0.004749; fp2->xmax = 0.370921;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp2->xmax = 0.270921; fp1->ymax = 0.104750;
        CHECK_FALSE(fractalParamComparator(fp1, fp2));
        fp1->ymax = 0.004750;
        CHECK(fractalParamComparator(fp1, fp2));
    }
}
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "mandel-tiles-graphic.h"
#include "parsing.h"

TEST_CASE("int_tok_parsing"){
    SUBCASE("Base"){
        int myInt = 0;
        std::string str = "201 202 203";
        CHECK_NOTHROW(int_tok_parsing(myInt, str, " "));
    }
    SUBCASE("Sucess: many"){
        int myInt = 0;
        std::string str = "201 202 203";

        int_tok_parsing(myInt, str, " ");
        CHECK(myInt == 201);
        int_tok_parsing(myInt, str, " ");
        CHECK(myInt == 202);
        int_tok_parsing(myInt, str, " ");
        CHECK(myInt == 203);
    }
    SUBCASE("Success: only 1"){
        int myInt = 0;
        std::string str = "201";
        int_tok_parsing(myInt, str, " ");
        CHECK(myInt == 201);
        CHECK_THROWS_MESSAGE(int_tok_parsing(myInt, str, " "), "Int Parsing Failed");
    }
    SUBCASE("Failure: empty"){
        int myInt = 0;
        std::string str = "";
        CHECK_THROWS_MESSAGE(int_tok_parsing(myInt, str, " "), "Int Parsing Failed");
    }
}

TEST_CASE("double_tok_parsing"){
    SUBCASE("Base"){
        double myDouble = 0;
        std::string str = "201.01 202.02 203.03";
        CHECK_NOTHROW(double_tok_parsing(myDouble, str, " "));
    }
    SUBCASE("Sucess: many"){
        double myDouble = 0;
        std::string str = "201.01 202.02 203.03";

        double_tok_parsing(myDouble, str, " ");
        CHECK(myDouble == 201.01);
        double_tok_parsing(myDouble, str, " ");
        CHECK(myDouble == 202.02);
        double_tok_parsing(myDouble, str, " ");
        CHECK(myDouble == 203.03);
    }
    SUBCASE("Success: only 1"){
        double myDouble = 0;
        std::string str = "201.01";
        double_tok_parsing(myDouble, str, " ");
        CHECK(myDouble == 201.01);
        CHECK_THROWS_MESSAGE(double_tok_parsing(myDouble, str, " "), "Double Parsing Failed");
    }
    SUBCASE("Failure: empty"){
        double myDouble = 0;
        std::string str = "";
        CHECK_THROWS_MESSAGE(double_tok_parsing(myDouble, str, " "), "Double Parsing Failed");
    }
}

TEST_CASE("string2fractalparam"){
    SUBCASE("Base"){
        CHECK_NOTHROW(string2fractalparam("0 0 640 480 0.270920 0.004749 0.270921 0.004750"));
    }
    SUBCASE("TestFractal"){
        std::shared_ptr<fractal_param_t> fp1;
        fp1 = std::make_shared<fractal_param_t>();
        fp1->left = 0; fp1->low = 0; fp1->ires = 640; fp1->jres = 480;
        fp1->xmin = 0.270920; fp1->ymin = 0.004749;
        fp1->xmax = 0.270921; fp1->ymax = 0.004750;

        std::shared_ptr<fractal_param_t> fp2;
        fp2 = string2fractalparam("0 0 640 480 0.270920 0.004749 0.270921 0.004750");
        CHECK(fractalParamComparator(fp1,fp2));
    }
    
}
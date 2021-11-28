#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H
#pragma once

#include "mandel-tiles-graphic.h"
#include "parsing.h"

#include <iostream>

inline void printFractalParam(std::shared_ptr<fractal_param_t> fp){
    std::cout << fp->left << "," << fp->low << ",";
    std::cout << fp->ires << "," << fp->jres << ",";
    std::cout << fp->xmin << "," << fp->ymin << ",";
    std::cout << fp->xmax << "," << fp->ymax;
    std::cout << std::endl;
    
}

inline void fillVector(std::vector<std::shared_ptr<fractal_param_t>>& correct, 
                std::string str)
{
    correct.push_back(string2fractalparam(str));
}

inline bool equalVector(std::vector<std::shared_ptr<fractal_param_t>>& vec1,
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


#endif /* VECTOR_OPS */


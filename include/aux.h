#ifndef AUX_H
#define AUX_H

#include "mandel-tiles-graphic.h"

#include <memory>
#include <vector>
#include <string>

std::shared_ptr<fractal_param_t> string2fractalparam(std::string str);
bool fractalParamComparator(fractal_param_t fp1, fractal_param_t fp2);
bool fractalParamComparator(std::shared_ptr<fractal_param_t> fp1, 
                            std::shared_ptr<fractal_param_t> fp2);


#endif /* AUX_H */
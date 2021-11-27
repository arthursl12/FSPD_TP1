#include "aux.h"

std::shared_ptr<fractal_param_t> string2fractalparam(std::string str){}

// typedef struct {
// 	int left; int low;  // lower left corner in the screen
// 	int ires; int jres; // resolution in pixels of the area to compute
// 	double xmin; double ymin;   // lower left corner in domain (x,y)
// 	double xmax; double ymax;   // upper right corner in domain (x,y)
// } fractal_param_t;


bool fractalParamComparator(fractal_param_t fp1, fractal_param_t fp2){
    if ((fp1.left != fp2.left) || (fp1.low != fp2.low) ||
        (fp1. ires != fp2.ires) || (fp1.jres != fp2.jres) ||
        (fp1.xmin != fp2.xmin) || (fp1.ymin != fp2.ymin) ||
        (fp1.xmax != fp2.xmax) || (fp1.ymax != fp2.ymax)){
        
        return false;
    }else{
        return true;
    }
}

bool fractalParamComparator(std::shared_ptr<fractal_param_t> fp1, 
                            std::shared_ptr<fractal_param_t> fp2){
    return fractalParamComparator(*fp1, *fp2);
}
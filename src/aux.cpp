#include "aux.h"

#include <iostream>

/*
Compares if two structs 'fractal_param_t' are equal, i.e., have the same
attribute values
*/
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

/*
Compares if two struct pointers 'fractal_param_t' are equal, i.e., have the same
attribute values (which they point to)
*/
bool fractalParamComparator(std::shared_ptr<fractal_param_t> fp1, 
                            std::shared_ptr<fractal_param_t> fp2){
    return fractalParamComparator(*fp1, *fp2);
}

/* 
Partitions string 'str' by delimiter 'delimiter' and stores the partitioned
token into the string 'token' (note: this token is erased from input string).
If the delimiter wasn't found, simply copies the original string to 'token'.

Returns 'true' if it found the delimiter and it was able to do the operation.
Returns 'false' otherwise. 
*/
bool str_tok(std::string& str, std::string delimiter, std::string& token){
    std::size_t idx = str.find(delimiter);
    token = str.substr(0, idx);

    if (idx == std::string::npos){
        // Did not found delimiter: doesn't do anything, 
        // just copies string to token
        token = str;
        return false;
    }
    str.erase(0, idx+1); /* Erases token and delimiter from input string */
    return true;
}



/*
Parses an integer from a string. This number is separated by a simple space
from the rest of the string. It erases the number and the delimiter from the
input string and stores the integer into passed 'out' parameter.
*/
void int_tok_parsing(int& out, std::string& str, std::string delim){
    std::string token;
    bool res;

    res = str_tok(str, " ", token);
    if (res){
        out = std::stoi(token);
    }else{
        try{
            // We didn't find the delimiter, but maybe the string
            // left is a number itself
            out = std::stoi(token); 
            str.clear();
        }catch (std::exception& e){
            throw "Int Parsing Failed";
        }
    }
}

// typedef struct {
// 	int left; int low;  // lower left corner in the screen
// 	int ires; int jres; // resolution in pixels of the area to compute
// 	double xmin; double ymin;   // lower left corner in domain (x,y)
// 	double xmax; double ymax;   // upper right corner in domain (x,y)
// } fractal_param_t;

/*
Breaks an input string with fractal parameters and puts them into the 
appropriate struct
*/
std::shared_ptr<fractal_param_t> string2fractalparam(std::string str){
    std::shared_ptr<fractal_param_t> pointer;
    pointer = std::make_shared<fractal_param_t>();

    int_tok_parsing(pointer->left, str, " ");
    int_tok_parsing(pointer->low, str, " ");
    int_tok_parsing(pointer->ires, str, " ");
    int_tok_parsing(pointer->jres, str, " ");

    
}
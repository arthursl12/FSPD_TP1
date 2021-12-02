#include "parsing.h"

#include <iostream>
#include <fstream>
#include <vector>

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

/*
Parses an double from a string. This number is separated by a simple space
from the rest of the string. It erases the number and the delimiter from the
input string and stores the double into passed 'out' parameter.
*/
void double_tok_parsing(double& out, std::string& str, std::string delim){
    std::string token;
    bool res;

    res = str_tok(str, " ", token);
    if (res){
        out = std::stod(token);
    }else{
        try{
            // We didn't find the delimiter, but maybe the string
            // left is a number itself
            out = std::stod(token); 
            str.clear();
        }catch (std::exception& e){
            throw "Double Parsing Failed";
        }
    }
}

/*
Breaks an input string with fractal parameters and puts them into the 
appropriate struct object (returns a pointer to it)
*/
std::shared_ptr<fractal_param_t> string2fractalparam(std::string str){
    std::shared_ptr<fractal_param_t> pointer;
    pointer = std::make_shared<fractal_param_t>();

    int_tok_parsing(pointer->left, str, " ");
    int_tok_parsing(pointer->low, str, " ");
    int_tok_parsing(pointer->ires, str, " ");
    int_tok_parsing(pointer->jres, str, " ");

    double_tok_parsing(pointer->xmin, str, " ");
    double_tok_parsing(pointer->ymin, str, " ");
    double_tok_parsing(pointer->xmax, str, " ");
    double_tok_parsing(pointer->ymax, str, " ");

    return pointer;
}

/*
Reads N lines from file. Each line is transformed into a pointer to struct
'fractal_param' and stored into vector 'result', passed as parameter. 

If the file has less than N lines, it will only read those lines available. 
If the file already reached EOF, output will be an empty vector.

Note: result will be cleared before being filled

Returns 'true' if we reached end of file after reading those N lines (or less
if there were less available).
*/
bool readFromFile (std::vector<std::shared_ptr<fractal_param_t>>& result,
                   std::ifstream& input, int n_lines)
{
    std::string line;
    result.clear();
    int i = 0;
    while (std::getline(input, line)){
        result.push_back(string2fractalparam(line));
        i++;
        if (i >= n_lines){
            break;
        }
    }

    // Detect if we reached EOF, i.e., this or next character is EOF
    char peek = input.peek();
    bool end = (peek == EOF);
    if (i == n_lines && !(end)){
        return false;
    }else{
        return true;
    }
}

/*
Generates a EOW task. It is a struct fractal_params, but all values are 0
*/
std::shared_ptr<fractal_param_t> generateEOW(){
    std::shared_ptr<fractal_param_t> fp1;
    fp1 = std::make_shared<fractal_param_t>();
    fp1->left = 0; fp1->low = 0; fp1->ires = 0; fp1->jres = 0;
    fp1->xmin = 0; fp1->ymin = 0;
    fp1->xmax = 0; fp1->ymax = 0;
    return fp1;
}

/*
Checks if a given struct fractal_params is an EOW task, i.e., all its 
attributes' values are 0 
*/
bool isEOW(std::shared_ptr<fractal_param_t> fp){
    std::shared_ptr<fractal_param_t> eow = generateEOW();
    return fractalParamComparator(fp, eow);
}
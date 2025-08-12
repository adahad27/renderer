#ifndef PAR
#define PAR

#include <string>
#include "model.h"
#include <fstream>
#include <sstream>

class Parser {
    public:
        void parse_obj(std::string filename, Model *model);
        void parse_mtl(std::string filename);


};








#endif
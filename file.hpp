#ifndef PAR
#define PAR

#include <string>
#include "model.h"
#include "material.hpp"
#include <unordered_map>
#include <fstream>
#include <sstream>

class Parser {
    public:
        void parse_obj(std::string filename, Model *model);
        void parse_mtl(std::string filename, std::unordered_map<std::string, Material> &materials);


};








#endif
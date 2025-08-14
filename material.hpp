#ifndef MAT
#define MAT
#include "vec.h"
#include <string>
class Material {
    friend class Parser;
    private:
        vec3 Ka;
        vec3 Kd;
        vec3 Ks;
        std::string map_Kd;

};
#endif
#ifndef MAT
#define MAT
#include "vec.h"
#include <string>
// #include "libpng/png.h"

class Material {
    friend class Parser;
    private:
        vec3 Ka;
        vec3 Kd;
        vec3 Ks;
        std::string map_Kd;
        FILE *file;
        png_structp png_ptr;
        png_infop info_ptr;

};
#endif
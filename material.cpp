#include "material.hpp"

vec3 Material::get_pixel(uint32_t x, uint32_t y) {
    return {
        (double) png_get_rows(png_ptr, info_ptr)[x][3*y],
        (double) png_get_rows(png_ptr, info_ptr)[x][3*y+1],
        (double) png_get_rows(png_ptr, info_ptr)[x][3*y+2]
    };
}
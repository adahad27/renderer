#include "vec.h"


bool operator==(vec2 p1, vec2 p2) {
    return p1.x == p2.x && p1.y == p2.y;
}
bool operator!=(vec2 p1, vec2 p2) {
    return !(p1.x == p2.x && p1.y == p2.y);
}


double dot_product(vec2 p1, vec2 p2) {
    return p1.x * p2.x + p1.y + p2.y;
}

double dot_product(vec3 p1, vec3 p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

vec3 operator*(double k, vec3 p) {
    return {k*p.x, k*p.y, k*p.z};
}

vec3 operator+(vec3 p1, vec3 p2) {
    return {p1.x + p2.x, p1.y + p2.y, p1.z + p2.z};
}
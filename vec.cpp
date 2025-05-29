#include "vec.h"
#include <cmath>

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

void normalize(vec3 &v) {
    double norm = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
    v.x /= norm;
    v.y /= norm;
    v.z /= norm;
}

vec3 cross_product(vec3 v, vec3 u) {
    return {v.y*u.z - v.z*u.y, v.z*u.x - v.x*u.z ,v.x*u.y - v.y*u.x};
}
#ifndef VEC
#define VEC

struct vec2
{
    double x;
    double y;
};

struct vec3
{
    double x;
    double y;
    double z;
};

struct triangle_information
{
    vec3 vertices[3];
    vec3 normals[3];
    double reflectivities[3];
};

bool operator==(vec2 p1, vec2 p2);

bool operator!=(vec2 p1, vec2 p2);

double dot_product(vec2 p1, vec2 p2);

double dot_product(vec3 p1, vec3 p2);

vec3 operator*(double k, vec3 p);

vec3 operator+(vec3 p1, vec3 p2);

void normalize(vec3 &v);

vec3 cross_product(vec3 v, vec3 u);

vec3 matmul(vec3 mat[], vec3 v);
#endif
#ifndef MODEL
#define MODEL
#include <string>
#include <vector>
#include <unordered_map>
#include "vec.h"
#include "material.hpp"

class Component {
    friend class Renderer;
    friend class Parser;
    
    private:
        std::vector <vec3> vertices;
        std::vector <vec3> texture_coordinates;
        std::vector <vec3> normals;
        std::vector <vec3> texture_indices;
        std::vector <vec3> faces;
        std::string mat_name;
    
};

class Model {
    /* 
    Renderer will accept an instance of a Model as an argument to render.
    Renderer needs friend class privileges to access the vectors of information
    stored from parsing. 
    The user will not need to access the many vectors because it could lead to
    disturbing the model and forcing a strange rendering.
    */
    friend class Renderer;
    friend class Parser;
    
    private:
        std::unordered_map<std::string, Component> components;
    
    public:

        Model();

};





#endif
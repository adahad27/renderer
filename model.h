#ifndef MODEL
#define MODEL
#include <string>
#include <vector>
#include "vec.h"

class Model {
    /* 
    Renderer will accept an instance of a Model as an argument to render.
    Renderer needs friend class privileges to access the vectors of information
    stored from parsing. 
    The user will not need to access the many vectors because it could lead to
    disturbing the model and forcing a strange rendering.
    */
    friend class Renderer;
    
    private:
        
        std::vector <vec3> vertices;
        std::vector <vec3> texture_coordinates;
        std::vector <vec3> normals;
        std::vector <vec3> texture_indices;
        std::vector <vec3> faces;
        
        void parse_obj(std::string filename);
    
    public:
        
        Model(std::string filename);
        
        void load_model(std::string filename);


};




#endif
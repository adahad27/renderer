#ifndef GL
#define GL
#include "vec.h"
#include "tgaimage.h"
#include <string>


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

class Light {
    private:
        TGAColor color;
        vec3 direction;
        double intensity;
    
    public:
        
    Light();

    Light(vec3 light_direction, double light_intensity, TGAColor light_color);

    void set_color(TGAColor light_color);

    void set_direction(vec3 light_direction);

    void set_intensity(double light_intensity);

    TGAColor get_color();

    vec3 get_direction();

    double get_intensity();

    
};

class Renderer {
    private:
        /* Instance variables */
        TGAImage image;
        TGAImage texture_map;
        
        vec2 camera_position;
        
        double rotation_theta;
        double scale;
        
        int image_width, image_height;
        int texture_width, texture_height;

        std::vector <int> z_buffer;
        Light light;
        /* TODO: Add a vector of lights here */
        
        /* Class methods */
        void line(vec2 p1, vec2 p2, TGAColor color);

        void hollow_triangle(vec2 p1, vec2 p2, vec2 p3, TGAColor color);

        double calculate_triangle_area(vec2 p1, vec2 p2, vec2 p3);

        vec2 projection_on_screen(vec3 p);

        double calculate_angle(double coordinate_1, double coordinate_2);

        void triangle(triangle_information triangle_info, vec3 texture_index, std::vector <vec3> &texture_coordinates);

        void modify_vertices(Model &model);
    public:
        
        Renderer();

        void load_image(int width, int height);

        void load_texture(std::string filename, int width, int height);

        void wireframe(Model &model, TGAColor color);

        void render(Model &model);

        void change_camera(vec2 v);

        void change_scale(double zoom);

        void change_rotation(double rotation);

        void write(std::string filename);

};
// void line(vec2 p1, vec2 p2, TGAImage &image, TGAColor color);

// void hollow_triangle(vec2 p1, vec2 p2, vec2 p3, TGAImage &image, TGAColor color);

// void traditional_triangle(vec2 p1, vec2 p2, vec2 p3, TGAImage &image, TGAColor color);

// double calculate_triangle_area(vec2 p1, vec2 p2, vec2 p3);

// vec2 projection_on_screen(vec3 p, uint32_t offset);

// void triangle(triangle_information triangle_info, vec3 light_direction, double light_intensity, vec3 texture_index, std::vector<vec3> &texture_coodinates, uint32_t offset, TGAImage &texture_map,TGAImage &image, int width, int z_buffer[]);

// void parse_obj(std::string filename, std::vector<vec3> &vertices, std::vector<vec3> &texture_coodinates, std::vector<vec3> &normals,std::vector<vec3> &texture_indices,std::vector<vec3> &faces);

// void scale_obj(uint32_t scale_factor, std::vector<vec3> &vertices);

// double calculate_angle(double coordinate_1, double coordinate_2);

// void rotate_obj(char axis, double angle, std::vector<vec3> &vertices);

// void wireframe_render(std::string filename, uint32_t scale_factor, uint32_t offset, TGAImage &image, TGAColor color);

// void solid_render(std::string filename,uint32_t scale_factor, uint32_t offset, TGAImage &image, vec3 light_direction, double light_intensity, int width, int zbuffer[]);

#endif
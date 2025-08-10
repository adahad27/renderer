#ifndef GL
#define GL
#include "vec.h"
#include "tgaimage.h"
#include <string>
#include "light.h"
#include "model.h"
#include <SDL2/SDL.h>
#define WIDTH 1024
#define HEIGHT 1024

extern SDL_Renderer *sdl_renderer;
extern SDL_Window *sdl_window;

class Renderer {
    private:
        /* Instance variables */
        TGAImage image;
        TGAImage texture_map;
        
        vec3 camera_position;
        vec3 camera_direction;
        
        double rotation_theta;
        double scale;
        
        int image_width, image_height;
        int texture_width, texture_height;

        std::vector <int> z_buffer;
        
        /* TODO: Add a vector of lights here */
        
        /* Class methods */
        void line(vec2 p1, vec2 p2, TGAColor color);

        void hollow_triangle(vec2 p1, vec2 p2, vec2 p3, TGAColor color);

        double calculate_triangle_area(vec2 p1, vec2 p2, vec2 p3);

        vec2 projection_on_screen(vec3 p);

        double calculate_angle(double coordinate_1, double coordinate_2);

        void triangle(triangle_information triangle_info, vec3 texture_index, std::vector <vec3> &texture_coordinates);

        void modify_vertices(Model &model);

        double calculate_diffuse_intensity(double reflectivity, vec3 &vertex,vec3 &surface_normal);

        double calculate_specular_intensity(double reflectivity, vec3 &vertex, vec3 &surface_normal);
    public:
        Light light;

        Renderer();

        void load_image(int width, int height);

        void load_texture(std::string filename, int width, int height);

        void wireframe(Model &model, TGAColor color);

        void render(Model &model);

        void change_camera(vec3 v);

        void change_scale(double zoom);

        void change_rotation(char axis, double rotation, Model &model);

        void write(std::string filename);

};

#endif
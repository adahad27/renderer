#ifndef GL
#define GL

void line(vec2 p1, vec2 p2, TGAImage &image, TGAColor color);

void hollow_triangle(vec2 p1, vec2 p2, vec2 p3, TGAImage &image, TGAColor color);

void traditional_triangle(vec2 p1, vec2 p2, vec2 p3, TGAImage &image, TGAColor color);

double calculate_triangle_area(vec2 p1, vec2 p2, vec2 p3);

vec2 projection_on_screen(vec3 p, uint32_t offset);

void triangle(triangle_information triangle_info, vec3 light_direction, double light_intensity, vec3 texture_index, std::vector<vec3> &texture_coodinates, uint32_t offset, TGAImage &texture_map,TGAImage &image, int width, int z_buffer[]);

void parse_obj(std::string filename, std::vector<vec3> &vertices, std::vector<vec3> &texture_coodinates, std::vector<vec3> &normals,std::vector<vec3> &texture_indices,std::vector<vec3> &faces);

void scale_obj(uint32_t scale_factor, std::vector<vec3> &vertices);

double calculate_angle(double coordinate_1, double coordinate_2);

void rotate_obj(char axis, double angle, std::vector<vec3> &vertices);

void wireframe_render(std::string filename, uint32_t scale_factor, uint32_t offset, TGAImage &image, TGAColor color);

void solid_render(std::string filename,uint32_t scale_factor, uint32_t offset, TGAImage &image, vec3 light_direction, double light_intensity, int width, int zbuffer[]);

#endif
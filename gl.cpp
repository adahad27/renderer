#include "tgaimage.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "vec.h"
#include "gl.h"
#include <sstream>

void Light::set_color(TGAColor light_color) {
    color = light_color;
}

void Light::set_direction(vec3 light_direction) {
    direction = light_direction;
}

void Light::set_intensity(double light_intensity) {
    intensity = light_intensity;
}

Light::Light() {
    color = TGAColor(255, 255, 255, 255);
    direction = {0, 0, 1};
    intensity = 1;
}

TGAColor Light::get_color() {
    return color;
}

vec3 Light::get_direction() {
    return direction;
}

double Light::get_intensity() {
    return intensity;
}

Model::Model(std::string filename) {
    Model::parse_obj(filename);
}

void Model::load_model(std::string filename) {
    Model::parse_obj(filename);
}

Renderer::Renderer() {
    camera_position = {0, 0};
    rotation_theta = 0.;
    scale = 1.;
}

void Renderer::load_image(int width, int height) {
    image = TGAImage(width, height, TGAImage::RGB);
    image_width = width;
    image_height = height;
    /* Resize the z-buffer to the proper dimensions, and then fill it. */
    z_buffer.resize(width * height);
    for(auto &item : z_buffer){
        item = std::numeric_limits<int>::min();
    }

}

void Renderer::load_texture(std::string filename, int width, int height) {
    texture_map = TGAImage(width, height, TGAImage::RGB);
    texture_map.read_tga_file(filename.c_str());
    texture_map.flip_vertically();
}

void Renderer::write(std::string filename) {
    image.flip_vertically();
    image.write_tga_file(filename.c_str());

}

void Renderer::change_scale(double zoom) {
    scale = zoom;
}

void Renderer::line(vec2 p1, vec2 p2, TGAColor color) { 
    bool steep = false; 
    if (std::abs(p1.x-p2.x)<std::abs(p1.y-p2.y)) { 
        std::swap(p1.x, p1.y); 
        std::swap(p2.x, p2.y); 
        steep = true; 
    } 
    if (p1.x>p2.x) { 
        std::swap(p1.x, p2.x); 
        std::swap(p1.y, p2.y); 
    } 
    int dx = p2.x-p1.x; 
    int dy = p2.y-p1.y; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = p1.y; 
    for (int x=p1.x; x<=p2.x; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (p2.y>p1.y?1:-1);
            error2 -= dx*2; 
        } 
    } 
}


void Renderer::hollow_triangle(vec2 p1, vec2 p2, vec2 p3, TGAColor color) {
    /* We must confirm that the points for the triangle are distinct points. */
    assert(p1 != p2);
    assert(p2 != p3);
    assert(p1 != p3);

    /* This will draw the sides of the triangle. */
    line(p1, p2, color);
    line(p2, p3, color);
    line(p3, p1, color);
}


double Renderer::calculate_triangle_area(vec2 p1, vec2 p2, vec2 p3) {
    /* Here we use std::abs because we want the overload for doubles */
    // return std::abs(double(p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) +p3.x*(p1.y-p2.y))/ 2.0);

    return 0.5*((p2.y-p1.y)*(p2.x+p1.x) + (p3.y-p2.y)*(p3.x+p2.x) + (p1.y-p3.y)*(p1.x+p3.x));
}


vec2 Renderer::projection_on_screen(vec3 p) {
    /* 
    We know that the basis vectors for the screen is e1, e2.
    Let our vector of interest be x.
    proj_screen(x) = ((x dot e1)/(e1 dot e1))*(e1) +  ((x dot e2)/(e2 dot e2))*(e2)
    proj_screen(x) = (x dot e1) * e1 + (x dot e2) * e2.
    */
    vec3 e1 = {1, 0, 0};
    vec3 e2 = {0, 1, 0};

    /* Calculate projections here */
    vec3 proj_p1 = dot_product(p, e1)*e1 + dot_product(p, e2)*e2;

    return {proj_p1.x + 512, proj_p1.y + 512};
}

double calculate_diffuse_intensity(double reflectivity, double light_intensity, vec3 surface_normal, vec3 light_direction) {
    /*
    Both surface_normal and light_direction must be unit vectors.
    Furthermore, surface_normal should be positive */
    normalize(surface_normal);
    normalize(light_direction);
    return reflectivity * light_intensity * std::max(dot_product(surface_normal, light_direction), 0.0);
}

void modify_color_intensity(double intensity, TGAColor &color) {
    /* Taking the min allows for higher brightness levels for the user. */
    color.r = std::min(color.r * intensity, 255.0);
    color.g = std::min(color.g * intensity, 255.0);
    color.b = std::min(color.b * intensity, 255.0);
}

/* TODO: Combine triangle_vertices[], vertex_normals[], and reflectivities[] into one structure and pass the structure in */
void Renderer::triangle(triangle_information triangle_info, vec3 texture_index, std::vector <vec3> &texture_coordinates) {
    /*
    First we draw the bounding box for the triangle
    */
    vec2 points[3] = {projection_on_screen(triangle_info.vertices[0]), projection_on_screen(triangle_info.vertices[1]), projection_on_screen(triangle_info.vertices[2])};
    
    int bottom_limit, top_limit, right_limit, left_limit;

    bottom_limit = INT32_MAX;
    top_limit = 0;
    right_limit = 0;
    left_limit = INT32_MAX;

    for(int i = 0; i < 3; ++i) {
        if(points[i].y < bottom_limit) {
            bottom_limit = std::max(points[i].y, 0.);
        }
        if(points[i].x < left_limit) {
            left_limit = std::max(points[i].x, 0.);
        }
        if(points[i].y > top_limit) {
            top_limit = std::min(points[i].y, (double)image_width);
        }
        if(points[i].x > right_limit) {
            right_limit = std::min(points[i].x, (double)image_width);
        }
    }

/* 
TODO: need to work on getting OpenMP pragmas to work properly
so the following for loop can be parallelized.
#pragma omp parallel for

*/
    double total_area = calculate_triangle_area(points[0], points[1], points[2]);
    double area_1, area_2, area_3;
    

    /* Calculating texture coordinates at vertices to use for interpolation later */

    double p1_texture_x = texture_coordinates[(int) texture_index.x].x;
    double p1_texture_y = texture_coordinates[(int) texture_index.x].y;

    double p2_texture_x = texture_coordinates[(int) texture_index.y].x;
    double p2_texture_y = texture_coordinates[(int) texture_index.y].y;

    double p3_texture_x = texture_coordinates[(int) texture_index.z].x;
    double p3_texture_y = texture_coordinates[(int) texture_index.z].y;
    
    vec2 current_vec2;

    for(int x = left_limit; x <= right_limit; ++x) {
        for(int y = bottom_limit; y <= top_limit; ++y) {
            current_vec2 = {(double)x, (double)y};

            /* TODO: Change the variable names for alpha, beta, and gamma to be accurate to Barycentric coordinates */
            area_1 = calculate_triangle_area(current_vec2, points[0], points[1]) / total_area; //gamma
            area_2 = calculate_triangle_area(current_vec2, points[1], points[2]) / total_area; //alpha
            area_3 = calculate_triangle_area(current_vec2, points[2], points[0]) / total_area; //beta
            

            /* Depth interpolation using Barycentric Coordinates*/

            double z_value = area_1*triangle_info.vertices[2].z + area_2*triangle_info.vertices[0].z + area_3*triangle_info.vertices[1].z;

            

            /* We also make a check for the z_buffer to see how close values are */
            if(!(area_1 < 0 || area_2 < 0 || area_3 <0) && z_buffer[x*image_width + y] < z_value) {

                /* UV texture coordinate interpolation using Barycentric Coordinates */

            
                double interpolated_texture_x = 1024 * (area_1*p1_texture_x + area_2*p2_texture_x + area_3*p3_texture_x);
                double interpolated_texture_y = 1024 * (area_1*p1_texture_y + area_2*p2_texture_y + area_3*p3_texture_y);

                TGAColor color = texture_map.get((int)interpolated_texture_x, (int)interpolated_texture_y);

                /* Intensity interpolation using Phong shading */


                vec3 interpolated_normal = area_1*triangle_info.normals[2] + area_2*triangle_info.normals[0] + area_3*triangle_info.normals[1];

                double interpolated_reflectiveness = area_1*triangle_info.reflectivities[0] + area_2*triangle_info.reflectivities[1] + area_3*triangle_info.reflectivities[2];
                double diffuse_intensity = calculate_diffuse_intensity(interpolated_reflectiveness, light.get_intensity(), interpolated_normal, light.get_direction());

                modify_color_intensity(diffuse_intensity, color);


                z_buffer[x*image_width + y] = z_value;
                image.set(x, y, color);
            }
            
        }
    }

}


void Model::parse_obj(std::string filename) {
    std::fstream model_file;

    /* Modify path to change which relative folder renderer searches for when
    rendering models. */
    std::string line;

    model_file.open(filename.c_str());

    
    

    int start, space_index, slash_index_1, slash_index_2;

    /* When starting the parsing, the second character is where
    we want to substring from. */
    start = 2;
    /* Input parsing */
    while(std::getline(model_file, line)) {
        
        if(line[0] == 'v' && line[1] == ' ') {
            /* We parse the string and pass a vertex into our vector */
            vec3 vertex;
            
            double coordinates[3];
            std::stringstream ss(line);
            std::string token;

            uint32_t index = 0;

            while(std::getline(ss, token, ' ')) {
                if(index != 0) {
                    coordinates[index - 1] = std::stod(token);
                }
                index += 1;
            }
            vertex = {coordinates[0], coordinates[1], coordinates[2]};
            vertices.push_back(vertex);
        }
        else if(line[0] == 'v' && line[1] == 't') {
            
            vec3 texture;

            double colors[3];

            std::stringstream ss(line);
            std::string token;

            uint32_t index = 0;

            while(std::getline(ss, token, ' ')) {
                if(index != 0 && token != "") {
                    colors[index - 1] = std::stod(token);
                }
                if(token != "") {
                    /* Accounts for the case when there is more than one space between data points */
                    index += 1;
                }
                
            }

            texture = {colors[0], colors[1], colors[2]};

            texture_coordinates.push_back(texture);

        }
        else if(line[0] == 'v' && line[1] == 'n') {
            vec3 normal;

            double normal_coordinates[3];

            std::stringstream ss(line);
            std::string token;

            uint32_t index = 0;

            while(std::getline(ss, token, ' ')) {
                if(index != 0 && token != "") {
                    normal_coordinates[index - 1] = std::stod(token);
                }
                if(token != "") {
                    /* Accounts for the case when there is more than one space between data points */
                    index += 1;
                }
                
            }

            normal = {normal_coordinates[0], normal_coordinates[1], normal_coordinates[2]};

            normals.push_back(normal);
        }
        else if(line[0] == 'f' && line[1] == ' ') {
            /* We parse the string and pass a face into our vector */
            vec3 face;
            vec3 texture_index;


            double face_vertices[3];
            double coordinate_indices[3];

            std::stringstream ss(line);
            std::string token;

            uint32_t index = 0;

            while(std::getline(ss, token, ' ')) {
                if(index != 0 && token != "") {
                    face_vertices[index - 1] = std::stod(token.substr(0, token.find("/"))) - 1;
                    coordinate_indices[index - 1] = std::stod(token.substr(token.find("/") + 1, token.rfind("/") - token.find("/") - 1)) - 1;
                }
                if(token != "") {
                    /* Accounts for the case when there is more than one space between data points */
                    index += 1;
                }
                
            }
            /* TODO: Change the order in which vertices are paassed for texture_indices */
            face = {face_vertices[0], face_vertices[1], face_vertices[2]};
            texture_index = {coordinate_indices[2], coordinate_indices[0], coordinate_indices[1]};

            faces.push_back(face);
            texture_indices.push_back(texture_index);

        }
    }
}


void scale_obj(uint32_t scale_factor, std::vector<vec3> &vertices) {
    for(uint32_t i = 0; i < vertices.size(); ++i) {
        vertices[i] = scale_factor * vertices[i];
    }
}


double Renderer::calculate_angle(double coordinate_1, double coordinate_2) {
    double normalized = sqrt(pow(coordinate_1, 2) + pow(coordinate_2, 2));
    coordinate_1 = coordinate_1 / normalized;
    coordinate_2 = coordinate_2 / normalized;
    
    return atan2(coordinate_1, coordinate_2);
}


// void rotate_obj(char axis, double angle, std::vector<vec3> &vertices) {
//     /*
//     This function will transform all of the coordinates of the vertices along 
//     the given axis with the given angle
//     Assume the angle is given in degrees.
//     */
//     if(axis == 'x') {
//         for(uint32_t i = 0; i < vertices.size(); ++i) {

//             double raw_y = vertices[i].y;
//             double raw_z = vertices[i].z;

//             double current_angle = calculate_angle(raw_z, raw_y);
//             current_angle += (angle * M_PI / 180);


//             vertices[i].y = cos(current_angle) * sqrt(pow(raw_y, 2) + pow(raw_z, 2));
//             vertices[i].z = sin(current_angle) * sqrt(pow(raw_y, 2) + pow(raw_z, 2));
//         }
//     }
//     else if(axis == 'y') {
//         for(uint32_t i = 0; i < vertices.size(); ++i) {

//             double raw_x = vertices[i].x;
//             double raw_z = vertices[i].z;

//             double current_angle = calculate_angle(raw_z, raw_x);
//             current_angle += (angle * M_PI / 180);


//             vertices[i].x = cos(current_angle) * sqrt(pow(raw_x, 2) + pow(raw_z, 2));
//             vertices[i].z = sin(current_angle) * sqrt(pow(raw_x, 2) + pow(raw_z, 2));
//         }
//     }
//     else if(axis == 'z') {
//         for(uint32_t i = 0; i < vertices.size(); ++i) {

//             double raw_x = vertices[i].x;
//             double raw_y = vertices[i].y;

//             double current_angle = calculate_angle(raw_y, raw_x);
//             current_angle += (angle * M_PI / 180);


//             vertices[i].x = cos(current_angle) * sqrt(pow(raw_x, 2) + pow(raw_y, 2));
//             vertices[i].y = sin(current_angle) * sqrt(pow(raw_x, 2) + pow(raw_y, 2));
//         }
//     }
// }


void Renderer::modify_vertices(Model &model) {
    for(uint32_t i = 0; i < model.vertices.size(); ++i) {
        model.vertices[i] = scale * model.vertices[i];
    }
}

void Renderer::wireframe(Model &model, TGAColor color) {
    

    /* To draw a line from a to b, we must project a, b onto to the 
    screen, to form some vector c, d. Then we call the line() or hollow_triangle()
    function on c and d*/

    modify_vertices(model);

    for(uint32_t i = 0; i < model.faces.size(); ++i) {
        vec3 p1, p2, p3;

        /* Assign the vertices */
        p1 = model.vertices[model.faces[i].x];
        p2 = model.vertices[model.faces[i].y];
        p3 = model.vertices[model.faces[i].z];

        /* Draw a face with the projected vertices on the given image with the given color */
        hollow_triangle(projection_on_screen(p1), 
                        projection_on_screen(p2), 
                        projection_on_screen(p3), color);
    }


}


void Renderer::render(Model &model) {

    modify_vertices(model);

    for(uint32_t i = 0; i < model.faces.size(); ++i) {
        
        

        triangle_information triangle_info;

        triangle_info.vertices[0] = model.vertices[model.faces[i].x];
        triangle_info.vertices[1] = model.vertices[model.faces[i].y];
        triangle_info.vertices[2] = model.vertices[model.faces[i].z];

        triangle_info.normals[0] = model.normals[model.faces[i].x];
        triangle_info.normals[1] = model.normals[model.faces[i].y];
        triangle_info.normals[2] = model.normals[model.faces[i].z];

        triangle_info.reflectivities[0] = 1;
        triangle_info.reflectivities[1] = 1;
        triangle_info.reflectivities[2] = 1;

        triangle(triangle_info, model.texture_indices[i], model.texture_coordinates);
    }
}
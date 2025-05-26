#include "tgaimage.h"
#include <cassert>
#include "model.h"
#include "geometry.h"
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255,   0,   255);
const TGAColor blue  = TGAColor(0, 0,   255,   255);

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

void line(vec2 p1, vec2 p2, TGAImage &image, TGAColor color) { 
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


void hollow_triangle(vec2 p1, vec2 p2, vec2 p3, TGAImage &image, TGAColor color) {
    /* We must confirm that the points for the triangle are distinct points. */
    assert(p1 != p2);
    assert(p2 != p3);
    assert(p1 != p3);

    /* This will draw the sides of the triangle. */
    line(p1, p2, image, color);
    line(p2, p3, image, color);
    line(p3, p1, image, color);
}


void traditional_triangle(vec2 p1, vec2 p2, vec2 p3, TGAImage &image, TGAColor color) {
    
    /* We must confirm that the points for the triangle are distinct points. */
    assert(p1 != p2);
    assert(p2 != p3);
    assert(p1 != p3);

    /* This will draw the sides of the triangle. */
    line(p1, p2, image, color);
    line(p2, p3, image, color);
    line(p3, p1, image, color);

    /* 
    Now we must fill in the triangle, which we can do by sweeping a line over the triangle.
    So to fill in the triangle, let us try using a vertical line that sweeps from left to right. 
    */

    vec2 points[3] = {p1, p2, p3};

    int x_left = INT32_MAX;
    int x_right = 0;

    vec2 left_point, right_point, middle_point;

    for(uint32_t i = 0; i < 3; ++i) {
        
        if(points[i].x < x_left) {
            x_left = points[i].x;
            left_point = points[i];
        }
        if(points[i].x > x_right) {
            x_right = points[i].x;
            right_point = points[i];
        }
    }

    for(uint32_t i = 0; i < 3; ++i) {
        if(points[i] != left_point && points[i] != right_point) {
            middle_point = points[i];
            break;
        }
    }
    int y_top, y_bot;
    int y_line = left_point.y + ((right_point.y - left_point.y) / (right_point.x - left_point.x)) * (middle_point.x - left_point.x);
    for(int x=0; x<=middle_point.x - x_left; ++x) {
        
        /* Now we must calculate the correct values of y_bot, y_top for each x-value */
        y_top = left_point.y + x*(middle_point.y - left_point.y)/float(middle_point.x - left_point.x);
        y_bot = left_point.y + x*(right_point.y - left_point.y)/float(right_point.x - left_point.x);

        if(middle_point.y < y_line) { 
            std::swap(y_top, y_bot);
        }        
        
        for(int y=y_bot; y<y_top; ++y) {
            image.set(x + x_left, y, color);
        }
    }
    if(middle_point.y < y_line) { 
            std::swap(y_top, y_bot);
    } 
    int y_top_offset = y_top;
    int y_bot_offset = y_bot;
    for(int x=0; x<=x_right - middle_point.x; ++x) {
        
        /* Now we must calculate the correct values of y_bot, y_top for each x-value */
        y_top = y_top_offset + x*(right_point.y - middle_point.y)/float(right_point.x - middle_point.x);
        y_bot = y_bot_offset + x*(right_point.y - left_point.y)/float(right_point.x - left_point.x);
        if(middle_point.y < y_line) { 
            std::swap(y_top, y_bot);
        } 
        for(int y=y_bot; y<y_top; ++y) {
            image.set(x + middle_point.x, y, color);
        }
    }
}


double calculate_triangle_area(vec2 p1, vec2 p2, vec2 p3) {
    /* Here we use std::abs because we want the overload for doubles */
    // return std::abs(double(p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) +p3.x*(p1.y-p2.y))/ 2.0);

    return 0.5*((p2.y-p1.y)*(p2.x+p1.x) + (p3.y-p2.y)*(p3.x+p2.x) + (p1.y-p3.y)*(p1.x+p3.x));
}


vec2 projection_on_screen(vec3 p, uint32_t offset) {
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

    return {proj_p1.x + offset, proj_p1.y + offset};
}


void triangle(vec3 p1, vec3 p2, vec3 p3, uint32_t offset, TGAImage &image, TGAColor color, int width, int z_buffer[]) {
    /*
    First we draw the bounding box for the triangle
    */
    vec2 points[3] = {projection_on_screen(p1, offset), projection_on_screen(p2, offset), projection_on_screen(p3, offset)};
    
    int bottom_limit, top_limit, right_limit, left_limit;

    bottom_limit = INT32_MAX;
    top_limit = 0;
    right_limit = 0;
    left_limit = INT32_MAX;

    for(int i = 0; i < 3; ++i) {
        if(points[i].y < bottom_limit) {
            bottom_limit = points[i].y;
        }
        if(points[i].x < left_limit) {
            left_limit = points[i].x;
        }
        if(points[i].y > top_limit) {
            top_limit = points[i].y;
        }
        if(points[i].x > right_limit) {
            right_limit = points[i].x;
        }
    }

/* 
TODO: need to work on getting OpenMP pragmas to work properly
so the following for loop can be parallelized.
#pragma omp parallel for

*/
    double total_area = calculate_triangle_area(points[0], points[1], points[2]);
    double area_1, area_2, area_3;
    vec2 current_vec2;
    for(int x = left_limit; x <= right_limit; ++x) {
        for(int y = bottom_limit; y <= top_limit; ++y) {
            current_vec2 = {(double)x, (double)y};

            area_1 = calculate_triangle_area(current_vec2, points[0], points[1]) / total_area; //alpha
            area_2 = calculate_triangle_area(current_vec2, points[1], points[2]) / total_area; //beta
            area_3 = calculate_triangle_area(current_vec2, points[2], points[0]) / total_area; //gamma
            

            /* Interpolate depth for each pixel in the triangle here using the
            barycentric coordinates for the triangle*/

            double z_value = area_1*p1.z + area_2*p2.z + area_3*p3.z;

            /* We also make a check for the z_buffer to see how close values are */
            if(!(area_1 < 0 || area_2 < 0 || area_3 <0) && z_buffer[x*width + y] < z_value) {
                z_buffer[x*width + y] = z_value;
                image.set(x, y, color);
            }
            
        }
    }

}


void parse_obj(std::string filename, uint32_t scale_factor, std::vector<vec3> &vertices, std::vector<vec3> &faces) {
    std::fstream model_file;

    /* Modify path to change which relative folder renderer searches for when
    rendering models. */
    std::string path = "obj/";
    path.append(filename);
    std::string line;

    model_file.open(path.c_str());

    
    

    int start, space_index, slash_index;

    /* When starting the parsing, the second character is where
    we want to substring from. */
    start = 2;
    /* Input parsing */
    while(std::getline(model_file, line)) {
        
        if(line[0] == 'v' && line[1] == ' ') {
            /* We parse the string and pass a vertex into our vector */
            vec3 vertex;
            
            double coordinates[3];

            
            start = 2;
            
            for(int i = 0; i < 3; ++i) {
                space_index = line.find(" ", start);
                
                coordinates[i] = std::stod(line.substr(start, space_index - start)) * scale_factor;
                start = space_index + 1;
            }

            vertex = {coordinates[0], coordinates[1], coordinates[2]};

            vertices.push_back(vertex);
        }
        else if(line[0] == 'f' && line[1] == ' ') {
            /* We parse the string and pass a face into our vector */
            vec3 face;

            double vertices[3];

            start = 2;

            for(int i = 0; i < 3; ++i) {
                space_index = line.find(" ", start);
                slash_index = line.find("/", start);
                /* Vertices are indexed from 1 in the .obj file */
                vertices[i] = std::stod(line.substr(start, slash_index - start)) - 1;
                start = space_index + 1;
            }

            face = {vertices[0], vertices[1], vertices[2]};
            faces.push_back(face);

        }
    }
}

void wireframe_render(std::string filename, uint32_t scale_factor, uint32_t offset, TGAImage &image, TGAColor color) {
    
    std::vector<vec3> vertices;
    std::vector<vec3> faces;
    
    parse_obj(filename, scale_factor, vertices, faces);

    /* To draw a line from a to b, we must project a, b onto to the 
    screen, to form some vector c, d. Then we call the line() or hollow_triangle()
    function on c and d*/

    for(uint32_t i = 0; i < faces.size(); ++i) {
        vec3 p1, p2, p3;

        /* Assign the vertices */
        p1 = vertices[faces[i].x];
        p2 = vertices[faces[i].y];
        p3 = vertices[faces[i].z];

        /* Draw a face with the projected vertices on the given image with the given color */
        hollow_triangle(projection_on_screen(p1, offset), 
                        projection_on_screen(p2, offset), 
                        projection_on_screen(p3, offset), image, color);
    }


}


void solid_render(std::string filename, uint32_t scale_factor, uint32_t offset, TGAImage &image, TGAColor color, int width, int zbuffer[]) {
    
    std::vector<vec3> vertices;
    std::vector<vec3> faces;
    
    parse_obj(filename, scale_factor, vertices, faces);

    for(uint32_t i = 0; i < faces.size(); ++i) {
        vec3 p1, p2, p3;

        /* Assign the vertices */
        p1 = vertices[faces[i].x];
        p2 = vertices[faces[i].y];
        p3 = vertices[faces[i].z];

        TGAColor random = TGAColor(rand() % 256, rand() % 256, rand() % 256, 255);

        triangle(p1, p2, p3, offset, image, random, width, zbuffer);
    }
}


void rasterize_line(vec2 p1, vec2 p2, TGAImage &image, TGAColor color, int depth_buffer[]) {
    if (p1.x>p2.x) {
        std::swap(p1, p2);
    }
    for (int x=p1.x; x<=p2.x; x++) {
        float t = (x-p1.x)/(float)(p2.x-p1.x);
        int y = p1.y*(1.-t) + p2.y*t;
        if (depth_buffer[x]<y) {
            depth_buffer[x] = y;
            image.set(x, 0, color);
        }
    }
}


void rasterize(vec2 p1, vec2 p2, vec2 p3, TGAImage &image, TGAColor color, int depth_buffer[]) {

}


int main(int argc, char** argv) {
    int width, height;
    
    width = 800;
    height = 800;
    uint32_t offset, scale_factor;
    scale_factor = 300;
    offset = 800 / 2;

	TGAImage image(width, height, TGAImage::RGB);


    

    int zbuffer[width*height];
    for(int x=0; x<width; ++x) {
        for(int y = 0; y < height; ++y) {
            zbuffer[x*width + y] = std::numeric_limits<int>::min();
        }
    }


    // wireframe_render("african_head.obj", scale_factor, offset, image, white);

    solid_render("diablo3_pose.obj", scale_factor, offset, image, white, width, zbuffer);

    // rasterize({1, 1}, {1, 1}, {1, 1}, image, red, zbuffer);

    // line({10,10}, {790, 10}, image, white);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


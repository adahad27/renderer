#include "tgaimage.h"
#include <cassert>
#include "model.h"
#include "geometry.h"
#include <iostream>
#include <cmath>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255,   0,   255);
const TGAColor blue  = TGAColor(0, 0,   255,   255);

struct pixel
{
    int x;
    int y;
};



bool operator==(pixel p1, pixel p2) {
    return p1.x == p2.x && p1.y == p2.y;
}
bool operator!=(pixel p1, pixel p2) {
    return !(p1.x == p2.x && p1.y == p2.y);
}
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1);
            error2 -= dx*2; 
        } 
    } 
}
void hollow_triangle(pixel p1, pixel p2, pixel p3, TGAImage &image, TGAColor color) {
    /* We must confirm that the points for the triangle are distinct points. */
    assert(p1 != p2);
    assert(p2 != p3);
    assert(p1 != p3);

    /* This will draw the sides of the triangle. */
    line(p1.x, p1.y, p2.x, p2.y, image, color);
    line(p2.x, p2.y, p3.x, p3.y, image, color);
    line(p3.x, p3.y, p1.x, p1.y, image, color);
}
void triangle(pixel p1, pixel p2, pixel p3, TGAImage &image, TGAColor color) {
    
    /* We must confirm that the points for the triangle are distinct points. */
    assert(p1 != p2);
    assert(p2 != p3);
    assert(p1 != p3);

    /* This will draw the sides of the triangle. */
    line(p1.x, p1.y, p2.x, p2.y, image, color);
    line(p2.x, p2.y, p3.x, p3.y, image, color);
    line(p3.x, p3.y, p1.x, p1.y, image, color);

    /* 
    Now we must fill in the triangle, which we can do by sweeping a line over the triangle.
    So to fill in the triangle, let us try using a vertical line that sweeps from left to right. 
    */

    pixel points[3] = {p1, p2, p3};

    int x_left = INT32_MAX;
    int x_right = 0;

    pixel left_point, right_point, middle_point;

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


double calculate_triangle_area(pixel p1, pixel p2, pixel p3) {
    /* Here we use std::abs because we want the overload for doubles */
    return std::abs(double(p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) +p3.x*(p1.y-p2.y))/ 2.0);
}

void modern_triangle(pixel p1, pixel p2, pixel p3, TGAImage &image, TGAColor color) {
    /*
    First we draw the bounding box for the triangle
    */
    pixel points[3] = {p1, p2, p3};
    
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
    double total_area = calculate_triangle_area(p1, p2, p3);
    double area_1, area_2, area_3;
    pixel current_pixel;
    for(int x = left_limit; x <= right_limit; ++x) {
        for(int y = bottom_limit; y <= top_limit; ++y) {
            current_pixel = {x, y};

            area_1 = calculate_triangle_area(current_pixel, p1, p2);
            area_2 = calculate_triangle_area(current_pixel, p2, p3);
            area_3 = calculate_triangle_area(current_pixel, p1, p3);
            
            if(total_area == area_1 + area_2 + area_3) {
                image.set(x, y, color);
            }
            
        }
    }

}

int main(int argc, char** argv) {
	TGAImage image(200, 200, TGAImage::RGB);

	// triangle(std::make_pair(10,70), std::make_pair(50, 160), std::make_pair(70, 80), image, red);
    
    pixel p1 = {180, 50};
    pixel p2 = {150, 1};
    pixel p3 = {70, 180};

    // triangle(p1, p2, p3, image, green);
    modern_triangle(p1, p2, p3, image, white);
    

    // triangle(std::make_pair(180, 150), std::make_pair(120, 160), std::make_pair(130, 180), image, red);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


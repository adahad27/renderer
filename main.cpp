#include "tgaimage.h"
#include <cassert>
#include "model.h"
#include "geometry.h"
#include <iostream>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255,   0,   255);
const TGAColor blue  = TGAColor(0, 0,   255,   255);

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
void hollow_triangle(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p3, TGAImage &image, TGAColor color) {
    /* We must confirm that the points for the triangle are distinct points. */
    assert(p1 != p2);
    assert(p2 != p3);
    assert(p1 != p3);

    /* This will draw the sides of the triangle. */
    line(p1.first, p1.second, p2.first, p2.second, image, color);
    line(p2.first, p2.second, p3.first, p3.second, image, color);
    line(p3.first, p3.second, p1.first, p1.second, image, color);
}
void triangle(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p3, TGAImage &image, TGAColor color) {
    
    /* We must confirm that the points for the triangle are distinct points. */
    assert(p1 != p2);
    assert(p2 != p3);
    assert(p1 != p3);

    /* This will draw the sides of the triangle. */
    line(p1.first, p1.second, p2.first, p2.second, image, color);
    line(p2.first, p2.second, p3.first, p3.second, image, color);
    line(p3.first, p3.second, p1.first, p1.second, image, color);

    /* 
    Now we must fill in the triangle, which we can do by sweeping a line over the triangle.
    So to fill in the triangle, let us try using a vertical line that sweeps from left to right. 
    */

    std::pair<int, int> points[3] = {p1, p2, p3};

    int x_left = INT32_MAX;
    int x_right = 0;

    std::pair<int, int> left_point, right_point, middle_point;

    for(uint32_t i = 0; i < 3; ++i) {
        
        if(points[i].first < x_left) {
            x_left = points[i].first;
            left_point = points[i];
        }
        if(points[i].first > x_right) {
            x_right = points[i].first;
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
    int y_line = left_point.second + ((right_point.second - left_point.second) / (right_point.first - left_point.first)) * (middle_point.first - left_point.first);
    for(int x=0; x<=middle_point.first - x_left; ++x) {
        
        /* Now we must calculate the correct values of y_bot, y_top for each x-value */
        y_top = left_point.second + x*(middle_point.second - left_point.second)/float(middle_point.first - left_point.first);
        y_bot = left_point.second + x*(right_point.second - left_point.second)/float(right_point.first - left_point.first);

        if(middle_point.second < y_line) { 
            std::swap(y_top, y_bot);
        }        
        
        for(int y=y_bot; y<y_top; ++y) {
            image.set(x + x_left, y, color);
        }
    }
    if(middle_point.second < y_line) { 
            std::swap(y_top, y_bot);
    } 
    int y_top_offset = y_top;
    int y_bot_offset = y_bot;
    for(int x=0; x<=x_right - middle_point.first; ++x) {
        
        /* Now we must calculate the correct values of y_bot, y_top for each x-value */
        y_top = y_top_offset + x*(right_point.second - middle_point.second)/float(right_point.first - middle_point.first);
        y_bot = y_bot_offset + x*(right_point.second - left_point.second)/float(right_point.first - left_point.first);
        if(middle_point.second < y_line) { 
            std::swap(y_top, y_bot);
        } 
        for(int y=y_bot; y<y_top; ++y) {
            image.set(x + middle_point.first, y, color);
        }
    }
}


int main(int argc, char** argv) {
	TGAImage image(200, 200, TGAImage::RGB);

	// triangle(std::make_pair(10,70), std::make_pair(50, 160), std::make_pair(70, 80), image, red);

    // triangle(std::make_pair(180, 50), std::make_pair(150, 1), std::make_pair(70, 180), image, green);

    triangle(std::make_pair(180, 150), std::make_pair(120, 160), std::make_pair(130, 180), image, red);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


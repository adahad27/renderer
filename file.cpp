#include "file.hpp"
#include "libpng/png.h"

void Parser::parse_obj(std::string filename, Model *model) {
    std::fstream model_file;

    /* Modify path to change which relative folder renderer searches for when
    rendering models. */
    std::string line;

    model_file.open(filename.c_str());

    
    

    
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
            model->vertices.push_back(vertex);
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

            model->texture_coordinates.push_back(texture);

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

            model->normals.push_back(normal);
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
            face = {face_vertices[0], face_vertices[1], face_vertices[2]};
            texture_index = {coordinate_indices[0], coordinate_indices[1], coordinate_indices[2]};

            model->faces.push_back(face);
            model->texture_indices.push_back(texture_index);

        }
    }
}


void Parser::parse_mtl(std::string filename, std::unordered_map<std::string, Material> &materials) {
    std::fstream mtl_file;
    std::string line, word, current_mtl_name;
    std::vector<std::string> words;

    mtl_file.open(filename.c_str());

    while(std::getline(mtl_file, line)) {
        words.clear();
        std::stringstream line_stream(line);
        while(std::getline(line_stream, word, ' ')) {
            words.push_back(word);
        }

        if(!words[0].compare("newmtl")) {
            Material mat = Material();
            materials.insert({words[1], mat});
            current_mtl_name = words[1];
        }
        else if(!words[0].compare("Kd")) {
            materials[current_mtl_name].Kd = {
                std::stod(words[1]),
                std::stod(words[2]),
                std::stod(words[3])
            };
        }
        else if(!words[0].compare("Ka")) {
            materials[current_mtl_name].Ka = {
                std::stod(words[1]),
                std::stod(words[2]),
                std::stod(words[3])
            };
        }
        else if(!words[0].compare("Ks")) {
            materials[current_mtl_name].Ks = {
                std::stod(words[1]),
                std::stod(words[2]),
                std::stod(words[3])
            };
        }
        else if(!words[0].compare("map_Kd")) {
            materials[current_mtl_name].map_Kd = words[1];
        }
        
    }
}

void read_png_alloc(std::string filename, FILE* file, png_structp png_ptr, png_infop info_ptr) {
    png_bytepp row_pointers;
    file = fopen(filename.c_str(), "rb");
    
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, file);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    

}

void read_png_free(FILE* file, png_structp png_ptr, png_infop info_ptr) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);
}

vec3 get_pixel(uint32_t x, uint32_t y, png_structp png_ptr, png_infop info_ptr) {
    return {
        png_get_rows(png_ptr, info_ptr)[x][3*y],
        png_get_rows(png_ptr, info_ptr)[x][3*y+1],
        png_get_rows(png_ptr, info_ptr)[x][3*y+2]
    };
}
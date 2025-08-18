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
        std::stringstream ss(line);
        std::vector<std::string> token_list;
        
        std::string token;
        std::string current_obj;

        while(std::getline(ss, token, ' ')) {
            token_list.push_back(token);
        }
        //load obj/diablo3_pose.obj obj/diablo3_pose_diffuse.tga
        if(token_list.size() > 0 && !token_list[0].compare("v")) {
            Component object;
            model->components.insert({token_list[1], object});
            current_obj = token_list[1];

        }
        else if(token_list.size() > 0 && !token_list[0].compare("v")) {
            /* We parse the string and pass a vertex into our vector */
            
            vec3 vertex = {
                std::stod(token_list[1]),
                std::stod(token_list[2]),
                std::stod(token_list[3])
            };

            model->components[current_obj].vertices.push_back(vertex);
        }
        else if(token_list.size() > 0 && !token_list[0].compare("vt")) {
            
            vec3 texture = {
                std::stod(token_list[1]),
                std::stod(token_list[2]),
                std::stod(token_list[3])
            };

            model->components[current_obj].texture_coordinates.push_back(texture);
        }
        else if(token_list.size() > 0 && !token_list[0].compare("vn")) {
            vec3 normal = {
                std::stod(token_list[1]),
                std::stod(token_list[2]),
                std::stod(token_list[3])
            };
            model->components[current_obj].normals.push_back(normal);
        }
        else if(token_list.size() > 0 && !token_list[0].compare("f")) {
            /* We parse the string and pass a face into our vector */
            vec3 face;
            vec3 texture_index;


            double face_vertices[3];
            double coordinate_indices[3];


            for(uint32_t i = 0; i < 3; ++i) {
                std::stringstream ss(token_list[i+1]);
                
                std::getline(ss, token, '/');
                face_vertices[i] = std::stod(token) - 1;

                std::getline(ss, token, '/');
                coordinate_indices[i] = std::stod(token) - 1;
            }

            face = {face_vertices[0], face_vertices[1], face_vertices[2]};
            texture_index = {coordinate_indices[0], coordinate_indices[1], coordinate_indices[2]};

            model->components[current_obj].faces.push_back(face);
            model->components[current_obj].texture_indices.push_back(texture_index);

        }
        else if(token_list.size() > 0 && !token_list[0].compare("usemtl")) {
            model->components[current_obj].mat_name = token_list[1];
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
    // png_bytepp row_pointers;
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

// vec3 get_pixel(uint32_t x, uint32_t y, png_structp png_ptr, png_infop info_ptr) {
//     return {
//         png_get_rows(png_ptr, info_ptr)[x][3*y],
//         png_get_rows(png_ptr, info_ptr)[x][3*y+1],
//         png_get_rows(png_ptr, info_ptr)[x][3*y+2]
//     };
// }
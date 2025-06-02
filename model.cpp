#include "model.h"
#include <fstream>
#include <sstream>

Model::Model(std::string filename) {
    Model::parse_obj(filename);
}

void Model::load_model(std::string filename) {
    Model::parse_obj(filename);
}

void Model::parse_obj(std::string filename) {
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
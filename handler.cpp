#include "handler.h"
#include "parser.hpp"
#include "vec.h"
#include "gl.h"

#define CMD_QUIT "quit"
#define CMD_LOAD "load"
#define CMD_ROTATE "rotate"
#define CMD_SCALE "scale"
#define CMD_MAT "material"

Renderer renderer = Renderer();
Parser parser = Parser();
Model model;
std::unordered_map<std::string, Material> materials;

//quit
bool check_quit(std::vector<std::string> &args) {
    return args.size() == 1 && args[0].compare(CMD_QUIT) == 0;
}

//load <path_to_file>
bool check_load(std::vector<std::string> &args) {
    //TODO: Add check to see if file exists
    return args.size() == 2 && args[0].compare(CMD_LOAD) == 0;
}

//rotate <axis> <degree>
bool check_rotate(std::vector<std::string> &args) {
    try {
        bool status = 
        
        (args.size() == 3 && 
        args[0].compare(CMD_ROTATE) == 0 &&
        args[1].size() == 1 &&
        (tolower(args[1][0]) == 'x' ||
        tolower(args[1][0]) == 'y' ||
        tolower(args[1][0]) == 'z'));

        // int degree = std::stoi(args[2]);
        return status;
    }
    catch(std::invalid_argument &e) {
        return false;
    }
    
}

//scale <scale_level>
bool check_scale(std::vector<std::string> &args) {
    try {
        bool status = args.size() == 1 && args[0].compare(CMD_SCALE) == 0;
        // int scale = std::stoi(args[1]);
        return status;
    }
    catch(std::invalid_argument &e) {
        return false;
    }
}


void load_model(std::string model_file, std::string texture_file) {
    parser.parse_obj(model_file, &model);

    renderer.load_texture(texture_file.c_str(), WIDTH, HEIGHT);
    renderer.load_image(WIDTH, HEIGHT);
    renderer.light.set_direction({0, 0, 1});
    renderer.render(model);
}

void load_materials(std::string material_file) {
    parser.parse_mtl(material_file, materials);
}

void scale_model(std::string scale) {
    renderer.change_scale(std::stod(scale));
    renderer.render(model);
}

void rotate_model(std::string axis, std::string degree) {
    std::cout << "Changing rotation on " << axis << " axis by " << degree << " degrees\n";
    renderer.change_rotation(axis[0], std::stod(degree), model);
    renderer.render(model);
}

void start_IO_loop() {
    




    std::string input;
    std::string arg;

    std::vector<std::string> args;
    

    while(true) {
        std::getline(std::cin, input);
        std::istringstream buffer(input);

        while(std::getline(buffer, arg, ' ')) {
            args.push_back(arg);
        }
        if(!input.compare(CMD_QUIT)) {
            std::cout << "Application closed!\n";
            return;
        }
        else if(!args[0].compare(CMD_LOAD)) {
            load_model(args[1], args[2]);
            std::cout << "If you can't see the model, try scaling it up!\n";
        }
        else if(!args[0].compare(CMD_ROTATE)) {
            rotate_model(args[1], args[2]);
        }
        else if(!args[0].compare(CMD_SCALE)) {
            scale_model(args[1]);
        }
        else if(!args[0].compare(CMD_MAT)) {
            load_materials(args[1]);
            std::cout << "Material file loaded\n";
        }
        else {
            std::cout << "Unrecognized Command\n";
        }
        args.clear();
    }
}
#include "interface.h"

#define CMD_QUIT "quit"
#define CMD_LOAD "load"
#define CMD_ROTATE "rotate"
#define CMD_SCALE "scale"
#define PORT 8000
#define MAX_MSG_LEN 1000

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

        int degree = std::stoi(args[2]);
        return status;
    }
    catch(std::invalid_argument e) {
        return false;
    }
    
}

//scale <scale_level>
bool check_scale(std::vector<std::string> &args) {
    try {
        bool status = args.size() == 1 && args[0].compare(CMD_SCALE) == 0;
        int scale = std::stoi(args[1]);
        return status;
    }
    catch(std::invalid_argument e) {
        return false;
    }
}

void add_connection(int fd, std::vector<pollfd> &connections) {

}

void delete_connection(int fd, std::vector<pollfd> &connections) {

}


int init_server() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(fd, F_SETFL, O_NONBLOCK);

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &val, sizeof(val));

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);


    bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    return fd;
}

void run_server(int fd) {
    listen(fd, MAX_MSG_LEN);

    struct sockaddr_in client{};
    socklen_t addrlen = sizeof(client);



    std::vector<pollfd> connections;

    add_connection(fd, connections);
    

    while(true) {


        poll(connections.data(), connections.size(), -1);


        for(uint32_t i = 0; i < connections.size(); ++i) {
            if(connections[i].revents & POLLIN) {

                if(connections[i].fd == fd) {
                    int incoming_fd = accept(fd, (sockaddr*) &client, &addrlen);
                    add_connection(incoming_fd, connections);
                    connections[i].revents = 0;
                    std::cout << "Connection established on socket: " << incoming_fd << std::endl;
                }
                else {
                    //Insert read callback here
                    handle_read(states[i]);
                    delete_connection(i, connections);
                    int socket_fd = connections[i].fd;
                    close(socket_fd);
                    std::cout << "Connection closed on socket: " << socket_fd << std::endl;
                }
            }
        }

    }




}

void start_IO_loop() {
    

    int fd = init_server();
    run_server(fd);



    // std::string input;
    // std::string arg;

    // std::vector<std::string> args;
    

    // while(true) {
    //     std::getline(std::cin, input);
    //     std::istringstream buffer(input);

    //     while(std::getline(buffer, arg, ' ')) {
    //         args.push_back(arg);
    //     }
    //     if(input.compare(CMD_QUIT)) {
    //         std::cout << "Application closed!\n";
    //         return;
    //     }
    //     else if(input.compare(CMD_LOAD)) {

    //     }
    //     else if(input.compare(CMD_ROTATE)) {

    //     }
    //     else if(input.compare(CMD_SCALE)) {

    //     }
    //     else {
    //         std::cout << "Unrecognized Command\n";
    //     }
    //     args.clear();
    // }
}
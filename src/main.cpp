#include <iostream>
#include <string>

#include "Request.hpp"
#include "LoadBalancer.hpp"
#include "WebServer.hpp"

const int SCALING_BUFFER = 9;
const int SERVERS = 3;


int main() {
    std::cout << "Setting up " << SERVERS << " initial servers..." << std::endl;

    LoadBalancer lb(SCALING_BUFFER);

    for(int i = 0; i < SERVERS; i++){
        lb.addServer();
    }

    for(int j = 0; j < SERVERS * 50; j++) {
        lb.addRequest();
    }

    std::cout << "Starting simulation..." << std::endl;

    lb.start();

    return 0;
}

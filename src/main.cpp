#include <iostream>
#include <string>

#include "Request.hpp"
#include "WebServer.hpp"

int main() {
    std::cout << "Starting simulation..." << std::endl;
    
    LoadBalancer lb;
    WebServer server1(lb, 10);

    for(int i = 0; i < 10; i++) {
        server1.step();
    }

    return 0;
}

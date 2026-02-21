#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include "Request.hpp"
#include "LoadBalancer.hpp"

class WebServer {
    // request request from load balancer
    // internally process
    public:
        WebServer(LoadBalancer& caller, int processTime)
            : processTime_(processTime), caller_(caller) {}

        bool isProcessing();
        void step();

    private:
        bool processing_ = false;
        int processTime_ = 0;
        int remainingTime_ = 0;
        LoadBalancer& caller_;
        Request& askForRequest(LoadBalancer&);
};

#endif

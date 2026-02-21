#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include "Request.hpp"
#include "LoadBalancer.hpp"

class WebServer {
    // request request from load balancer
    // internally process
    public:
        bool isProcessing();
        void step();

    private:
        bool processing_ = false;
        void processRequest(const Request&);
        void askForRequest(LoadBalancer&);

};

#endif
#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include "Request.hpp"

class LoadBalancer;

class WebServer {
    // request request from load balancer
    // internally process
    public:
        WebServer(LoadBalancer& caller, int processTime)
            : processTime_(processTime), caller_(&caller) {}

        bool isProcessing();
        void step();
        bool shuttingDown_ = false;

    private:
        bool processing_ = false;
        int processTime_ = 0;
        int remainingTime_ = 0;
        LoadBalancer* caller_ = nullptr;
        Request askForRequest(LoadBalancer&);
};

#endif

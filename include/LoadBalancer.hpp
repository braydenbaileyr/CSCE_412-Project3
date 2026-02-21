#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "WebServer.hpp"
#include "Request.hpp"
#include <queue>

class LoadBalancer {
    public:
        LoadBalancer(int scalingBuffer, int processingTime)
            : scalingBuffer_(scalingBuffer), processingTime_(processingTime) {}

        Request getNextRequest();
        void scaleWebServers();
        void addRequest();
        void addServer();
        void start();

    private:
        std::queue<Request> requestQueue_;
        int time_ = 0;
        int scalingBuffer_ = 0;
        int processingTime_ = 0;
        std::vector<WebServer> activeServers_;
        void safeShutDownServers();

};

#endif

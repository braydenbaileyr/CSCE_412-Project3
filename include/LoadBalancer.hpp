#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "WebServer.hpp"
#include "Request.hpp"
#include <queue>

class LoadBalancer {
    public:
        LoadBalancer(int scalingBuffer)
            : scalingBuffer_(scalingBuffer) {}

        Request getNextRequest();
        void scaleWebServers();
        void addRequest();
        void addServer();
        void start();

    private:
        std::queue<Request> requestQueue_;
        int time_ = 0;
        int scalingBuffer_ = 0;
        std::vector<WebServer> activeServers_;
        void safeShutDownServers();

};

#endif

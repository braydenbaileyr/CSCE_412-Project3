#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "Request.hpp"
#include <queue>

class LoadBalancer {
    public:
        Request& getNextRequest();
        void scaleWebServers();

    private:
        std::queue<Request> requestQueue_;
        int time_;
};

#endif
#include "LoadBalancer.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <random>

void LoadBalancer::addRequest() {
    Request req{};
    req.ip_in = std::random_device{}();
    req.ip_out = std::random_device{}();
    
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 7);

    req.time = dist(rng);
    req.type = JobType::Processing;

    requestQueue_.push(req);
}

void LoadBalancer::addServer() {
    WebServer server(*this);
    activeServers_.push_back(server);
}

Request LoadBalancer::getNextRequest() {
    Request req = requestQueue_.front();
    requestQueue_.pop();
    return req;
}

void LoadBalancer::safeShutDownServers() {
    for (auto it = activeServers_.begin(); it != activeServers_.end();) {
        if(it->shuttingDown_ && !it->isProcessing()){
            std::cout << "Shutting down server" << std::endl;
            it = activeServers_.erase(it);
        }
        else {
            ++it;
        }
    }
}

void LoadBalancer::scaleWebServers() {
    int queueSize = requestQueue_.size();
    int serverQty = activeServers_.size();

    if (queueSize > serverQty * 80) {
        // add new server
        std::cout << "Adding server" << std::endl;
        addServer();
    }
    else if (queueSize < serverQty * 50) {
        if (activeServers_.empty()) {
            return;
        }

        std::cout << "Removing server" << std::endl;

        // look for servers not currently processing
        for (auto it = activeServers_.rbegin(); it != activeServers_.rend(); ++it) {
            if (!it->shuttingDown_ && !it->isProcessing()) {
                it->shuttingDown_ = true;
                return;
            }
        }

        // shut down most recently added server
        for (auto it = activeServers_.rbegin(); it != activeServers_.rend(); ++it) {
            if (!it->shuttingDown_) {
                it->shuttingDown_ = true;
                return;
            }
        }
    }

    std::cout << "Total Servers: " << activeServers_.size() << std::endl;
}

void LoadBalancer::start() {
    while(true) {
        std::cout << "Processing at " << time_ << "seconds" << std::endl;

        if(time_ % scalingBuffer_ == 0 ) scaleWebServers();

        for(WebServer& server : activeServers_) {
            server.step();
        }
        safeShutDownServers();

        time_++;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

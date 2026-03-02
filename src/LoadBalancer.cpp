/**
 * @file LoadBalancer.cpp
 * @brief Implements queue management, scaling, and simulation control.
 */
#include "LoadBalancer.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <random>
#include <string>

/**
 * @brief Creates a random request and appends it to the queue if it is not blocked.
 */
void LoadBalancer::addRequest() {
    Request req{};
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<std::uint32_t> octet(0, 255);
    static std::uniform_int_distribution<int> duration(1, 5);
    static std::uniform_int_distribution<int> jobType(0,1);

    req.ip_in = std::to_string(octet(gen)) + "." +
        std::to_string(octet(gen)) + "." +
        std::to_string(octet(gen)) + "." +
        std::to_string(octet(gen));

    req.ip_out = std::to_string(octet(gen)) + "." +
        std::to_string(octet(gen)) + "." +
        std::to_string(octet(gen)) + "." +
        std::to_string(octet(gen));

    req.time = duration(gen);

    if(jobType(gen) == 0){
        req.type = JobType::Processing;
    }
    else {
        req.type = JobType::Streaming;
    }

    if (!firewall_.allows(req)) {
        logger_.log("[" + std::to_string(time_) + "] [FW] Blocked Request from " + req.ip_in);
        return;
    }

    requestQueue_.push(req);
}

/**
 * @brief Adds a new web server to the active server pool.
 */
void LoadBalancer::addServer() {
    WebServer server(*this, activeServers_.size(), logger_);
    logger_.log("[" + std::to_string(time_) + "] [LB] Added server " + std::to_string(server.id));
    activeServers_.push_back(server);
}

/**
 * @brief Removes and returns the next request from the queue.
 * @return The next queued request.
 */
Request LoadBalancer::getNextRequest() {
    Request req = requestQueue_.front();
    requestQueue_.pop();
    return req;
}

/**
 * @brief Checks whether queued work is available.
 * @return True when the request queue contains at least one request.
 */
bool LoadBalancer::hasRequests() const {
    return !requestQueue_.empty();
}

/**
 * @brief Removes servers that were marked for shutdown after they finish processing.
 */
void LoadBalancer::safeShutDownServers() {
    for (auto it = activeServers_.begin(); it != activeServers_.end();) {
        if(it->shuttingDown_ && !it->isProcessing()){
            logger_.log("[" + std::to_string(time_) + "] [LB] Shut down server " + std::to_string(it->id));
            it = activeServers_.erase(it);
        }
        else {
            ++it;
        }
    }
}

/**
 * @brief Adds or removes servers to keep the queue near the target range.
 */
void LoadBalancer::scaleWebServers() {
    int queueSize = requestQueue_.size();
    int serverQty = activeServers_.size();

    if (queueSize > serverQty * 80) {
        logger_.log("[" + std::to_string(time_) + "] [LB] Scaling up");
        addServer();
    }
    else if (queueSize < serverQty * 50) {
        if (activeServers_.empty()) {
            return;
        }

        // look for servers not currently processing
        for (auto it = activeServers_.rbegin(); it != activeServers_.rend(); ++it) {
            if (!it->shuttingDown_ && !it->isProcessing()) {
                it->shuttingDown_ = true;
                logger_.log("[" + std::to_string(time_) + "] [LB] Marked server " + std::to_string(it->id) + " for shutdown");
                return;
            }
        }

        // shut down most recently added server
        for (auto it = activeServers_.rbegin(); it != activeServers_.rend(); ++it) {
            if (!it->shuttingDown_) {
                it->shuttingDown_ = true;
                logger_.log("[" + std::to_string(time_) + "] [LB] Marked busy server " + std::to_string(it->id) + " for shutdown");
                return;
            }
        }
    }

    logger_.log("[" + std::to_string(time_) + "] [LB] Requests remaining: " + std::to_string(requestQueue_.size()));
    logger_.log("[" + std::to_string(time_) + "] [LB] Total servers: " + std::to_string(activeServers_.size()));
}

/**
 * @brief Runs the main simulation loop until the program is stopped.
 */
void LoadBalancer::start() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<std::uint32_t> requestsFreq(0, 2);
    while(true) {
        if(time_ != 0 && time_ % scalingBuffer_ == 0) scaleWebServers();

        int requests = requestsFreq(gen);
        for (int i = 0; i < requests; i++) {
            addRequest();
        }

        for(WebServer& server : activeServers_) {
            server.step();
        }
        safeShutDownServers();

        time_++;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

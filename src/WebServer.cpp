/**
 * @file WebServer.cpp
 * @brief Implements per-server request handling for the simulation.
 */
#include "WebServer.hpp"
#include "LoadBalancer.hpp"
#include <iostream>
#include <string>

/**
 * @brief Reports whether the server is currently busy.
 * @return True when a request is in progress.
 */
bool WebServer::isProcessing() {
    return processing_;
}


/**
 * @brief Advances the server by one clock cycle.
 */
void WebServer::step() {
    if(processing_ == false){
        if(shuttingDown_){
            return;
        }
        if(!caller_->hasRequests()) return;

        Request nextRequest = askForRequest(*caller_);
        // std::cout << "Server " << id << ": Recieved request" << std::endl;
        logger_->log("[" + std::to_string(caller_->getTime()) + "] [S" + std::to_string(id) + "] Received request (" + std::to_string(nextRequest.time) + "s)");
        // std::cout << "Request IP IN: " << nextRequest.ip_in << std::endl;
        
        remainingTime_ = nextRequest.time;
        processing_ = true;
    }   
    else{
        remainingTime_--;
    }

    if(remainingTime_ == 0) {
        processing_ = false;
        // std::cout << "Server " << id << ": Finished processing request" << std::endl;
        logger_->log("[" + std::to_string(caller_->getTime()) + "] [S" + std::to_string(id) + "] Finished request");
    }
}

/**
 * @brief Requests the next item of work from the load balancer.
 * @param caller Load balancer that owns the queue.
 * @return The next queued request.
 */
Request WebServer::askForRequest(LoadBalancer& caller){
    return caller.getNextRequest();
}

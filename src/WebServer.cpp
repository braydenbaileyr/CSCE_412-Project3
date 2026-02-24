#include "WebServer.hpp"
#include "LoadBalancer.hpp"
#include <iostream>

bool WebServer::isProcessing() {
    return processing_;
}

void WebServer::step() {
    if(processing_ == false){
        if(shuttingDown_){
            return;
        }
        // TODO: add handling for no requests in queue
        Request nextRequest = askForRequest(*caller_);
        std::cout << "Procssing Request for " << nextRequest.ip_in << std::endl;
        remainingTime_ = nextRequest.time;
        processing_ = true;
    }   
    else{
        remainingTime_--;
    }

    if(remainingTime_ == 0) {
        processing_ = false;
    }
}

Request WebServer::askForRequest(LoadBalancer& caller){
    return caller.getNextRequest();
}

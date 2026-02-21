#include "WebServer.hpp"
#include "LoadBalancer.hpp"
#include <iostream>

bool WebServer::isProcessing() {
    return processing_;
}

void WebServer::step() {
    if(remainingTime_ == 0){
        // TODO: maybe add handling for no requests left?
        processing_ = false;
        Request& nextRequest = askForRequest(caller_);
        std::cout << "Procssing Request for " << nextRequest.ip_in << std::endl;
        remainingTime_ = processTime_;
        processing_ = true;
    }   
    else{
        remainingTime_--;
    }
}

Request& WebServer::askForRequest(LoadBalancer& caller){
    return caller.getNextRequest();
}

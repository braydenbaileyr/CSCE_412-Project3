/**
 * @file WebServer.hpp
 * @brief Declares the WebServer class used to process queued requests.
 */
#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include "Request.hpp"
#include "Logger.hpp"

class LoadBalancer;

/**
 * @class WebServer
 * @brief Represents a worker that pulls and processes requests from the load balancer.
 */
class WebServer {
    public:
        /**
         * @brief Constructs a web server attached to a load balancer.
         * @param caller Load balancer that supplies requests.
         * @param incomingId Numeric identifier assigned to the server.
         * @param logger Logger used for status messages.
         */
        WebServer(LoadBalancer& caller, int incomingId, Logger& logger)
            : id(incomingId), logger_(&logger), caller_(&caller) {}

        /**
         * @brief Reports whether the server is currently processing a request.
         * @return True when a request is in progress.
         */
        bool isProcessing();

        /**
         * @brief Advances the server by one simulation tick.
         */
        void step();

        /** @brief Indicates whether the server should shut down when idle. */
        bool shuttingDown_ = false;
        /** @brief Identifier used in logs and status output. */
        int id = 0;

    private:
        /** @brief Shared logger used to record server activity. */
        Logger* logger_ = nullptr;
        /** @brief Tracks whether the server has an active request. */
        bool processing_ = false;
        /** @brief Clock cycles remaining for the active request. */
        int remainingTime_ = 0;
        /** @brief Load balancer that owns this server. */
        LoadBalancer* caller_ = nullptr;

        /**
         * @brief Requests the next queued job from the load balancer.
         * @param caller Load balancer that owns the request queue.
         * @return The next request in the queue.
         */
        Request askForRequest(LoadBalancer&);
};

#endif

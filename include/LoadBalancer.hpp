/**
 * @file LoadBalancer.hpp
 * @brief Declares the LoadBalancer class that manages requests and servers.
 */
#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "WebServer.hpp"
#include "Request.hpp"
#include "Logger.hpp"
#include <queue>
#include "Firewall.hpp"

/**
 * @class LoadBalancer
 * @brief Owns the request queue, scaling policy, and active web servers.
 */
class LoadBalancer {
    public:
        /**
         * @brief Constructs a load balancer with logging and firewall support.
         * @param scalingBuffer Number of cycles between scaling checks.
         * @param logger Logger used for simulation events.
         * @param firewall Firewall used to reject blocked IP ranges.
         */
        LoadBalancer(int scalingBuffer, Logger& logger, Firewall& firewall)
            : firewall_(firewall), logger_(logger), scalingBuffer_(scalingBuffer) {}

        /**
         * @brief Removes and returns the next request from the queue.
         * @return The next queued request.
         */
        Request getNextRequest();

        /**
         * @brief Reports whether there are queued requests waiting to be processed.
         * @return True when the request queue is not empty.
         */
        bool hasRequests() const;

        /**
         * @brief Returns the current simulation clock.
         * @return The current time in clock cycles.
         */
        int getTime() const { return time_; }

        /**
         * @brief Applies the scaling policy to add or remove servers.
         */
        void scaleWebServers();

        /**
         * @brief Generates a random request and enqueues it if allowed by the firewall.
         */
        void addRequest();

        /**
         * @brief Adds a new web server to the active pool.
         */
        void addServer();

        /**
         * @brief Starts the main simulation loop.
         */
        void start();

    private:
        /** @brief Firewall used to block requests from restricted IP ranges. */
        Firewall& firewall_;
        /** @brief Shared logger used for status and event messages. */
        Logger& logger_;
        /** @brief FIFO queue containing pending requests. */
        std::queue<Request> requestQueue_;
        /** @brief Current simulation time in clock cycles. */
        int time_ = 0;
        /** @brief Number of cycles between scaling checks. */
        int scalingBuffer_ = 0;
        /** @brief Collection of currently active web servers. */
        std::vector<WebServer> activeServers_;

        /**
         * @brief Removes servers that were marked for shutdown after they become idle.
         */
        void safeShutDownServers();

};

#endif

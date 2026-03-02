/**
 * @file Request.hpp
 * @brief Defines the request payload used by the load balancer simulation.
 */
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

/**
 * @enum JobType
 * @brief Identifies the kind of work a request represents.
 */
enum class JobType: char {
    Processing='p',
    Streaming='s'
};

/**
 * @struct Request
 * @brief Stores the metadata for a single incoming request.
 */
struct Request {
    /** @brief Source IPv4 address for the request. */
    std::string ip_in;
    /** @brief Destination IPv4 address for the request. */
    std::string ip_out;
    /** @brief Number of clock cycles required to complete the request. */
    int time = 0;
    /** @brief Type of work assigned to the request. */
    JobType type = JobType::Processing;
};

#endif

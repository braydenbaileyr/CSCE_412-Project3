/**
 * @file Firewall.hpp
 * @brief Declares the Firewall class used to block configured IP ranges.
 */
#ifndef FIREWALL_HPP
#define FIREWALL_HPP

#include "Request.hpp"

/**
 * @class Firewall
 * @brief Rejects requests whose source address falls within a blocked range.
 */
class Firewall {
    public:
        /**
         * @brief Constructs a firewall with an inclusive blocked IP range.
         * @param startIp First blocked IPv4 address.
         * @param endIp Last blocked IPv4 address.
         */
        Firewall(std::string startIp, std::string endIp)
            :blockStartIp_(startIp), blockEndIp_(endIp) {}

        /**
         * @brief Determines whether a request is allowed through the firewall.
         * @param req Request being checked.
         * @return True when the request source IP is outside the blocked range.
         */
        bool allows(const Request& req);
    private:
        /** @brief First IPv4 address in the blocked range. */
        std::string blockStartIp_;
        /** @brief Last IPv4 address in the blocked range. */
        std::string blockEndIp_;
};

#endif

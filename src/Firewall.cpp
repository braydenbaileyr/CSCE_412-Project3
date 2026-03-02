/**
 * @file Firewall.cpp
 * @brief Implements IP range parsing and firewall checks.
 */
#include "Firewall.hpp"

#include <cstdint>
#include <sstream>

namespace {
/**
 * @brief Converts an IPv4 string into a 32-bit integer for comparison.
 * @param ip IPv4 address string.
 * @return Numeric representation of the IPv4 address.
 */
std::uint32_t parseIp(const std::string& ip) {
    std::istringstream stream(ip);
    char dot = '\0';
    int first = 0;
    int second = 0;
    int third = 0;
    int fourth = 0;

    stream >> first >> dot >> second >> dot >> third >> dot >> fourth;

    return (static_cast<std::uint32_t>(first) << 24) |
           (static_cast<std::uint32_t>(second) << 16) |
           (static_cast<std::uint32_t>(third) << 8) |
           static_cast<std::uint32_t>(fourth);
}
}

/**
 * @brief Checks whether a request's source IP is allowed.
 * @return True when the source IP is outside the blocked range.
 */
bool Firewall::allows(const Request& req) {
    std::uint32_t requestIp = parseIp(req.ip_in);
    std::uint32_t startIp = parseIp(blockStartIp_);
    std::uint32_t endIp = parseIp(blockEndIp_);

    return requestIp < startIp || requestIp > endIp;
}

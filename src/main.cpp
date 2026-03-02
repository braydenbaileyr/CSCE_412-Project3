/**
 * @file main.cpp
 * @brief Collects user input and launches the load balancer simulation.
 */
#include <iostream>
#include <sstream>
#include <string>

#include "Request.hpp"
#include "LoadBalancer.hpp"
#include "WebServer.hpp"
#include "Logger.hpp"
#include "Firewall.hpp"

/**
 * @brief Validates whether a string is a well-formed IPv4 address.
 * @param ip Candidate IPv4 address string.
 * @return True when the input contains four octets in the range 0 to 255.
 */
bool isValidIp(const std::string& ip) {
    std::istringstream stream(ip);
    char dot1 = '\0';
    char dot2 = '\0';
    char dot3 = '\0';
    int first = 0;
    int second = 0;
    int third = 0;
    int fourth = 0;

    if (!(stream >> first >> dot1 >> second >> dot2 >> third >> dot3 >> fourth)) {
        return false;
    }

    if (!stream.eof()) {
        return false;
    }

    if (dot1 != '.' || dot2 != '.' || dot3 != '.') {
        return false;
    }

    return first >= 0 && first <= 255 &&
           second >= 0 && second <= 255 &&
           third >= 0 && third <= 255 &&
           fourth >= 0 && fourth <= 255;
}

/**
 * @brief Entry point for the load balancer simulation.
 * @return Zero when the program exits successfully.
 */
int main() {
    int initialServers = 0;
    int scalingBuffer = 0;
    std::string firewallStartIp;
    std::string firewallEndIp;

    Logger logger("log/LoadBalancer.log");

    std::cout << "Enter number of initial servers: ";
    std::cin >> initialServers;
    while (!std::cin || initialServers <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Please enter a positive integer for initial servers: ";
        std::cin >> initialServers;
    }

    std::cout << "Enter scaling buffer (clock cycles between scaling checks): ";
    std::cin >> scalingBuffer;
    while (!std::cin || scalingBuffer <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Please enter a positive integer for scaling buffer: ";
        std::cin >> scalingBuffer;
    }

    std::cout << "Enter firewall block start IP: ";
    std::cin >> firewallStartIp;
    while (!isValidIp(firewallStartIp)) {
        std::cout << "Please enter a valid IPv4 address for the block start ex: \"192.0.0.0\" : ";
        std::cin >> firewallStartIp;
    }

    std::cout << "Enter firewall block end IP: ";
    std::cin >> firewallEndIp;
    while (!isValidIp(firewallEndIp)) {
        std::cout << "Please enter a valid IPv4 address for the block end ex: \"192.0.0.0\" : ";
        std::cin >> firewallEndIp;
    }

    Firewall firewall(firewallStartIp, firewallEndIp);

    logger.log("[0] [Main] Setting up " + std::to_string(initialServers) + " servers");
    logger.log("[0] [Main] Creating " + std::to_string(initialServers*10) + " requests");
    logger.log("[0] [Main] Task lengths from 1-5 seconds");
    logger.log("[0] [FW] Blocking all requests from " + firewallStartIp + " to " + firewallEndIp);

    LoadBalancer lb(scalingBuffer, logger, firewall);

    for(int i = 0; i < initialServers; i++){
        lb.addServer();
    }

    for(int j = 0; j < initialServers * 100; j++) {
        lb.addRequest();
    }

    logger.log("[0] [Main] Starting simulation");

    lb.start();

    return 0;
}

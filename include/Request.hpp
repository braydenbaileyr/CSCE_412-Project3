#ifndef REQUEST_HPP
#define REQUEST_HPP

#include<cstdint>

enum class JobType: char {
    Processing='p',
    Streaming='s'
};

struct Request {
    std::uint32_t ip_in = 0;
    std::uint32_t ip_out = 0;
    int time = 0;
    JobType type = JobType::Processing;
};

#endif
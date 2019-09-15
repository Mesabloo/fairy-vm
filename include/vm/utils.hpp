#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstring>
#include <string>

namespace vm::utils
{
    inline double bitstring_to_double(std::string const& s)
    {
        uint64_t x = 0;
        for (auto&& it = s.cbegin(); it != s.cend(); ++it)
            x = (x << 1) + (*it - '0');

        double d;
        memcpy(&d, &x, 8);
        return d;
    }
}

#endif
#pragma once
#ifndef LOGS_HPP
#define LOGS_HPP

#include <iostream>
#include <termcolor/termcolor.hpp>

namespace vm::logs
{
    template <class ...T>
    std::ostream& debug(std::ostream& os, T const&... t)
    {
        return os << termcolor::blue << (... << t) << termcolor::reset << std::flush;
    }

    template <class ...T>
    std::ostream& info(std::ostream &os, T const&... t)
    {
        return os << termcolor::green << (... << t) << termcolor::reset << std::flush;
    }

    template <class ...T>
    std::ostream& warn(std::ostream &os, T const&... t)
    {
        return os << termcolor::yellow << (... << t) << termcolor::reset << std::flush;
    }

    template <class ...T>
    std::ostream& error(std::ostream &os, T const&... t)
    {
        return os << termcolor::red << (... << t) << termcolor::reset << std::flush;
    }
}

#endif
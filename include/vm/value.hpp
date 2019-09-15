#pragma once
#ifndef VALUE_HPP
#define VALUE_HPP

#include <variant>
#include <string>
#include <iostream>

namespace vm
{
    using value = std::variant<std::string, std::uint64_t, double>;

    std::ostream& operator<<(std::ostream& os, vm::value const& v)
    {
        if (std::holds_alternative<std::string>(v))
            return os << std::get<std::string>(v);
        if (std::holds_alternative<std::uint64_t>(v))
            return os << std::get<std::uint64_t>(v);
        if (std::holds_alternative<double>(v))
            return os << std::get<double>(v);
        return os << "<no-type-error>";
    }

    namespace value_
    {
        inline vm::value from(std::string& s)
        {
            return s;
        }

        inline vm::value from(std::uint64_t i)
        {
            return i;
        }

        inline vm::value from(double d)
        {
            return d;
        }
    }
}

#endif
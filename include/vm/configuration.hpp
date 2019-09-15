#pragma once
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <array>
#include <vm/instruction.hpp>
#include <vm/value.hpp>
#include <memory>
#include <unordered_map>

namespace vm
{
    struct constants_table
    {
        constexpr static uint16_t MAX_SIZE = std::numeric_limits<std::uint16_t>::max();

        std::uint16_t size;
        std::unordered_map<std::uint16_t, vm::value> map;

        constants_table(): size{0}, map()
        {}

        vm::value& operator[](std::uint16_t index)
        {
            return map.at(index);
        }
    };

    struct labels_table
    {
        constexpr static uint16_t MAX_SIZE = std::numeric_limits<std::uint16_t>::max();

        std::uint16_t size;
        std::unordered_map<std::uint16_t, std::uint64_t> map;

        labels_table(): size{0}, map()
        {}

        std::uint64_t& operator[](std::uint16_t index)
        {
            return map.at(index);
        }
    };

    struct code_table
    {
        std::uint64_t size;
        std::unordered_map<std::uint64_t, vm::instruction> map;

        code_table(): size{0}, map()
        {}

        vm::instruction& operator[](std::uint16_t index)
        {
            return map.at(index);
        }
    };

    struct configuration
    {
        std::uint64_t ip{0};
        std::unique_ptr<vm::constants_table> constants;
        std::unique_ptr<vm::labels_table> labels;
        std::unique_ptr<vm::code_table> code;

        configuration(std::uint64_t ip = 0): ip(ip), constants(nullptr), labels(nullptr), code(nullptr)
        {}

        friend std::ostream& operator<<(std::ostream& os, vm::configuration const& config)
        {
            os << "IP: 0x" << std::hex << config.ip;

            os << "\nCONSTANTS TABLE: [\n";
            if (config.constants)
                for (std::uint16_t i{0}; i < config.constants->size; ++i)
                    os << "    " << i << " => " << config.constants->map[i] << "\n";

            os << "]\nLABELS TABLE: [\n";
            if (config.labels)
                for (std::uint16_t i{0}; i < config.labels->size; ++i)
                    os << "    " << i << " => 0x" << std::hex << config.labels->map[i] << "\n";

            os << "]\nCODE TABLE: [\n";
            if (config.code)
                for (std::uint64_t i{0}; i < config.code->size; ++i)
                    os << "    " << std::hex << i << ": " << config.code->map.at(i) << "\n";

            return os << "]" << std::endl;
        }
    };
}

#endif
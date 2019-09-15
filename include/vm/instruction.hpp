#pragma once
#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <vector>
#include <iostream>

namespace vm
{
    struct instruction
    {
        std::uint16_t opcode;
        std::vector<std::uint16_t> args;

        instruction(std::uint16_t op, std::vector<std::uint16_t> args = {}): opcode(op), args(args)
        {}

        instruction(instruction&& i): opcode(i.opcode), args(i.args)
        {}

        friend std::ostream& operator<<(std::ostream& os, vm::instruction const& i)
        {
            os << "{" << std::hex << i.opcode << " ";
            for (auto&& it = i.args.cbegin(); it != i.args.cend(); ++it)
                os << std::hex << *it << " ";
            return os << "}";
        }
    };
}

#endif
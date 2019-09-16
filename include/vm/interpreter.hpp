#pragma once
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vm/configuration.hpp>
#include <vm/instruction.hpp>
#include <vector>
#include <memory>

namespace vm
{
    enum instruction_table
    {
        INT = 0x0003,
        LOAD_CONST = 0x0002
    };

    inline void execute(std::shared_ptr<vm::configuration> const& config, std::uint16_t const opcode, std::vector<std::uint16_t> const& args)
    {
        switch (opcode)
        {
            case LOAD_CONST:
                config->stack.push(config->constants->map.at(args[0]));
        }
    }

    inline uint8_t start(std::shared_ptr<vm::configuration> const& config)
    {
        vm::instruction const& i = config->code->map.at(config->ip++);
        switch (i.opcode)
        {
            case INT:
                return std::get<std::uint64_t>(config->stack.top());
            default:
                execute(config, i.opcode, i.args);
                return start(config);
        }
    }
}

#endif
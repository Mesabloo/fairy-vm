#pragma once
#ifndef INIT_HPP
#define INIT_HPP

#include <vector>
#include <string>
#include <vm/configuration.hpp>
#include <fstream>
#include <argh.h>
#include <vm/utils.hpp>
#include <filesystem>
#include <memory>

#ifndef NDEBUG
#   include <vm/logs.hpp>
#endif

namespace vm
{
    namespace
    {
        inline bool parse_arguments(int argc, char** argv, std::ifstream& is, std::shared_ptr<vm::configuration>& conf)
        {
            argh::parser args_parser;
            args_parser.add_params({"l", "load"});
            args_parser.parse(argc, argv);

            std::string const file_name(args_parser({"-l", "--load"}).str());
            std::filesystem::path const p(file_name);

            if (!std::filesystem::exists(p))
                return false;

            is = std::ifstream(std::filesystem::canonical(p), std::ios::binary);
            return true;
        }

        inline bool parse_header(std::ifstream& is)
        {
            char header[5];
            is.read(header, 5);
            std::string s(header, 5);
            return s == "fairy";
        }

        inline std::unique_ptr<vm::constants_table> parse_constants_table(std::ifstream& is)
        {
            auto table = std::make_unique<vm::constants_table>();

            if (is.peek() != 0x30)
                return table;
            is.get();

            std::uint16_t const size = (is.get() << 8) | is.get();
            table->size = size;

            for (std::uint16_t i{0}; i < size; ++i)
            {
                std::uint16_t constant_id = (is.get() << 8) | is.get();
                std::uint8_t type = is.get();
                std::uint16_t constant_size = (is.get() << 8) | is.get();
                switch (type)
                {
                    case 0x01: // is uint64_t
                    {
                        std::uint64_t val{0};
                        for (std::uint16_t j{0};j < constant_size; ++j)
                            val <<= 8, val += is.get();
                        table->map[constant_id] = val;
                        break;
                    }
                    case 0x02: // is double
                    {
                        std::string bin;
                        for (std::uint16_t j{0}; j < constant_size; ++j)
                            bin += is.get() + '0';

                        double const d = vm::utils::bitstring_to_double(bin);
                        table->map[constant_id] = d;
                        break;
                    }
                    case 0x03: // is string
                    {
                        std::string data;
                        for (std::uint16_t j{0}; j < constant_size; ++j)
                            data += is.get();

                        table->map[constant_id] = data;
                        break;
                    }
                }
            }

            return table;
        }

        inline std::unique_ptr<vm::labels_table> parse_labels_table(std::ifstream& is)
        {
            auto table = std::make_unique<vm::labels_table>();

            if (is.peek() != 0x31)
                return table;
            is.get();

            std::uint16_t const size = (is.get() << 8) | is.get();
            table->size = size;

            for (std::uint16_t i{0}; i < size; ++i)
            {
                std::uint16_t label_id = (is.get() << 8) | is.get();
                std::uint64_t address = (is.get() << 24) | (is.get() << 16) | (is.get() << 8) | is.get();

                table->map[label_id] = address;
            }

            return table;
        }

        inline std::unique_ptr<vm::code_table> parse_code_table(std::ifstream& is)
        {
            auto table = std::make_unique<vm::code_table>();

            if (is.peek() != 0x32)
                return table;
            is.get();

            std::uint64_t const size = (is.get() << 24) | (is.get() << 16) | (is.get() << 8) | is.get();
            table->size = size;

            for (std::uint64_t i{0}; i < size; ++i)
            {
                std::uint16_t op = (is.get() << 8) | is.get();
                std::vector<std::uint16_t> args{};
                std::uint8_t args_size{};

                if ((args_size = is.get()) != 0)
                    for (uint8_t j{0}; j < args_size; ++j)
                    {
                        std::uint16_t arg = (is.get() << 8) | is.get();
                        args[j] = arg;
                    }

                table->map.emplace(i, vm::instruction(op, args));
            }

            return table;
        }
    }

    inline std::shared_ptr<vm::configuration> init(int argc, char** argv)
    {
#ifndef NDEBUG
        namespace vlogs = vm::logs;
#endif

        std::ifstream file;
        auto config = std::make_shared <vm::configuration>();

        if (!parse_arguments(argc, argv, file, config))
#ifdef NDEBUG
            return config;
#else
            return vlogs::error(std::cout, "File not found.\n"), config;
#endif

        if (!parse_header(file))
#ifdef NDEBUG
            return config;
#else
            return vlogs::error(std::cout, "Invalid bytecode file header.\n"), config;
#endif

        config->constants = std::move(parse_constants_table(file));
        config->labels = std::move(parse_labels_table(file));
        config->code = std::move(parse_code_table(file));

        return config;
    }
}

#endif
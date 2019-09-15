#include <vm/init.hpp>
#include <vm/configuration.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    auto const& config = vm::init(argc, argv);
#ifndef NDEBUG
    std::cout << *config << std::endl;
#endif

    return 0;
}
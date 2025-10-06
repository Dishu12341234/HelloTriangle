#include "HelloTriangleApplication.hpp"
#include <stdexcept>
#include <iostream>
#include <cstdlib>

int main(int argc, char const *argv[])
{
    HelloTriangleApplication application;

    try
    {
        application.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    
}

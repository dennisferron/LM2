#include "gtest/gtest.h"

#include <iostream>

using namespace std;

int main(int argc, char const* argv[])
{
    try
    {
        int result = UnitTest::RunAllTests();
        //std::cout << "Done" << std::endl;
        std::cout << "Press enter..." << std::endl;
        std::cin.ignore( 99, '\n' );
        return result;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Tests exited with exception '" << e.what() << "'" << std::endl;
        std::cout << "Error. Press enter..." << std::endl;
        std::cin.ignore( 99, '\n' );
        return -1;
    }
    catch (...)
    {
        std::cout << "Error. Press enter..." << std::endl;
        std::cin.ignore( 99, '\n' );
        return -1;
    }
}

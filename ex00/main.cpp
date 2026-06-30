#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>

int main (int ac, char *av[])
{
    if (ac != 2)
    {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    
    BitcoinExchange btc;
    btc.processInputFile(av[1]);

    return 0;
}
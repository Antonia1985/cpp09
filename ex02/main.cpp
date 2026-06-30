#include "PmergeMe.hpp"
#include <iostream>

int main(int ac, char**av)
{
    if (ac == 1)
    {
        std::cerr << "Error Usage: ./PmergeMe <expression>" << std::endl;
        return 1;
    }

    std::string numberStr;

    for (int i = 1; i < ac; i++)
    {
        numberStr += std::string(av[i]) + " ";
    }

    PmergeMe mrg;
    if (!mrg.StartProgram(numberStr))
        return 1;

    return 0;
}
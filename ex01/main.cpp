#include "RPN.hpp"
#include <iostream>

int main(int ac, char*av[])
{
    if (ac != 2)
    {
        std::cerr << "Usage: ./RPN <expression>" << std::endl;
        return 1;
    }

    RPN rpn;
    rpn.calculateExpr(av[1]);

    return 0;
}
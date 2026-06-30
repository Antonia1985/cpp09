#include "RPN.hpp"
#include <iostream>
#include <sstream>

RPN::RPN() :_elements(){}

RPN::RPN(const RPN& other) : _elements(other._elements){}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
    {
        this->_elements = other._elements;
    }
    return (*this);
}

RPN::~RPN()
{

}

bool RPN::applyOperator(char opertr)
{
    int a;
    int b;
    int result;
    if(_elements.size() < 2)
        return false;

    if(opertr == '+') //+
    {
        b = _elements.top();
        _elements.pop();

        a = _elements.top();
        _elements.pop();

        result = a + b;
        _elements.push(result);
        //std::cout << "In (+): a = " << a << ", b = " << b << ", result = " << result << std::endl;
    }
    else if(opertr == '-') //-
    {
        b = _elements.top();
        _elements.pop();

        a = _elements.top();
        _elements.pop();

        result = a - b;
        _elements.push(result);
        //std::cout << "In (-): a = " << a << ", b = " << b << ", result = " << result << std::endl;
    }
    else if(opertr == '*') //*
    {
        b = _elements.top();
        _elements.pop();

        a = _elements.top();
        _elements.pop();

        result = a * b;
        _elements.push(result);
        //std::cout << "In (*): a = " << a << ", b = " << b << ", result = " << result << std::endl;
    }
    else if(opertr == '/') // /
    {
        b = _elements.top();
        
        if (b == 0)
            return false;

        _elements.pop();
        a = _elements.top();
        _elements.pop();

        result = a / b;
        _elements.push(result);
        //std::cout << "In (/): a = " << a << ", b = " << b << ", result = " << result << std::endl;
    }
    return true;
}

void RPN::calculateExpr(const std::string& elementsStr)
{
    std::string token;

    //empty the stack if there is remaining from possible previous call.
    while (!_elements.empty())
        _elements.pop();

    if(elementsStr.empty())
    {
        std::cerr << "Error" << std::endl;
        return;
    }
    if(elementsStr.find_first_not_of("0123456789+-/* ") != std::string::npos)//optional because the same check happens later
    {
        std::cerr << "Error" << std::endl;
        return;
    }

    
    std::istringstream iss(elementsStr);//Use istringstream if you want easier validation of bad tokens like 24 and fatser for tokens seperated by spaces

    while(iss >> token)
    {
        if(token.length() != 1)
        {
            std::cerr << "Error" << std::endl;
            return;
        }

        char c = token[0];
        if (c >= '0' && c <= '9') // if number
        {
            _elements.push(c - '0');
        }
        else if (c == '+' || c == '-' || c == '*' ||c == '/') // if operator
        {
            if(!applyOperator(c))
            {
                std::cerr << "Error" << std::endl;
                return;
            }
        }
        else //if anything else
        {
            std::cerr << "Error" << std::endl;
            return;
        }
    }
   
    if (_elements.size() != 1)
    {
        std::cerr << "Error" << std::endl;
        return;
    }
    std::cout << _elements.top() << std::endl;
}

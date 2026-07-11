#include "PmergeMe.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <climits>
#include<ctime>


PmergeMe::PmergeMe() : _mainchain(), _mainchaind() {}

PmergeMe::PmergeMe(const PmergeMe& other) : _mainchain(other._mainchain), _mainchaind(other._mainchaind){}

PmergeMe& PmergeMe::operator= (const PmergeMe& other)
{
    if (this != &other)
    {
        _mainchain = other._mainchain;
        _mainchaind = other._mainchaind;
    }
    return (*this);
}

PmergeMe::~PmergeMe(){}


static bool validateToken(std::string token)
{
    for(size_t i = 0; i < token.size(); i++)
    {
        if((token[i] < '0') || token[i] > '9')
            return false;        
    }
    return true;
}

static bool validateExpression(std::string expression, std::vector<int>& expr, std::deque<int>& exprd)
{
    //populate the vector and deque
    std::string token;
    int num;        
    std::istringstream iss(expression);
   
    while(iss >> token)
    {
        if (!validateToken(token))
        {
            return false;
        }
        long l = 0;
        std::istringstream iss1(token);
        if (!(iss1 >> l))
            return false;
       
        if ((l <= 0) || (l > INT_MAX))
        {
            return false;
        }       
        num = l;
        expr.push_back(num);        
        exprd.push_back(num);
    }
    if(expr.empty())
        return false;

    std::vector<int> sorted = expr;
    std::sort(sorted.begin(), sorted.end());
    if (std::adjacent_find(sorted.begin(), sorted.end()) != sorted.end())
        return false;    

    return true;
}

template <typename T>
static void printNums(const T& nums, const std::string& str)  
{
    typename T::const_iterator itm = nums.begin();
    std::cout << str << " ";
    while(itm != nums.end())
    {
        std::cout << *itm << " ";
        ++itm;
    }
    std::cout << std::endl;
}

bool PmergeMe::compareResults()
{
    size_t sizeVect = _mainchain.size();
    size_t sizeDequ = _mainchaind.size();
    
    if(sizeVect != sizeDequ)
        return false;

    for(size_t i = 0; i < sizeVect; i++)
    {
        if (_mainchain[i] != _mainchaind[i])
            return false;
    }

    return true;
}

bool PmergeMe::StartProgram(std::string expression)
{
    //clear leftovers form previous run
    _mainchain.clear();
    _mainchaind.clear();

    std::vector<int> expr;
    std::deque<int> exprd;
    
    if(!validateExpression(expression, expr, exprd))
    {
        std::cerr << "Error" << std::endl;
        return false;
    }   
    
    clock_t  start_s1=clock();
    applyFJAlgoByVector(expr);
    clock_t  stop_s1=clock();

    clock_t  start_s2=clock(); 
    applyFJAlgoByDeque(exprd);//to be 
    clock_t  stop_s2=clock();

    if(!compareResults())
    {
        std::cerr << "Error" << std::endl;
        return false;
    }

    printNums(expr, "Before: ");
    printNums(_mainchain, "After: ");

    //take input's size as a string
    std::ostringstream oss;
    oss << expr.size();
    std::string size = oss.str();

    std::cout << "Time to process a range of " << size <<" elements with std::vector : " << (stop_s1-start_s1)/double(CLOCKS_PER_SEC)*1000000 << " us" << std::endl; 
    std::cout << "Time to process a range of " << size <<" elements with std::deque : " << (stop_s2-start_s2)/double(CLOCKS_PER_SEC)*1000000 << " us" << std::endl; 

    return true;
}
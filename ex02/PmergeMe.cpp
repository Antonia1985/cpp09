#include "PmergeMe.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <utility>//pair
#include <climits>
#include<ctime>


PmergeMe::PmergeMe() : _pairs(), _mainchain(), _leftoverchain(),
                    _pairsd(), _mainchaind(), _leftoverchaind(){}

PmergeMe::PmergeMe(const PmergeMe& other) : _pairs(other._pairs),_mainchain(other._mainchain), _leftoverchain(other._leftoverchain),
                _pairsd(other._pairsd), _mainchaind(other._mainchaind), _leftoverchaind(other._leftoverchaind){}

PmergeMe& PmergeMe::operator= (const PmergeMe& other)
{
    if (this != &other)
    {
        _pairs = other._pairs;
        _mainchain = other._mainchain;
        _leftoverchain = other._leftoverchain;
        _pairsd = other._pairsd;
        _mainchaind = other._mainchaind;
        _leftoverchaind = other._leftoverchaind;
    }
    return (*this);
}

PmergeMe::~PmergeMe(){}

static bool compareBySecond(const std::pair<int, int>& a, const std::pair<int, int>& b)
{
    return a.second < b.second;
}

/*//testing func
void printpairs(std::vector<std::pair<int, int> >& _pairs)
{
    std::vector<std::pair<int, int> >::iterator i;
    for(i = _pairs.begin(); i !=_pairs.end(); ++i)
    {
        std::cout << i->first << "-" << i->second << " " ;
    }
    std::cout << std::endl;
}
    
void printvect(std::vector<int>& vec)
{
    std::vector<int>::iterator i;
    for(i = vec.begin(); i !=vec.end(); ++i)
    {
        std::cout << *i << " " ;
    }
    std::cout << std::endl;

}*/

bool PmergeMe::applyFJAlgoByVect(const std::vector<int>& expr)
{
    if(expr.size() == 1)
    {
        _mainchain = expr;
        return true;
    }

    //create pairs with first = small, second = big    
    bool leftover = false;
    int leftoverNum;
    std::vector<int>::const_iterator it = expr.begin();
    while(it != expr.end())
    {        
        int a = *it;
        int b;
        if(++it != expr.end())
            b = *it;
        else
        {
            leftover = true;
            leftoverNum = a;
            break;
        }
        if(a < b)
        {
            _pairs.push_back(std::make_pair(a, b));
        }
        else
        {
            _pairs.push_back(std::make_pair(b, a));
        }
        
        ++it;
    }

    //sort the _pairs by the second part
    std::sort(_pairs.begin(), _pairs.end(), compareBySecond);

    //built the mainchain by the _pairs
    std::vector<std::pair<int, int> >::iterator itp = _pairs.begin();    
    size_t i = 0;
    while(itp != _pairs.end())
    {
        _mainchain.push_back(_pairs[i].second);
        ++itp;
        i++;
    }

    //built the _leftoverchain    
    itp = _pairs.begin();    
    i = 0;
    while(itp != _pairs.end())
    {
        _leftoverchain.push_back(_pairs[i].first);
        ++itp;
        i++;
    }
    if (leftover)
    {
        _leftoverchain.push_back(leftoverNum);
    }
   
    //merge the _mainchain with the _leftoverchain
    std::vector<int>::iterator itl = _leftoverchain.begin();
    std::vector<int>::iterator itm;
    size_t compLimit = 0;
    size_t last = _leftoverchain.size() - 1;
    int pairedBig;

    while(itl != _leftoverchain.end())
    {        
        itm = _mainchain.begin();
        i = 0;

        //handle the comparison limit in the mainchain
        if((leftover) && (*itl == leftoverNum) && (*itl == _leftoverchain[last]))
            compLimit = _mainchain.size() -1;
        else
        {
            std::vector<std::pair<int, int> >::iterator itp = _pairs.begin();
            while(itp != _pairs.end())
            {
                if (*itl == itp->first)
                {
                    pairedBig = itp->second;
                    std::vector<int>::iterator it = std::find(_mainchain.begin(), _mainchain.end(), pairedBig);
                    if (it != _mainchain.end())
                    {
                        compLimit = it - _mainchain.begin();
                    }
                }
                itp++;
            }
        }       

        //walk in the mainchain until you find that the small < big)
        while (i <= compLimit) 
        {           
            if(*itl < *itm) //if small < big 
            {
                pairedBig = *itm;
                _mainchain.insert(itm, *itl);
                break;
            }
            else if ((leftover) && (*itl == leftoverNum) && (*itl == _leftoverchain[last]))
            {
                if(leftoverNum > *itm)                
                {
                    if (i == compLimit)
                    {
                        _mainchain.push_back(*itl);
                        return true;
                    }
                }
                else if(leftoverNum < *itm)
                {
                    _mainchain.insert(itm, *itl);
                    return true;
                }
            }
            if(compLimit < _mainchain.size())
            {                
                itm++;
            }
            i++;
            
        }
        ++itl;
    }
   
    return true;
}

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
        long l;
        std::istringstream iss1(token);
        iss1 >> l;
       
        if ((l <= 0) || (l > INT_MAX))
        {
            return false;
        }       
        num = l;
        expr.push_back(num);        

        std::vector<int> sorted = expr;
        std::sort(sorted.begin(), sorted.end());
        if (std::adjacent_find(sorted.begin(), sorted.end()) != sorted.end())
        {
            return false;
        }

        exprd.push_back(num);
    }
    return true;
}

void PmergeMe::printNums(const std::vector<int>& nums, const std::string str)const  
{
    std::vector<int>::const_iterator itm = nums.begin();
    std::cout << str << " ";
    while(itm != nums.end())
    {
        std::cout << *itm << " ";
        ++itm;
    }
    std::cout << std::endl;
}


bool PmergeMe::applyFJAlgoByDeque(const std::deque<int>& exprd)
{
    if(exprd.size() == 1)
    {
        _mainchaind = exprd;
        return true;
    }

    //create pairs with first = small, second = big    
    bool leftover = false;
    int leftoverNum;
    std::deque<int>::const_iterator it = exprd.begin();
    while(it != exprd.end())
    {        
        int a = *it;
        int b;
        if(++it != exprd.end())
            b = *it;
        else
        {
            leftover = true;
            leftoverNum = a;
            break;
        }
        if(a < b)
        {
            _pairsd.push_back(std::make_pair(a, b));
        }
        else
        {
            _pairsd.push_back(std::make_pair(b, a));
        }
        
        ++it;
    }

    //sort the _pairs by the second part
    std::sort(_pairsd.begin(), _pairsd.end(), compareBySecond);
    
    //built the mainchain by the _pairs
    std::deque<std::pair<int, int> >::iterator itp = _pairsd.begin();    
    size_t i = 0;
    while(itp != _pairsd.end())
    {
        _mainchaind.push_back(_pairsd[i].second);
        ++itp;
        i++;
    }

    //built the _leftoverchaind
    itp = _pairsd.begin();
    i = 0;
    while(itp != _pairsd.end())
    {
        _leftoverchaind.push_back(_pairsd[i].first);
        ++itp;
        i++;
    }
    if (leftover)
    {
        _leftoverchaind.push_back(leftoverNum);
    }

    //merge the _mainchaind with the _leftoverchaind
    std::deque<int>::iterator itl = _leftoverchaind.begin();
    std::deque<int>::iterator itm;
    size_t compLimit = 0;
    size_t last = _leftoverchaind.size() - 1;
    int pairedBig;

    // while there are leftover numbers in the deque
    while(itl != _leftoverchaind.end())
    {        
        itm = _mainchaind.begin();
        i = 0; // position 0 of mainchain

        //handle the comparison limit in the mainchain
        if((leftover) && (*itl == leftoverNum) && (*itl == _leftoverchaind[last]))
            compLimit = _mainchaind.size() -1;
        else
        {
            std::deque<std::pair<int, int> >::iterator itp = _pairsd.begin();
            while(itp != _pairsd.end())
            {
                if (*itl == itp->first)
                {
                    pairedBig = itp->second;
                    std::deque<int>::iterator it = std::find(_mainchaind.begin(), _mainchaind.end(), pairedBig);
                    if (it != _mainchaind.end())
                    {
                        compLimit = it - _mainchaind.begin();
                    }
                }
                itp++;
            }
        }       

        //walk in the mainchain until you find that the small < big)
        while (i <= compLimit) 
        {           
            if(*itl < *itm) //if small < big 
            {
                pairedBig = *itm;
                _mainchaind.insert(itm, *itl);
                break;
            }
            else if ((leftover) && (*itl == leftoverNum) && (*itl == _leftoverchaind[last]))
            {
                if(leftoverNum > *itm)                
                {
                    if (i == compLimit)
                    {
                        _mainchaind.push_back(*itl);
                        return true;
                    }
                }
                else if(leftoverNum < *itm)
                {
                    _mainchaind.insert(itm, *itl);
                    return true;
                }
            }
            if(compLimit < _mainchaind.size())
            {                
                itm++;
            }
            i++;
            
        }
        ++itl;
    }
   
    return true;
}

bool compareResults( std::vector<int> _mainchain, std::deque<int> _mainchaind)
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
    _pairs.clear();
    _mainchain.clear();
    _leftoverchain.clear();

    _pairsd.clear();
    _mainchaind.clear();
    _leftoverchaind.clear();
    std::vector<int> expr;
    std::deque<int> exprd;
    
    if(!validateExpression(expression, expr, exprd))
    {
        std::cerr << "Error in validation expr" << std::endl;
        return false;
    }   
    
    clock_t  start_s1=clock();
    if(!applyFJAlgoByVect(expr))
    {
        std::cerr << "Error in applyFJAlgoByVect" << std::endl;
        return false;
    }    
    clock_t  stop_s1=clock();

    clock_t  start_s2=clock(); 
    if(!applyFJAlgoByDeque(exprd))
    {
        std::cerr << "Error in applyFJAlgoByDeque" << std::endl;
        return false;
    }    
    clock_t  stop_s2=clock();

    if(!compareResults(_mainchain, _mainchaind))
    {
        std::cerr << "Error ,not identical results for vector and deque" << std::endl;
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

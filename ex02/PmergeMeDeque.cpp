#include "PmergeMe.hpp"
#include <algorithm>
#include <utility>

static size_t getCurrentDeque(int prev, int prevprev)
{
    size_t current = prev + 2 * prevprev;   
    return current;
}

static int comparisonLimitDeque(std::deque<int>& bigs, std::deque<int>& smalls, 
                                std::deque<std::pair<int, int> >& pairs, 
                                bool leftover, int indexLC, int leftoverNum, size_t last)
{
    //handle the comparison limit in the mainchain
    int compLimit;
    int num = smalls[indexLC];
    if((leftover) && (num == leftoverNum) && (num == smalls[last]))
        compLimit = bigs.size();
    else
    {
        std::deque<std::pair<int, int> >::iterator itp = pairs.begin();
        while(itp != pairs.end())
        {
            if (num == itp->first)
            {
                int pairedBig = itp->second;
                std::deque<int>::iterator it = std::find(bigs.begin(), bigs.end(), pairedBig);
                if (it != bigs.end())
                {
                    compLimit = it - bigs.begin();
                }
            }
            itp++;
        }
    }
    return compLimit;
}

static size_t getMiddleDeque(size_t left, size_t right)
{
    return left + (right - left) / 2;
}

static void binarySearchMergeDeque(std::deque<int>& bigs, std::deque<int>& smalls, size_t compLimit, int il)
{
    int num = smalls[il];
    size_t left = 0;
    size_t right = compLimit;
    size_t middle = getMiddleDeque(left, right);
    while(left != right)
    {
        if(num > bigs[middle])
            left = middle + 1;           
        else 
            right = middle;

        middle = getMiddleDeque(left, right);
    }
    
    bigs.insert(bigs.begin() + left, num);
}

static void jacbAndBinaryDeque(std::deque<int>& bigs, std::deque<int>& smalls, 
                        std::deque<std::pair<int, int> >& pairs,
                        bool leftover, int leftoverNum)
{
    size_t compLimit = 0;
    size_t last = smalls.size() - 1;
    
    int leftchLength = static_cast<int>(smalls.size());
    int il = 0;
    int prevprev = 1;
    int prev = 1; 
    int current;
    while(il < leftchLength)
    {        
        if(il == 0) //run only for 1st element
        {
            compLimit = 0;
            binarySearchMergeDeque(bigs, smalls, compLimit, il);
        }
        else 
        {
            current = getCurrentDeque(prev, prevprev); // current block that I am searching
            if (il == current-1) // if the index is a block
            {
                int bl = il;
                while (bl > (prev-1))
                {
                    compLimit = comparisonLimitDeque(bigs, smalls, pairs, leftover, bl, leftoverNum, last);
                    binarySearchMergeDeque(bigs, smalls, compLimit, bl);
                    bl--;
                }
                prevprev = prev;
                prev = current;
            }
            else if((il == leftchLength-1)) // if it is not a block but it is the last element
            {
                int bl = il;
                while (bl > (prev-1))
                {
                    compLimit = comparisonLimitDeque(bigs, smalls, pairs, leftover, bl, leftoverNum, last);
                    binarySearchMergeDeque(bigs, smalls, compLimit, bl);
                    //std::cout << bl << " ";
                    bl--;
                }
            }
           
        }
        il++;
    }
   
}

static std::deque<int> FordJohnsonDeque(const std::deque<int>& chain)
{
    //create pairs with first = small, second = big
    std::deque<std::pair<int, int> > pairs;
    std::deque<int> bigs;
    std::deque<int> smalls;
    std::deque<int> sorted_bigs;
    bool leftover = false;
    int leftoverNum = 0;
    std::deque<int>::const_iterator it = chain.begin();
    if(chain.size() <= 1)
        return chain;
    while(it != chain.end())
    {        
        int a = *it;
        int b;
        if(++it != chain.end())
            b = *it;
        else
        {
            leftover = true;
            leftoverNum = a;
            break;
        }
        if(a < b)
        {
            pairs.push_back(std::make_pair(a, b));
            bigs.push_back(b);
        }
        else
        {
            pairs.push_back(std::make_pair(b, a));
            bigs.push_back(a);
        }        
        ++it;
    }
    
    //apply FordJohnson recursively
    if(bigs.size() <= 1)
        sorted_bigs = bigs;
    else
        sorted_bigs = FordJohnsonDeque(bigs);
    
    //rebuild smalls
    size_t i = 0;
    while(i < sorted_bigs.size())//(itp != pairs.end())
    {
        std::deque<std::pair<int, int> >::iterator itp = pairs.begin();
        while (itp != pairs.end())
        {
            if (itp->second == sorted_bigs[i])
            {
                smalls.push_back(itp->first);
                break;
            }
            ++itp;
        }
        i++;
    }
    if(leftover)
        smalls.push_back(leftoverNum);

    //merge by jacobsthal sequence and binary insertion 
    jacbAndBinaryDeque(sorted_bigs, smalls, pairs, leftover, leftoverNum);

    //return sorted_bigs to the _mainchaind (or bigs)
    return sorted_bigs;
}

void PmergeMe::applyFJAlgoByDeque(const std::deque<int>& expr)
{
    if(expr.size() == 1)
    {
        _mainchaind = expr;
        return;
    }
    
    _mainchaind = FordJohnsonDeque(expr);
}

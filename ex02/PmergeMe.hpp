#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>

class PmergeMe
{
    private:
        std::vector<int> _mainchain;
        std::deque<int> _mainchaind;
        
        
        void applyFJAlgoByVector(const std::vector<int>& expr);
        void applyFJAlgoByDeque(const std::deque<int>& exprd);
        bool compareResults();

    public:
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator= (const PmergeMe& other);
        ~PmergeMe();

        bool StartProgram(std::string expression);
        
};

#endif
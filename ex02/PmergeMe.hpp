#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <utility>
#include <string>

class PmergeMe
{
    private:
        std::vector<std::pair<int, int> > _pairs;
        std::vector<int> _mainchain;
        std::vector<int> _leftoverchain;

        std::deque<std::pair<int, int> > _pairsd;
        std::deque<int> _mainchaind;
        std::deque<int> _leftoverchaind;
        
        void printNums(const std::vector<int>& nums, const std::string str) const ;
        bool applyFJAlgoByVect(const std::vector<int>& expr);

        bool applyFJAlgoByDeque(const std::deque<int>& exprd);

    public:
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator= (const PmergeMe& other);
        ~PmergeMe();

        bool StartProgram(std::string expression);
        
};

#endif
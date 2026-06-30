#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN
{
    private:
        std::stack<int> _elements;
        bool applyOperator(char opertr);
    public:
        RPN();
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);
        ~RPN();

        
        void calculateExpr(const std::string& elementStr);

};

#endif
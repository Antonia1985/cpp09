#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
    private:
        std::map <std::string, double> _rates;
        void loadDatabase();
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();

        void processInputFile(const std::string& path);
        void calculateBtcValue(const std::string& line);
};

#endif
#include "BitcoinExchange.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <iterator>

static bool isMonthWith31Days(int month)
{
    return (month == 1 || month == 3 || month == 5 || month == 7 ||
            month == 8 || month == 10 || month == 12);
}

static bool validateDate(const std::string& date) //validate format YYYY-MM-DD
{
    if((date.size() != 10))
        return false;
    for (int i=0; i<10; i++)
    {
        if(i == 4 || i ==7)
        {
            if (date[i] != '-')
                return false;
        }
        else 
        {
            if(!isdigit(date[i]))
                return false;
        }
    }
    int month;
    int day;
    int year;
    std::istringstream issm(date.substr(5,2));    
    std::istringstream issd(date.substr(8,2));    
    std::istringstream issy(date.substr(0, 4));   
    if(!(issd >> day) || !(issm >> month)|| !(issy >> year))
        return false;

    if(month < 1 || month > 12)
        return false;
    
    if(day < 1 || day > 31)
        return false;
   
    if (!isMonthWith31Days(month))
    {
        if(day == 31)
            return false;
    }
    if(month == 2)
    {
        if(((year%4) == 0) && ((year%100) != 0)) //it's leap year
        {
            if (day > 29)
                return false;
        }
        else if((year%400) == 0)  //it's still leap year
        {
            if (day > 29)
                return false;
        }
        else // it's normal year
        {
            if (day > 28)
                return false;
        }
    }
    return true;
}

static bool makeDouble(const std::string& str, double& exRate)
{
    char* end;
    double tmp = std::strtod(str.c_str(), &end);

    if (end == str.c_str()) //if no conversion at all happend, because empty or because str starts without number
        return false;
    if (*end != '\0')// if later in the str a letter is found
        return false;

    exRate = tmp;
    return true;
}

void BitcoinExchange::loadDatabase()
{
    std::string line;
    std::string date;
    double exRate;
    std::string pathname = "data.csv";
    std::ifstream database(pathname.c_str());
    if(!database.is_open())
    {
        std::cerr << "Error: could not open " << pathname << std::endl;
        return;
    }
    
    // if 1st line is empty   
    if(!std::getline(database, line))
    {
        std::cerr << "Error: empty database" << std::endl;
            return; 
    }
    // if 1st line is not header
     if(line != "date,exchange_rate")
    {
        std::cerr << "Error: malformed database header" << std::endl;
            return; 
    }

    //else continue with the rest lines
    while(std::getline(database, line))
    {
        std::string::size_type p = line.find(',');
        if(p == std::string::npos)
        {
            std::cerr << "Error: malformed database line in " << pathname << std::endl;
            return;          
        }
        date = line.substr(0, p);
        if (!validateDate(date)) //in case of malformed date print cerr + stop loading
        {
            std::cerr << "Error: malformed date in " << pathname << std::endl;
            return;          
        }
        if (!makeDouble(line.substr(p+1), exRate) || (exRate < 0 )) //if number is empty / has letters / negative 
        {
            std::cerr << "Error: wrong number in " << pathname << std::endl;
            return;            
        }
        _rates[date] = exRate;
    }
    //database.close(); //not necessary because ifstream closes it anyway    
}

BitcoinExchange::BitcoinExchange()
{
    loadDatabase();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _rates(other._rates)
{
  
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if(this != &other)
    {
        _rates = other._rates;
    }

    return (*this);
}

BitcoinExchange::~BitcoinExchange(){}


void BitcoinExchange::processInputFile(const std::string& path)
{
    std::ifstream file(path.c_str());
    if(!file.is_open())
    {
        std::cerr << "Error: could not open " << path << std::endl;
        return;
    }
    std::string line;
    
    // if 1st line is empty   
    if(!std::getline(file, line))
    {
        std::cerr << "Error: empty input file" << std::endl;
        return;
    }
    // if 1st line is not header
    if(line != "date | value")
    {
        std::cerr << "Error: malformed input file header" << std::endl;
    }
    while(std::getline(file, line))
    {
        calculateBtcValue(line);        
    }

}

void BitcoinExchange::calculateBtcValue(const std::string& line)
{
    std::string date;
    double value;
    if (_rates.empty())
    {
        std::cerr << "Error: database not loaded." << std::endl;
        return;
    }
    
    std::string::size_type p = line.find(" | ");
    date = line.substr(0, p);
    if (!validateDate(date)) //in case of malformed date
    {
        std::cerr << "Error: bad input => " << date << std::endl;
        return;          
    }
    if(p == std::string::npos)
    {
        std::cerr << "Error: bad file format." << std::endl;
        return;
    }
    if (!makeDouble(line.substr(p+3), value) ) //if number is empty / has letters
    {
        std::cerr << "Error: bad input => " << line.substr(p+3) << std::endl;
        return;            
    }
    if(value < 0 )
    {
        std::cerr << "Error: not a positive number." << std::endl;
        return;            
    }
    if(value > 1000)
    {
        std::cerr << "Error: too large a number." << std::endl;
        return;            
    }

    std::map<std::string, double>::iterator it = _rates.upper_bound(date);
    if(it == _rates.begin())
    {
        std::cerr << "Error: no exchange rate available before this date." << std::endl;
        return;
    }    
    --it;
    std::cout << date << " => " << value << " = " << (it->second * value) << std::endl;
        
}
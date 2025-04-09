#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "ErrorHandler.h"

class CSVData
{
    public:
        void setExpiration(const int& val) { _expiration = val; }
        void setStockPrice(const double& val) { _stockPrice = val; }
        void setStrikePrice(const int& val) { _strikePrice = val; }
        void setCallPrice(const double& val) { _callPrice = val; }

        int getExpiration() const { return _expiration; }
        double getStockPrice() const { return _stockPrice; }
        int getStrikePrice() const { return _strikePrice; }
        double getCallPrice() const { return _callPrice; }
        void print() const
        {
            std::cout << "Expiration: " << getExpiration() << ", Stock Price: " << getStockPrice()
                      << ", Strike Price: " << getStrikePrice() << ", Call Price: " << getCallPrice() << std::endl;
        }

    private:
        int _expiration;
        double _stockPrice;
        int _strikePrice;
        double _callPrice;
};

std::vector<CSVData> CSVDataReader(const std::string& filename);

#endif // INPUTREADER_H
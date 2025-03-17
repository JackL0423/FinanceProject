#include "../include/optionGreeks.h"

optionGreeks::optionGreeks()
{
    calculateDelta();
    calculateGamma();
    calculateVega();
    calculateTheta();
    calculateRho();
}

optionGreeks::optionGreeks(double underlyingPrice, double strikePrice, double timeToExperation,
                           double riskFreeRate, double volatility) : 
                           blackScholesModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility)
{
    calculateDelta();
    calculateGamma();
    calculateVega();
    calculateTheta();
    calculateRho();
}

void optionGreeks::calculateDelta()
{
    try
    {
        if (isnan(getD1()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }
        double delta = exp(-getD1() * getD1() / 2) / sqrt(2 * M_PI);

        setDelta(delta);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error in calculateDelta: " <<e.what() << '\n';
        setDelta(nan(""));
    }
}

void optionGreeks::calculateGamma()
{
    try
    {
        if (isnan(getD1()) || isnan(getD2()) || isnan(getUnderlyingPrice()) || isnan(getVolatility()) || isnan(getTimeToExperation()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }
        double gamma = exp(-getD2() * getD1() / 2) / (sqrt(2 * M_PI) * getUnderlyingPrice() * getVolatility() * sqrt(getTimeToExperation()));

        setGamma(gamma);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error in calculateGamma: " << e.what() << '\n';
        setGamma(nan(""));
    }
}

void optionGreeks::calculateVega()
{
    try
    {
        if (isnan(getD1()) || isnan(getUnderlyingPrice()) || isnan(getTimeToExperation()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }
        double vega = getUnderlyingPrice() * exp(-getD1() * getD1() / 2) * sqrt(getTimeToExperation());

        setVega(vega);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error in calculateVega: " << e.what() << '\n';
        setVega(nan(""));
    }
}

void optionGreeks::calculateTheta()
{
    try 
    {
        if (isnan(getD1()) || isnan(getD2()) || isnan(getK()) || isnan(getUnderlyingPrice()) || isnan(getVolatility()) || isnan(getTimeToExperation()) || isnan(getRiskFreeRate())) 
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }
        double theta = -getUnderlyingPrice() * exp(-getD1() * getD1() / 2) * getVolatility() / (2* sqrt(getTimeToExperation())) - getRiskFreeRate() * getK() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(getD2());

        setTheta(theta);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error in calculateTheta: " << e.what() << '\n';
        setTheta(nan(""));
    }
}

void optionGreeks::calculateRho()
{
    try
    {
        if (isnan(getD2()) || isnan(getK()) || isnan(getRiskFreeRate()) || isnan(getTimeToExperation()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }
        double rho = getK() * getTimeToExperation() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(getD2());

        setRho(rho);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error in calculateRho: " << e.what() << '\n';
        setRho(nan(""));
    }
}

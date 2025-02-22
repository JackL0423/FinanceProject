#include <iostream>
#include <cmath>
#include <numbers>
#include "../include/blackScholesModel.h"

using namespace std;

void blackScholesModel::calculateD1()
{
    double numerator = log(getUnderlyingPrice()/getStrikePrice()) + (getRiskFreeRate() + 0.5 * getVolatility() * getVolatility()) * getTimeToExperation();
    double denominator = getVolatility() * sqrt(getTimeToExperation());
    
    double d1 = numerator / denominator;

    setD1(d1);
}

void blackScholesModel::calculateD2()
{
    double d2 = getD1() - getVolatility() * sqrt(getTimeToExperation());

    setD2(d2);
}

// May need to double check this equation for the PUT return.
double blackScholesModel::calculateOptionPrice()
{
    switch(getOptionType()) 
    {
        case OptionType::CALL:
            return getUnderlyingPrice() * normalCDF(getD1()) - getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(getD2());
        
        case OptionType::PUT:
            return getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(-getD2()) - getUnderlyingPrice() * normalCDF(-getD1());
    }
}


void blackScholesModel::calculateK()
{
    double K = 1.0 / (1.0 + 0.2316419 * abs(getD1()));

    setK(K);
}


blackScholesModel::blackScholesModel()
{
    setUnderlyingPrice(0.0);
    setStrikePrice(0.0);
    setTimeToExperation(0.0);
    setRiskFreeRate(0.0);
    setVolatility(0.0);
    setOptionType(OptionType::CALL);
    setD1(nan(""));
}

blackScholesModel::blackScholesModel(double underlyingPrice, double strikePrice, double timeToExperation,double riskFreeRate, double volatility)
{

    setUnderlyingPrice(underlyingPrice);
    setStrikePrice(strikePrice);
    setTimeToExperation(timeToExperation);
    setRiskFreeRate(riskFreeRate);
    setVolatility(volatility);
    setOptionType(OptionType::CALL);

    calculateD1();
    calculateD2();
    calculateK();
}

blackScholesModel::blackScholesModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility, OptionType optionType)
{
    setUnderlyingPrice(underlyingPrice);
    setStrikePrice(strikePrice);
    setTimeToExperation(timeToExperation);
    setRiskFreeRate(riskFreeRate);
    setVolatility(volatility);
    setOptionType(optionType);

    calculateD1();
    calculateD2();
    calculateK();
}

void blackScholesModel::setUnderlyingPrice(const double& value) 
{
    _underlyingPrice = value; 
}

void blackScholesModel::setStrikePrice(const double& value) 
{
    _strikePrice = value; 
}

void blackScholesModel::setTimeToExperation(const double& value) 
{
    _timeToExperation = value; 
}

void blackScholesModel::setRiskFreeRate(const double& value) 
{ 
    _riskFreeRate = value; 
}

void blackScholesModel::setVolatility(const double& value) 
{ 
    _volatility = value;
}

void blackScholesModel::setOptionType(const OptionType& option) 
{
    _optionType = option; 
}

void blackScholesModel::setD1(const double& value) const 
{
    _d1 = value;
}

void blackScholesModel::setD2(const double& value) const 
{
    _d2 = value;
}

void blackScholesModel::setK(const double& value) const 
{
    _K = value;
}

const double& blackScholesModel::getUnderlyingPrice() const { return _underlyingPrice; }

const double& blackScholesModel::getStrikePrice() const { return _strikePrice; }

const double& blackScholesModel::getTimeToExperation() const { return _timeToExperation; }

const double& blackScholesModel::getRiskFreeRate() const { return _riskFreeRate; }

const double& blackScholesModel::getVolatility() const { return _volatility; }

const blackScholesModel::OptionType blackScholesModel::getOptionType() const { return _optionType; }

const double& blackScholesModel::getD1() const { return _d1; }

const double& blackScholesModel::getD2() const { return _d2; }

const double& blackScholesModel::getK() const { return _K; }

/// @brief N(x) in Black-Scholes eq.
/// @param d 
/// @return cumilative distribution of d.
double blackScholesModel::normalCDF(double d) const
{
    double L = abs(d);
    double K = getK();

    double denominator = 0.0;
    const double coefficients[2] = {0.0, 0.1};

    for (int i=0; i < 5; i++)
    {
        denominator += coefficients[i] * pow(K, i+1);
    }

    denominator *= sqrt(2 * 3.1415) * exp(-L * L / 2.0);

    double result = 1.0 - 1.0 / denominator;

    return d < 0 ? 1.0 - result : 1.0;
}

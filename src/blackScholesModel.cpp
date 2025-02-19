#include <iostream>
#include <cmath>
#include <numbers>
#include "../include/blackScholesModel.h"

#undef DEBUG

using namespace std;

double blackScholesModel::calculateD1(double underlyingPrice, double strikePrice, double timeToExperication, double riskFreeRate, double volatility) const
{
    double numerator = log(underlyingPrice/strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToExperation;
    double denominator = volatility * sqrt(timeToExperation);
    
    double d1 = numerator / denominator;

    setD1(d1);
    return d1;
}

double blackScholesModel::calculateD2(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility) const
{
    double d2 = calculateD1(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility) - volatility * sqrt(timeToExperation);

    setD2(d2);
    return d2;
}

double blackScholesModel::calculateOptionPrice()
{
    _d1 = calculateD1(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    _d2 = calculateD2(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);

    switch(optionType) 
    {
        case OptionType::CALL:
            return underlyingPrice * normalCDF(_d1) - strikePrice * exp(-riskFreeRate * timeToExperation) * normalCDF(_d2);
        
        case OptionType::PUT:
            return strikePrice * exp(-riskFreeRate * timeToExperation) * normalCDF(-_d2) - underlyingPrice * normalCDF(-_d1);
    }
}


double blackScholesModel::calculateK(double d) const
{
    double K = 1.0 / (1.0 + 0.2316419 * abs(d));

    setK(K);
    return K;
}


blackScholesModel::blackScholesModel()
{
    #ifndef debug
    cout << "Calling default constructor for scholes model...\n";
    #endif
    underlyingPrice = 0.0;
    strikePrice = 0.0;
    timeToExperation = 0.0;
    riskFreeRate = 0.0;
    volatility = 0.0;
    optionType = OptionType::CALL;
}

blackScholesModel::blackScholesModel(double underlyingPrice, double strikePrice, 
                                    double timeToExperation,double riskFreeRate, 
                                    double volatility) : underlyingPrice(underlyingPrice), strikePrice(strikePrice), timeToExperation(timeToExperation),
                                    riskFreeRate(riskFreeRate), volatility(volatility), optionType(OptionType::CALL)
{
    #ifndef debug
    cout << "Calling constructor with no optionType for scholes model...\n";
    #endif

    //  Calculate _d1, _d2, and K
    _d1 = calculateD1(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    _d2 = calculateD2(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    _K = calculateK(_d1);
}

blackScholesModel::blackScholesModel(double underlyingPrice, double strikePrice, double timeToExperation,
                      double riskFreeRate, double volatility, OptionType optionType) : underlyingPrice(underlyingPrice),
                      strikePrice(strikePrice), timeToExperation(timeToExperation), riskFreeRate(riskFreeRate), volatility(volatility),  optionType(optionType)
{
    #ifndef debug
    cout << "Calling constructor with optionType for scholes model...\n";
    #endif

    _d1 = calculateD1(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    _d2 = calculateD2(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    _K = calculateK(_d1);
}


void blackScholesModel::setUnderlyingPrice(const double& value) { underlyingPrice = value; }

void blackScholesModel::setStrikePrice(const double& value) { strikePrice = value; }

void blackScholesModel::setTimeToExperation(const double& value) { timeToExperation = value; }

void blackScholesModel::setRiskFreeRate(const double& value) { riskFreeRate = value; }

void blackScholesModel::setVolatility(const double& value) { volatility = value; }

void blackScholesModel::setOptionType(const OptionType& option) { optionType = option; }

void blackScholesModel::setD1(const double& value) const { _d1 = value; }

void blackScholesModel::setD2(const double& value) const { _d2 = value; }

void blackScholesModel::setK(const double& value) const { _K = value; }


const double& blackScholesModel::getUnderlyingPrice() const { return underlyingPrice; }

const double& blackScholesModel::getStrikePrice() const { return strikePrice; }

const double& blackScholesModel::getTimeToExperation() const { return timeToExperation; }

const double& blackScholesModel::getRiskFreeRate() const { return riskFreeRate; }

const double& blackScholesModel::getVolatility() const { return volatility; }

const blackScholesModel::OptionType blackScholesModel::getOptionType() const { return optionType; }

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

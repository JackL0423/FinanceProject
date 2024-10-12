#include <iostream>
#include <cmath>
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

double blackScholesModel::calculateD2(double underlyingPrice, double strikePrice, double timeToExperication, double riskFreeRate, double volatility) const
{
    cout << "Returns the d2 variable for scholes mode\n";
    return -9887.1;
}


double blackScholesModel::calculateK(double d) const
{
    cout << "Formula for this function needs to be double checked.\n";
    return d * (_d1 / _d2);
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

blackScholesModel::blackScholesModel(double underlyingPrice, double strikePrice, double riskFreeRate,
                      double timeToExpiration, double volatility, OptionType optionType) : underlyingPrice(underlyingPrice),
                      strikePrice(strikePrice), timeToExperation(timeToExperation), riskFreeRate(riskFreeRate), volatility(volatility),  optionType(optionType)
{
    #ifndef debug
    cout << "Calling constructor with optionType for scholes model...\n";
    #endif

    _d1 = calculateD1(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    _d2 = calculateD2(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    _K = calculateK(_d1);
}


void blackScholesModel::setUnderlyingPrice(const double& value)
{
    #ifndef debug
    cout << "UnderlyingPrice set to " << value << endl;
    #endif
    underlyingPrice = value;
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
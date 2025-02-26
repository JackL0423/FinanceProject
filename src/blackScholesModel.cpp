#include <iostream>
#include <cmath>
#include <numbers>
#include "../include/blackScholesModel.h"

using namespace std;

/// @brief calculates and sets the d1 value in the Black-Scholes model. 
void blackScholesModel::calculateD1()
{
    double numerator = log(getUnderlyingPrice()/getStrikePrice()) + (getRiskFreeRate() + 0.5 * getVolatility() * getVolatility()) * getTimeToExperation();
    double denominator = getVolatility() * sqrt(getTimeToExperation());
    
    double d1 = numerator / denominator;

    setD1(d1);
}

/// @brief calculates and sets the d2 value in the Black-Scholes model.
void blackScholesModel::calculateD2()
{
    double d2 = getD1() - getVolatility() * sqrt(getTimeToExperation());

    setD2(d2);
}


/// @brief calculates the option price using the Black-Scholes model.
/// @return the option price.
double blackScholesModel::calculateOptionPrice()
{
    switch(getOptionType()) 
    {
        case CALL:
            return getUnderlyingPrice() * normalCDF(getD1()) - getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(getD2());
        
        case PUT:
            return getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(-getD2()) - getUnderlyingPrice() * normalCDF(-getD1());
        default:
            return nan("");
    }
}


/// @brief calculates and sets the K value in the Black-Scholes model.
void blackScholesModel::calculateK()
{
    double K = 1.0 / (1.0 + 0.2316419 * abs(getD1()));

    setK(K);
}


/// @brief default constructor for the Black-Scholes model.
blackScholesModel::blackScholesModel()
{
    //  TODO: #3 [refactoring] Look at default values for scholes model. May be better to set all default as NaN and do checks.
    setUnderlyingPrice(0.0);
    setStrikePrice(0.0);
    setTimeToExperation(0.0);
    setRiskFreeRate(0.0);
    setVolatility(0.0);
    //setOptionType(OptionType::CALL);
    setD1(nan(""));
    setD2(nan(""));
    setK(nan(""));
}


/// @brief constructor with no option type for Black-Scholes model.
/// @param underlyingPrice 
/// @param strikePrice 
/// @param timeToExperation 
/// @param riskFreeRate 
/// @param volatility 
blackScholesModel::blackScholesModel(double underlyingPrice, double strikePrice, double timeToExperation,double riskFreeRate, double volatility)
{

    setUnderlyingPrice(underlyingPrice);
    setStrikePrice(strikePrice);
    setTimeToExperation(timeToExperation);
    setRiskFreeRate(riskFreeRate);
    setVolatility(volatility);
    //setOptionType(OptionType::CALL);

    calculateD1();
    calculateD2();
    calculateK();
}


/// @brief constructor with option type for Black-Scholes model.
/// @param underlyingPrice 
/// @param strikePrice 
/// @param timeToExperation 
/// @param riskFreeRate 
/// @param volatility 
/// @param optionType 
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

/// @brief sets the underlying price in the Black-Scholes model.
/// @param value 
void blackScholesModel::setUnderlyingPrice(const double& value) 
{
    _underlyingPrice = value; 
}

/// @brief sets the strike price in the Black-Scholes model.
/// @param value 
void blackScholesModel::setStrikePrice(const double& value) 
{
    _strikePrice = value; 
}

/// @brief sets the time to experation in the Black-Scholes model.
/// @param value 
void blackScholesModel::setTimeToExperation(const double& value) 
{
    _timeToExperation = value; 
}

/// @brief sets the risk free rate in the Black-Scholes model.
/// @param value 
void blackScholesModel::setRiskFreeRate(const double& value) 
{ 
    _riskFreeRate = value; 
}

/// @brief sets the volatility in the Black-Scholes model.
/// @param value 
void blackScholesModel::setVolatility(const double& value) 
{ 
    _volatility = value;
}

/// @brief sets the option type in the Black-Scholes model.
/// @param option 
void blackScholesModel::setOptionType(const OptionType& option) 
{
    _optionType = option; 
}

/// @brief sets the D1 value in the Black-Scholes model.
/// @param value 
void blackScholesModel::setD1(const double& value) const 
{
    _d1 = value;
}

/// @brief sets the D2 value in the Black-Scholes model.
/// @param value 
void blackScholesModel::setD2(const double& value) const 
{
    _d2 = value;
}

/// @brief sets the K value in the Black-Scholes model.
/// @param value 
void blackScholesModel::setK(const double& value) const 
{
    _K = value;
}

/// @brief getter for the underlying price in the Black-Scholes model.
/// @return _underlyingPrice
const double& blackScholesModel::getUnderlyingPrice() const { return _underlyingPrice; }

/// @brief getter for the strike price in the Black-Scholes model.
/// @return _strikePrice
const double& blackScholesModel::getStrikePrice() const { return _strikePrice; }

/// @brief getter for the time to experation in the Black-Scholes model.
/// @return _timeToExperation
const double& blackScholesModel::getTimeToExperation() const { return _timeToExperation; }

/// @brief getter for the risk free rate in the Black-Scholes model.
/// @return _riskFreeRate
const double& blackScholesModel::getRiskFreeRate() const { return _riskFreeRate; }

/// @brief getter for the volatility in the Black-Scholes model.
/// @return _volatility
const double& blackScholesModel::getVolatility() const { return _volatility; }

/// @brief getter for the option type in the Black-Scholes model.
/// @return _optionType
const blackScholesModel::OptionType blackScholesModel::getOptionType() const { return _optionType; }

/// @brief getter for the D1 value in the Black-Scholes model.
/// @return _d1
const double& blackScholesModel::getD1() const { return _d1; }

/// @brief getter for the D2 value in the Black-Scholes model.
/// @return _d2
const double& blackScholesModel::getD2() const { return _d2; }

/// @brief getter for the K value in the Black-Scholes model.
/// @return _K
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

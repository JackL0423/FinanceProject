#include <iostream>
#include <cmath>
#include <numbers>
#include "../include/blackScholesModel.h"
#include "../include/ErrorHandler.h"

using namespace std;

/// @brief calculates and sets the d1 value in the Black-Scholes model. 
void blackScholesModel::calculateD1()
{
    try
    {
        if (isnan(getUnderlyingPrice()) || isnan(getStrikePrice()) || isnan(getRiskFreeRate()) || isnan(getVolatility()) || isnan(getTimeToExperation()))
        {
            setD1(nan(""));
            return;
        }
        double numerator = log(getUnderlyingPrice()/getStrikePrice()) + (getRiskFreeRate() + 0.5 * getVolatility() * getVolatility()) * getTimeToExperation();
        double denominator = getVolatility() * sqrt(getTimeToExperation());
        
        double d1 = numerator / denominator;

        setD1(d1);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateD1: " + std::string(e.what()));
        setD1(nan(""));
    }
}

/// @brief calculates and sets the d2 value in the Black-Scholes model.
void blackScholesModel::calculateD2()
{
    try
    {
        if (isnan(getD1()) || isnan(getVolatility()) || isnan(getTimeToExperation()))
        {
            setD2(nan(""));
            return;
        }
        double d2 = getD1() - getVolatility() * sqrt(getTimeToExperation());

        setD2(d2);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateD2: " + std::string(e.what()));
        setD2(nan(""));
    }
}


/// @brief calculates the option price using the Black-Scholes model.
/// @return the option price.
double blackScholesModel::calculateOptionPrice()
{
    try
    {
        if (isnan(getD1()) || isnan(getD2()))
        {
            return nan("");
        }
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
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateOptionPrice: " + std::string(e.what()));
        return nan("");
    }
}


/// @brief calculates and sets the K value in the Black-Scholes model.
void blackScholesModel::calculateK()
{
    try
    {
        if (isnan(getD1()))
        {
            setK(nan(""));
            return;
        }
        double K = 1.0 / (1.0 + 0.2316419 * abs(getD1()));

        setK(K);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateK: " + std::string(e.what()));
        setK(nan(""));
    }
}


/// @brief default constructor for the Black-Scholes model.
blackScholesModel::blackScholesModel()
{
    //  TODO: #3 [refactoring] Look at default values for scholes model. May be better to set all default as NaN and do checks.
    setUnderlyingPrice(nan(""));
    setStrikePrice(nan(""));
    setTimeToExperation(nan(""));
    setRiskFreeRate(nan(""));
    setVolatility(nan(""));
    //setOptionType(OptionType::CALL);
    calculateD1();
    calculateD2();
    calculateK();
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
    try
    {
        if (value < 0.0)
        {
            throw invalid_argument("Invalid input: timeToExpiration must be greater than or equal to 0");
        }
        _timeToExperation = value;
    }
    catch(const std::exception& e)
    {
        ErrorHandler::logError("Error in setTimeToExperation: " + std::string(e.what()));
        _timeToExperation = nan("");
    } 
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
    try
    {
        if (value <= 0.0 || value >= 1.0)
        {
            throw invalid_argument("Invalid input: Volatility must be greater than or equal to 0");
        }
        _volatility = value;
    }
    catch(const std::exception& e)
    {
        ErrorHandler::logError("Error in setVolatility: " + std::string(e.what()));
        _volatility = nan("");
    }
    
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
const OptionType blackScholesModel::getOptionType() const { return _optionType; }

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
const double blackScholesModel::normalCDF(const double& d) const
{
    double z = abs(d);
    double K = 1.0 / (1.0 + 0.2316419 * z);

    if (isnan(d))
    {
        return nan("");
    }

    double a1 = 0.319381530;
    double a2 = -0.356563782;
    double a3 = 1.781477937;
    double a4 = -1.821255978;
    double a5 = 1.330274429;

    double y = 1.0 - (1.0 / sqrt(2 * M_PI)) * exp(-0.5 * z * z) * 
               (a1 * K + a2 * K * K + a3 * K * K * K + a4 * K * K * K * K + a5 * K * K * K * K * K);

    return d < 0 ? 1.0 - y : y;
}

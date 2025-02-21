#include <iostream>
#include <cmath>
#include <numbers>
#include "../include/blackScholesModel.h"

#undef DEBUG

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
    switch(_optionType) 
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
    #ifndef debug
    cout << "Calling default constructor for scholes model...\n";
    #endif
    setUnderlyingPrice(0.0);
    setStrikePrice(0.0);
    setTimeToExperation(0.0);
    setRiskFreeRate(0.0);
    setVolatility(0.0);
    setOptionType(OptionType::CALL);
}

blackScholesModel::blackScholesModel(double underlyingPrice, double strikePrice, double timeToExperation,double riskFreeRate, double volatility)
{
    #ifndef debug
    cout << "Calling constructor with no optionType for scholes model...\n";
    #endif

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
    #ifndef debug
    cout << "Calling constructor with optionType for scholes model...\n";
    #endif

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
    #ifndef debug
    cout << "Setting UnderlyingPrice = " << value << endl;
    #endif
    _underlyingPrice = value; 
}

void blackScholesModel::setStrikePrice(const double& value) 
{
    #ifndef debug
    cout << "Setting StrikePrice = " << value << endl;
    #endif 
    _strikePrice = value; 
}

void blackScholesModel::setTimeToExperation(const double& value) 
{
    #ifndef debug
    cout << "Setting timeToExperation = " << value << endl; 
    #endif
    _timeToExperation = value; 
}

void blackScholesModel::setRiskFreeRate(const double& value) 
{ 
    #ifndef debug
    cout << "Setting riskFreeRate = " << value << endl;
    #endif
    _riskFreeRate = value; 
}

void blackScholesModel::setVolatility(const double& value) 
{ 
    #ifndef debug
    cout << "Setting volatility = " << value << endl;
    #endif
    _volatility = value;
}

void blackScholesModel::setOptionType(const OptionType& option) 
{
    #ifndef debug
    cout << "Setting OptionType = " << option << endl;
    #endif 
    _optionType = option; 
}

void blackScholesModel::setD1(const double& value) const 
{
    #ifndef debug
    cout << "Setting _d1 = " << value << endl;
    #endif
    _d1 = value;
}

void blackScholesModel::setD2(const double& value) const 
{
    #ifndef debug
    cout << "Setting _d2 = " << value << endl;
    #endif
    _d2 = value;
}

void blackScholesModel::setK(const double& value) const 
{
    #ifndef debug
    cout << "Setting _K = " << value << endl;
    #endif
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

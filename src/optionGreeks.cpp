#include "../include/optionGreeks.h"

optionGreeks::optionGreeks()
{
    _delta = getDelta();
    _gamma = getGamma();
    _vega = getVega();
    _theta = getTheta();
    _rho = getRho();
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
    double delta = exp(-getD1() * getD1() / 2) / sqrt(2 * M_PI);

    setDelta(delta);
}

void optionGreeks::calculateGamma()
{
    double gamma = exp(-getD2() * getD1() / 2) / (sqrt(2 * M_PI) * getUnderlyingPrice() * getVolatility() * sqrt(getTimeToExperation()));

    setGamma(gamma);
}

void optionGreeks::calculateVega()
{
    double vega = getUnderlyingPrice() * exp(-getD1() * getD1() / 2) * sqrt(getTimeToExperation());

    setVega(vega);
}

void optionGreeks::calculateTheta()
{
    double theta = -getUnderlyingPrice() * exp(-getD1() * getD1() / 2) * getVolatility() / (2* sqrt(getTimeToExperation())) - getRiskFreeRate() * getK() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(getD2());

    setTheta(theta);
}

void optionGreeks::calculateRho()
{
    double rho = getK() * getTimeToExperation() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(getD2());

    setRho(rho);
}


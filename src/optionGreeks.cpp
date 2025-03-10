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
    if (isnan(getD1()))
    {
        setDelta(nan(""));
        return;
    }
    double delta = exp(-getD1() * getD1() / 2) / sqrt(2 * M_PI);

    setDelta(delta);
}

void optionGreeks::calculateGamma()
{
    if (isnan(getD1()) || isnan(getD2()))
    {
        setGamma(nan(""));
        return;
    }
    double gamma = exp(-getD2() * getD1() / 2) / (sqrt(2 * M_PI) * getUnderlyingPrice() * getVolatility() * sqrt(getTimeToExperation()));

    setGamma(gamma);
}

void optionGreeks::calculateVega()
{
    if (isnan(getD1()))
    {
        setVega(nan(""));
        return;
    }
    double vega = getUnderlyingPrice() * exp(-getD1() * getD1() / 2) * sqrt(getTimeToExperation());

    setVega(vega);
}

void optionGreeks::calculateTheta()
{
    if (isnan(getD1()) || isnan(getD2()))
    {
        setTheta(nan(""));
        return;
    }
    double theta = -getUnderlyingPrice() * exp(-getD1() * getD1() / 2) * getVolatility() / (2* sqrt(getTimeToExperation())) - getRiskFreeRate() * getK() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(getD2());

    setTheta(theta);
}

void optionGreeks::calculateRho()
{
    if (isnan(getD2()) || isnan(getK()))
    {
        setRho(nan(""));
        return;
    }
    double rho = getK() * getTimeToExperation() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(getD2());

    setRho(rho);
}


#include "optionGreeks.h"

optionGreeks::optionGreeks()
{
    _d1 = getD1();
    _d2 = getD2();
    _K = getK();
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
    _d1 = getD1();
    _d2 = getD2();
    _K = getK();
    _delta = getDelta();
    _gamma = getGamma();
    _vega = getVega();
    _theta = getTheta();
    _rho = getRho();
}

double optionGreeks::calculateDelta() const
{
    double delta = exp(-_d1 * _d1 / 2) / sqrt(2 * M_PI);

    setDelta(delta);

    return delta;
}

double optionGreeks::calculateGamma() const
{
    double gamma = exp(-_d1 * _d1 / 2) / (sqrt(2 * M_PI) * underlyingPrice * volatility * sqrt(timeToExperation));

    setGamma(gamma);

    return gamma;
}

double optionGreeks::calculateVega() const
{
    double vega = underlyingPrice * exp(-_d1 * _d1 / 2) * sqrt(timeToExperation);

    setVega(vega);

    return vega;
}

double optionGreeks::calculateTheta() const
{
    double theta = -underlyingPrice * exp(-_d1 * _d1 / 2) * volatility / (2* sqrt(timeToExperation)) - riskFreeRate * _K * exp(-riskFreeRate * timeToExperation) * normalCDF(_d2);

    setTheta(theta);

    return theta;
}

double optionGreeks::calculateRho() const
{
    double rho = _K * timeToExperation * exp(-riskFreeRate * timeToExperation) * normalCDF(_d2);

    setRho(rho);

    return rho;
}


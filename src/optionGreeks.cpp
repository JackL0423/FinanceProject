#include "optionGreeks.h"

optionGreeks::optionGreeks()
{
    _d1 = getD1();
    _d2 = getD2();
    _K = getK();
}

optionGreeks::optionGreeks(double underlyingPrice, double strikePrice, double timeToExperation,
                           double riskFreeRate, double volatility) : blackScholesModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility)
{
    _d1 = getD1();

}
double optionGreeks::calculateDelta() const
{
    return 0.0;
}

double optionGreeks::calculateGamma() const
{
    return 0.0;
}

double optionGreeks::calculateVega() const
{
    return 0.0;
}

double optionGreeks::calculateTheta() const
{
    return 0.0;
}

double optionGreeks::calculateRho() const
{
    return 0.0;
}


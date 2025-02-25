#include "../include/optionGreeksModel.h"

optionGreeksModel::optionGreeksModel()
{
    // does something
}

optionGreeksModel::optionGreeksModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility)
    : optionGreeks(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility)
{
    // does something
}

void optionGreeksModel::setOptionPriceIV(double value) const
{
    _optionPriceIv = value;
}

void optionGreeksModel::setOptionPriceGamma(double value) const
{
    _optionPriceGamma = value;
}

void optionGreeksModel::setOptionPriceVega(double value) const
{
    _optionPriceVega = value;
}

void optionGreeksModel::setOptionPriceTheta(double value) const
{
    _optionPriceTheta = value;
}

void optionGreeksModel::setOptionPriceGammaVega(double value) const
{
    _optionPriceGammaVega = value;
}

void optionGreeksModel::setIVAdjustedDelta(double value) const
{
    _ivAdjustedDelta = value;
}

void optionGreeksModel::setGammaAdjustedDelta(double value) const
{
    _gammaAdjustedDelta = value;
}

void optionGreeksModel::setVegaAdjustedDelta(double value) const
{
    _vegaAdjustedDelta = value;
}

void optionGreeksModel::setThetaAdjustedDelta(double value) const
{
    _thetaAdjustedDelta = value;
}

void optionGreeksModel::setGammaVegaAdjustedDelta(double value) const
{
    _gammaVegaAdjustedDelta = value;
}

const double& optionGreeksModel::getOptionPriceIV() const { return _optionPriceIv;}

const double& optionGreeksModel::getOptionPriceGamma() const { return _optionPriceGamma; }

const double& optionGreeksModel::getOptionPriceVega() const { return _optionPriceVega;}

const double& optionGreeksModel::getOptionPriceTheta() const { return _optionPriceTheta; }

const double& optionGreeksModel::getOptionPriceGammaVega() const { return _optionPriceGammaVega;}

const double& optionGreeksModel::getIVAdjustedDelta() const { return _ivAdjustedDelta;}

const double& optionGreeksModel::getGammaAdjustedDelta() const { return _gammaAdjustedDelta; }

const double& optionGreeksModel::getVegaAdjustedDelta() const { return _vegaAdjustedDelta; }

const double& optionGreeksModel::getThetaAdjustedDelta() const { return _thetaAdjustedDelta;}

const double& optionGreeksModel::getGammaVegaAdjustedDelta() const { return _gammaVegaAdjustedDelta; }

    
void optionGreeksModel::calculateOptionPriceIV(double impliedVolatility) const
{
    double delta = getIVAdjustedDelta();

    double d1 = getD1();    // Need to make overloaded function with impliedVolatility
    double d2 = d1 - (impliedVolatility * sqrt(getTimeToExperation())); // Make into overloaded func.

    double optionPriceIV = (delta * getUnderlyingPrice() * normalCDF(d1)) 
                            - (getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(d2));

    setOptionPriceIV(optionPriceIV);
}

void optionGreeksModel::calculateOptionPriceGamma() const
{
    double discountedStrikePrice = getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation());
    double adjustedOptionPriceGamma = (getGammaAdjustedDelta() * getUnderlyingPrice()) - discountedStrikePrice;

    setOptionPriceGamma(adjustedOptionPriceGamma);
}

void optionGreeksModel::calculateOptionPriceVega() const
{
    double optionPriceVega = getDelta() * getUnderlyingPrice() - getVega() * getVegaAdjustedDelta() + (0.5 * getVega() * getVega());

    setOptionPriceVega(optionPriceVega);
}

void optionGreeksModel::calculateOptionPriceTheta() const
{
    double something = 0.0;
}

void optionGreeksModel::calculateOptionPriceGammaVega() const
{
    
}
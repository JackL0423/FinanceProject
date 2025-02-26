#include "../include/optionGreeksModel.h"

optionGreeksModel::optionGreeksModel()
{
    // TODO: #2 write optionGreeksModel default constructor
}

optionGreeksModel::optionGreeksModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility)
    : optionGreeks(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility)
{
    // TODO: #4 [enhancement] Initialize the optionGreeksModel with given parameters
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

    // TODO: #1 write overloaded function for calculateD1(imlpiedVolatility)
    double d1 = getD1();
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
    double optionPriceTheta = getDelta() * getUnderlyingPrice() - getTheta() * getTimeToExperation() + getThetaAdjustedDelta() - (getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()));

    setOptionPriceTheta(optionPriceTheta);
}

void optionGreeksModel::calculateOptionPriceGammaVega() const
{
    double optionPriceGammaVega = getGammaAdjustedDelta() * getUnderlyingPrice() + 0.5 * getGamma() * pow(getUnderlyingPrice(),2) - getVega() * (getUnderlyingPrice() * sqrt(getTimeToExperation()) * exp(-getRiskFreeRate() * getTimeToExperation()) / 100.0);

    setOptionPriceGammaVega(optionPriceGammaVega);
}

void optionGreeksModel::calculateIVAdjustedDelta(double impliedVolatility) const
{
    double d1 = getD1();
    
    double ivAdjustedDelta = normalCDF(d1);

    setIVAdjustedDelta(ivAdjustedDelta);
}

void optionGreeksModel::calculateGammaAdjustedDelta() const
{
    double gammaAdjustedDelta = getDelta() + (0.5 * getGamma());

    setGammaAdjustedDelta(gammaAdjustedDelta);
}

void optionGreeksModel::calculateVegaAdjustedDelta() const
{
    double vegaAdjustedDelta = getDelta() + getVega();

    setVegaAdjustedDelta(vegaAdjustedDelta);
}

void optionGreeksModel::calculateThetaAdjustedDelta() const
{
    double thetaAdjustedDelta = getDelta() - getTheta();

    setThetaAdjustedDelta(thetaAdjustedDelta);
}

void optionGreeksModel::calculateGammaVegaAdjustedDelta() const
{
    double gammaVegaAdjustedDelta = getDelta() + (0.5 * getGamma()) + getVega();

    setGammaVegaAdjustedDelta(gammaVegaAdjustedDelta);
}

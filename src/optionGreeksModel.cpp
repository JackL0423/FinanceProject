#include "../include/optionGreeksModel.h"
#include "../include/ErrorHandler.h"
#include <stdexcept>
#include <iostream>

optionGreeksModel::optionGreeksModel()
{
    calculateOptionPriceIV();
    calculateOptionPriceGamma();
    calculateOptionPriceVega();
    calculateOptionPriceTheta();
    calculateOptionPriceGammaVega();
    calculateIVAdjustedDelta();
    calculateGammaAdjustedDelta();
    calculateVegaAdjustedDelta();
    calculateThetaAdjustedDelta();
    calculateGammaVegaAdjustedDelta();
}

optionGreeksModel::optionGreeksModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility)
    : optionGreeks(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility), blackScholesModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility) 
{
    // TODO: #4 [enhancement] Initialize the optionGreeksModel with given parameters
    //  Get rid of the parameters volatility to use implied volatility
    calculateOptionPriceIV();
    calculateOptionPriceGamma();
    calculateOptionPriceVega();
    calculateOptionPriceTheta();
    calculateOptionPriceGammaVega();
    calculateIVAdjustedDelta();
    calculateGammaAdjustedDelta();
    calculateVegaAdjustedDelta();
    calculateThetaAdjustedDelta();
    calculateGammaVegaAdjustedDelta();
}

void optionGreeksModel::setOptionPriceIV(const double& value) const
{
    _optionPriceIv = value;
}

void optionGreeksModel::setOptionPriceGamma(const double& value) const
{
    _optionPriceGamma = value;
}

void optionGreeksModel::setOptionPriceVega(const double& value) const
{
    _optionPriceVega = value;
}

void optionGreeksModel::setOptionPriceTheta(const double& value) const
{
    _optionPriceTheta = value;
}

void optionGreeksModel::setOptionPriceGammaVega(const double& value) const
{
    _optionPriceGammaVega = value;
}

void optionGreeksModel::setIVAdjustedDelta(const double& value) const
{
    _ivAdjustedDelta = value;
}

void optionGreeksModel::setGammaAdjustedDelta(const double& value) const
{
    _gammaAdjustedDelta = value;
}

void optionGreeksModel::setVegaAdjustedDelta(const double& value) const
{
    _vegaAdjustedDelta = value;
}

void optionGreeksModel::setThetaAdjustedDelta(const double& value) const
{
    _thetaAdjustedDelta = value;
}

void optionGreeksModel::setGammaVegaAdjustedDelta(const double& value) const
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


double optionGreeksModel::calculateImpliedVolatility() const
{
    try
    {
        if (isnan(getD1()) || isnan(getUnderlyingPrice()) || isnan(getStrikePrice()) || isnan(getRiskFreeRate()) || isnan(getTimeToExperation()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double d1 = getD1();
        double d2 = d1 - (getVolatility() * sqrt(getTimeToExperation()));

        double impliedVolatility = (getUnderlyingPrice() * normalCDF(d1) - getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(d2)) / getUnderlyingPrice();

        return impliedVolatility;
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateImpliedVolatility: " + std::string(e.what()));
        return nan("");
    }
}


void optionGreeksModel::calculateOptionPriceIV() const
{
    try
    {
        if (isnan(getD1()) || isnan(getUnderlyingPrice()) || isnan(getStrikePrice()) || isnan(getRiskFreeRate()) || isnan(getTimeToExperation()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }
        double impliedVol = calculateImpliedVolatility();
        double delta = getIVAdjustedDelta();

        calculateD1(impliedVol);
        double d1 = getD1();
        double d2 = d1 - (impliedVol * sqrt(getTimeToExperation()));

        double optionPriceIV = (delta * getUnderlyingPrice() * normalCDF(d1)) 
                                - (getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(d2));

        setOptionPriceIV(optionPriceIV);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateOptionPriceIV: " + std::string(e.what()));
        setOptionPriceIV(nan(""));
    }
}

void optionGreeksModel::calculateOptionPriceGamma() const
{
    try
    {
        if (isnan(getStrikePrice()) || isnan(getRiskFreeRate()) || isnan(getTimeToExperation()) || isnan(getUnderlyingPrice()) || isnan(getGammaAdjustedDelta()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double discountedStrikePrice = getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation());
        double adjustedOptionPriceGamma = (getGammaAdjustedDelta() * getUnderlyingPrice()) - discountedStrikePrice;

        setOptionPriceGamma(adjustedOptionPriceGamma);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateOptionPriceGamma: " + std::string(e.what()));
        setOptionPriceGamma(nan(""));
    }
}

void optionGreeksModel::calculateOptionPriceVega() const
{
    try
    {
        if (isnan(getDelta()) || isnan(getUnderlyingPrice()) || isnan(getVega()) || isnan(getVegaAdjustedDelta()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double optionPriceVega = getDelta() * getUnderlyingPrice() - getVega() * getVegaAdjustedDelta() + (0.5 * getVega() * getVega());

        setOptionPriceVega(optionPriceVega);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateOptionPriceVega: " + std::string(e.what()));
        setOptionPriceVega(nan(""));
    }
}

void optionGreeksModel::calculateOptionPriceTheta() const
{
    try
    {
        if (isnan(getDelta()) || isnan(getUnderlyingPrice()) || isnan(getTheta()) || isnan(getThetaAdjustedDelta()) || isnan(getRiskFreeRate()) || isnan(getTimeToExperation()) || isnan(getStrikePrice()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double optionPriceTheta = getDelta() * getUnderlyingPrice() - getTheta() * getTimeToExperation() + getThetaAdjustedDelta() - (getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()));

        setOptionPriceTheta(optionPriceTheta);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateOptionPriceTheta: " + std::string(e.what()));
        setOptionPriceTheta(nan(""));
    }
}

void optionGreeksModel::calculateOptionPriceGammaVega() const
{
    try
    {
        if (isnan(getGammaAdjustedDelta()) || isnan(getUnderlyingPrice()) || isnan(getGamma()) || isnan(getVega()) || isnan(getTimeToExperation()) || isnan(getRiskFreeRate()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double optionPriceGammaVega = getGammaAdjustedDelta() * getUnderlyingPrice() + 0.5 * getGamma() * pow(getUnderlyingPrice(),2) - getVega() * (getUnderlyingPrice() * sqrt(getTimeToExperation()) * exp(-getRiskFreeRate() * getTimeToExperation()) / 100.0);

        setOptionPriceGammaVega(optionPriceGammaVega);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateOptionPriceGammaVega: " + std::string(e.what()));
        setOptionPriceGammaVega(nan(""));
    }
}

void optionGreeksModel::calculateIVAdjustedDelta() const
{
    try
    {
        if (isnan(getD1()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }
        double impliedVol = calculateImpliedVolatility();
        double d1 = getD1();
        double ivAdjustedDelta = normalCDF(d1);

        setIVAdjustedDelta(ivAdjustedDelta);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateIVAdjustedDelta: " + std::string(e.what()));
        setIVAdjustedDelta(nan(""));
    }
}

void optionGreeksModel::calculateGammaAdjustedDelta() const
{
    try
    {
        if (isnan(getD1()) || isnan(getGamma()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double gammaAdjustedDelta = getDelta() + (0.5 * getGamma());

        setGammaAdjustedDelta(gammaAdjustedDelta);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateGammaAdjustedDelta: " + std::string(e.what()));
        setGammaAdjustedDelta(nan(""));
    }
}

void optionGreeksModel::calculateVegaAdjustedDelta() const
{
    try
    {
        if (isnan(getDelta()) || isnan(getVega()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double vegaAdjustedDelta = getDelta() + getVega();

        setVegaAdjustedDelta(vegaAdjustedDelta);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateVegaAdjustedDelta: " + std::string(e.what()));
        setVegaAdjustedDelta(nan(""));
    }
}

void optionGreeksModel::calculateThetaAdjustedDelta() const
{
    try
    {
        if (isnan(getDelta()) || isnan(getTheta()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double thetaAdjustedDelta = getDelta() - getTheta();

        setThetaAdjustedDelta(thetaAdjustedDelta);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateThetaAdjustedDelta: " + std::string(e.what()));
        setThetaAdjustedDelta(nan(""));
    }
}

void optionGreeksModel::calculateGammaVegaAdjustedDelta() const
{
    try
    {
        if (isnan(getDelta()) || isnan(getGamma()) || isnan(getVega()))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double gammaVegaAdjustedDelta = getDelta() + (0.5 * getGamma()) + getVega();

        setGammaVegaAdjustedDelta(gammaVegaAdjustedDelta);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateGammaVegaAdjustedDelta: " + std::string(e.what()));
        setGammaVegaAdjustedDelta(nan(""));
    }
}

void optionGreeksModel::calculateD1(double impliedVolatility) const
{
    try
    {
        if (isnan(getUnderlyingPrice()) || isnan(getStrikePrice()) || isnan(getRiskFreeRate()) || isnan(getTimeToExperation()) || isnan(impliedVolatility))
        {
            throw std::invalid_argument("Invalid input: NaN value detected");
        }

        double numerator = log(getUnderlyingPrice()/getStrikePrice()) + (getRiskFreeRate() + 0.5 * impliedVolatility * impliedVolatility) * getTimeToExperation();
        double denominator = impliedVolatility * sqrt(getTimeToExperation());

        double d1 = numerator / denominator;
        setD1(d1);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::logError("Error in calculateD1: " + std::string(e.what()));
        setD1(nan(""));
    }
}

//  Rework tests to check all these calculations when params are empty.
//  Need to create copy constructors?
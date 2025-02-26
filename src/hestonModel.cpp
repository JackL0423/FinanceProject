#include "../include/hestonModel.h"

hestonModel::hestonModel()
{
    // something
}

hestonModel::hestonModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility, double v0, double kappa, double theta, double sigma, double rho, OptionType optionType)
{
    // something
}

double hestonModel::calculateOptionPrice() const
{
    return 0.0;
}

void hestonModel::setV0(const double& value) const
{
    _v0 = value;
}

void hestonModel::setKappa(const double& value) const
{
    _kappa = value;
}

void hestonModel::setTheta(const double& value) const
{
    _theta = value;
}

void hestonModel::setSigma(const double& value) const
{
    _sigma = value;
}

void hestonModel::setRho(const double& value) const
{
    _rho = value;
}

const double& hestonModel::getV0() const
{
    return _v0;
}

const double& hestonModel::getKappa() const
{
    return _kappa;
}

const double& hestonModel::getTheta() const
{
    return _theta;
}

const double& hestonModel::getSigma() const
{
    return _sigma;
}

const double& hestonModel::getRho() const
{
    return _rho;
}

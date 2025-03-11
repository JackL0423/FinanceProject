#include "../include/hestonModel.h"
#include "../include/blackScholesModel.h"
#include <cmath>
#include <complex>

hestonModel::hestonModel()
    : optionGreeksModel(), _v0(0.0), _kappa(0.0), _theta(0.0), _sigma(0.0), _rho(0.0)
{
}

hestonModel::hestonModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility, double v0, double kappa, double theta, double sigma, double rho, OptionType optionType)
    : optionGreeksModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility), _v0(v0), _kappa(kappa), _theta(theta), _sigma(sigma), _rho(rho)
{
}

double hestonModel::calculateOptionPrice() const
{
    // Implement the Heston model option pricing formula
    // This is a simplified version and may need further refinement for production use

    // Define constants and parameters
    const double pi = 3.14159265358979323846;
    const std::complex<double> i(0, 1);

    // Define the characteristic function
    const auto alpha = [&](std::complex<double> u) -> std::complex<double> {
        return -u * u * 0.5 - i * u * getKappa() * getTheta();
    };

    const auto beta = [&](std::complex<double> u) -> std::complex<double> {
        return getKappa() - getRho() * getSigma() * i * u;
    };

    const auto gamma = [&](std::complex<double> u) -> std::complex<double> {
        return getSigma() * getSigma() * 0.5;
    };

    const auto D = [&](std::complex<double> u) -> std::complex<double> {
        return std::sqrt(beta(u) * beta(u) - 4.0 * alpha(u) * gamma(u));
    };

    const auto G = [&](std::complex<double> u) -> std::complex<double> {
        return (beta(u) - D(u)) / (beta(u) + D(u));
    };

    const auto C = [&](std::complex<double> u) -> std::complex<double> {
        return getKappa() * (getTheta() * getTimeToExperation() * (beta(u) - D(u)) - 2.0 * std::log((1.0 - G(u) * std::exp(-D(u) * getTimeToExperation())) / (1.0 - G(u))));
    };

    const auto characteristicFunction = [&](std::complex<double> u) -> std::complex<double> {
        return std::exp(C(u) + getV0() * (beta(u) - D(u)) * (1.0 - std::exp(-D(u) * getTimeToExperation())) / (1.0 - G(u) * std::exp(-D(u) * getTimeToExperation())));
    };

    // Integrate using the trapezoidal rule
    const auto integrand = [&](double phi) -> double {
        std::complex<double> u(phi, -0.5);
        std::complex<double> numerator = std::exp(i * u * std::log(getUnderlyingPrice() / getStrikePrice())) * characteristicFunction(u);
        std::complex<double> denominator = i * u;
        return std::real(numerator / denominator);
    };

    double integral = 0.0;
    const int N = 1000; // Number of integration steps
    const double upperLimit = 100.0; // Upper limit of integration
    const double delta = upperLimit / N;

    for (int j = 1; j < N; ++j) {
        double phi = j * delta;
        integral += integrand(phi);
    }

    integral *= delta;

    // Calculate the option price
    double optionPrice = getUnderlyingPrice() * 0.5 - getStrikePrice() * std::exp(-getRiskFreeRate() * getTimeToExperation()) * integral / pi;
    return optionPrice;
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

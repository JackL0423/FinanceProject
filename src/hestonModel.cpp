#include "../include/ErrorHandler.h"
#include "../include/hestonModel.h"
#include "../include/blackScholesModel.h"
#include <random>
#include <cmath>
#include <complex>
#include <fstream>

hestonModel::hestonModel() : blackScholesModel()
{
    setV0(nan(""));
    setKappa(nan(""));
    setTheta(nan(""));
    setSigma(nan(""));
    setRho(nan(""));
}
// TODO: Calculate volatility for the model
hestonModel::hestonModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility, double v0, double kappa, double theta, double sigma, double rho, OptionType optionType)
    : blackScholesModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility, optionType), _v0(v0), _kappa(kappa), _theta(theta), _sigma(sigma), _rho(rho)
{
    setVolatility(volatility);
}

double hestonModel::calculateOptionPrice(bool useMonteCarlo, int num_simulations, int num_time_steps) const
{
    try
    {
        if (isnan(getUnderlyingPrice()) || isnan(getStrikePrice()) || isnan(getTimeToExperation()) || isnan(getRiskFreeRate()) || isnan(getVolatility()) || isnan(getV0()) || isnan(getKappa()) || isnan(getTheta()) || isnan(getSigma()) || isnan(getRho()))
        {
            throw std::invalid_argument("Invalid input: One or more input parameters are NaN");
        }
        if (getVolatility() <= 0.0 || getV0() < 0.0 || getKappa() < 0.0 || getTheta() < 0.0 || getSigma() < 0.0)
        {
            throw std::invalid_argument("Invalid input: Parameters must be non-negative");
        }

        if (useMonteCarlo)
        {
            // Monte Carlo method
            double dt = getTimeToExperation() / num_time_steps;
            double optionPriceSum = 0.0;

            std::random_device rd;
            std::mt19937 generator(rd()); // Use mt19937 for better randomness

            #pragma omp parallel for reduction(+:optionPriceSum)
            for (int sim = 0; sim < num_simulations; sim++) {
                double Vt = simulateVariance(generator, num_time_steps);
                double simulatedVolatility = sqrt(Vt);

                double d1 = (log(getUnderlyingPrice() / getStrikePrice()) + (getRiskFreeRate() + 0.5 * simulatedVolatility * simulatedVolatility) * getTimeToExperation()) / (simulatedVolatility * sqrt(getTimeToExperation()));
                double d2 = d1 - simulatedVolatility * sqrt(getTimeToExperation());

                switch (getOptionType()) {
                    case OptionType::CALL:
                        optionPriceSum += getUnderlyingPrice() * normalCDF(d1) - getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(d2);
                        //cout << "Sim num: " << sim << ", Call option price: " << optionPriceSum << endl;
                        break;
                    case OptionType::PUT:
                        optionPriceSum += getStrikePrice() * exp(-getRiskFreeRate() * getTimeToExperation()) * normalCDF(-d2) - getUnderlyingPrice() * normalCDF(-d1);
                        break;
                    default:
                        throw std::invalid_argument("Invalid option type");
                }
            }

            return optionPriceSum / num_simulations;
        }
        else
        {
            //TODO: Incorporate variance / volatility into the characteristic function.
            // Analytic method using characteristic function
            const double pi = 3.14159265358979323846;
            const std::complex<double> i(0, 1);

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

            const auto integrand = [&](double phi) -> double {
                std::complex<double> u(phi, -0.5);
                std::complex<double> numerator = std::exp(i * u * std::log(getUnderlyingPrice() / getStrikePrice())) * characteristicFunction(u);
                std::complex<double> denominator = i * u;
                return std::real(numerator / denominator);
            };

            const auto adaptiveIntegrate = [&](double lower, double upper, int maxSteps) -> double {
                double result = 0.0;
                double step = (upper - lower) / maxSteps;
                for (int j = 0; j < maxSteps; ++j) {
                    double phi1 = lower + j * step;
                    double phi2 = phi1 + step;
                    result += (integrand(phi1) + integrand(phi2)) * step / 2.0; // Trapezoidal rule
                }
                //cout << "Adaptive integration result: " << result << endl;
                return result;
            };

            double integral = adaptiveIntegrate(0.0, 100.0, 1000);
            //double optionPrice = getUnderlyingPrice() * 0.5 - getStrikePrice() * std::exp(-getRiskFreeRate() * getTimeToExperation()) * integral / pi;
            double optionPrice = getUnderlyingPrice() - getStrikePrice() * std::exp(-getRiskFreeRate() * getTimeToExperation()) * integral / pi;
            return optionPrice;
        }
    }
    catch (const std::exception &e)
    {
        ErrorHandler::logError("Error in calculateOptionPrice: " + std::string(e.what()));
        return nan("");
        //throw;
    }
}

/// @brief 
/// @param generator 
/// @param num_time_steps 
/// @return Vt
double hestonModel::simulateVariance(std::mt19937& generator, int num_time_steps) const 
{
    try
    {
        double Vt = getV0();
        double dt = getTimeToExperation() / num_time_steps;

        std::normal_distribution<double> normalDist(0.0, 1.0);

        for (int i = 0; i < num_time_steps; i++) {
            double Z1 = normalDist(generator);
            double Z2 = getRho() * Z1 + sqrt(1.0 - getRho() * getRho()) * normalDist(generator);

            Vt += getKappa() * (getTheta() - std::max(0.0, Vt)) * dt + getSigma() * sqrt(std::max(0.0, Vt) * dt) * Z2;
            Vt = std::max(0.0, Vt); // Ensure non-negativity
        }

        return Vt;
    }
    catch (const std::exception &e)
    {
        ErrorHandler::logError("Error in simulateVariance: " + std::string(e.what()));
        throw;
    }
}

double hestonModel::random_normal(std::mt19937& generator) const
{
    // Use thread-local storage for the random engine and distribution
    static std::normal_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
} // random_normal()

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

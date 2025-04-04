#ifndef HESTONMODEL_H
#define HESTONMODEL_H

#include "blackScholesModel.h"
#include "optionGreeksModel.h"

#include <random>

class hestonModel : public blackScholesModel
{
    public: 
        hestonModel();

        hestonModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility, double v0, double kappa, double theta, double sigma, double rho, OptionType optionType);

        double calculateOptionPrice() const;

        double newCalculateOptionPrice() const;

        double random_normal(std::default_random_engine& generator) const;

        double simulateVariance(std::default_random_engine& generator, int num_time_steps) const;

        void setV0(const double& value) const;

        void setKappa(const double& value) const;

        void setTheta(const double& value) const;

        void setSigma(const double& value) const;

        void setRho(const double& value) const;

        const double& getV0() const;

        const double& getKappa() const;

        const double& getTheta() const;

        const double& getSigma() const;

        const double& getRho() const;

    
        private:
            mutable double _v0;     // initial volatility
            mutable double _kappa;  // rate at which v_t reverts to theta
            mutable double _theta;  // long variance / long-run average variance of the price
            mutable double _sigma;  // volatility of volatility
            mutable double _rho;    // correlation of the two wiener processes

};

#endif//HESTONMODEL_H
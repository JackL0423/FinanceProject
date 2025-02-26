#ifndef HESTONMODEL_H
#define HESTONMODEL_H

#include "blackScholesModel.h"
#include "optionGreeksModel.h"

class hestonModel : public optionGreeksModel
{
    public: 
        hestonModel();

        hestonModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility, double v0, double kappa, double theta, double sigma, double rho, OptionType optionType);

        double calculateOptionPrice() const;

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
            mutable double _v0;
            mutable double _kappa;
            mutable double _theta;
            mutable double _sigma;
            mutable double _rho;

};

#endif//HESTONMODEL_H
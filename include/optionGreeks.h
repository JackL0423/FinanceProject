#ifndef OPTIONGREEKS_H
#define OPTIONGREEKS_H

#include <cmath>

#include "blackScholesModel.h"

class optionGreeks : public virtual blackScholesModel
{
    public:
        optionGreeks();

        optionGreeks(double underlyingPrice, double strikePrice, double timeToExperation,
                     double riskFreeRate, double volatility);

        double calculateDelta() const;

        double calculateGamma() const;

        double calculateVega() const;

        double calculateTheta() const;

        double calculateRho() const;


        void setDelta(const double& val) { _delta = val; }

        void setGamma(const double& val) const { _gamma = val; }

        void setVega(const double& val) const { _vega = val; }

        void setTheta(const double& val) const { _theta = val; }

        void setRho(const double& val) const { _rho = val; }


        const double& getDelta() const { return _delta; }

        const double& getGamma() const { return _gamma; }

        const double& getVega() const { return _vega; }

        const double& getTheta() const { return _theta; }

        const double& getRho() const { return _rho; }

    protected:
        mutable double _delta;
        mutable double _gamma;
        mutable double _vega;
        mutable double _theta;
        mutable double _rho;

    private:
};


#endif//OPTIONGREEKS_H
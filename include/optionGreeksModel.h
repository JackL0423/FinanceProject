#ifndef OPTIONGREEKSMODEL_H
#define OPTIONGREEKSMODEL_H

#include "blackScholesModel.h"
#include "optionGreeks.h"

class optionGreeksModel : public virtual blackScholesModel, public optionGreeks
{
    public:
        optionGreeksModel();

        optionGreeksModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility);

        // TODO: #8 Fix set functions to intake const double& value
        void setOptionPriceIV(double value) const;

        void setOptionPriceGamma(double value) const;

        void setOptionPriceVega(double value) const;

        void setOptionPriceTheta(double value) const;

        void setOptionPriceGammaVega(double value) const;

        void setIVAdjustedDelta(double value) const;

        void setGammaAdjustedDelta(double value) const;

        void setVegaAdjustedDelta(double value) const;

        void setThetaAdjustedDelta(double value) const;

        void setGammaVegaAdjustedDelta(double value) const;

        const double& getOptionPriceIV() const;

        const double& getOptionPriceGamma() const;

        const double& getOptionPriceVega() const;

        const double& getOptionPriceTheta() const;

        const double& getOptionPriceGammaVega() const;

        const double& getIVAdjustedDelta() const;

        const double& getGammaAdjustedDelta() const;

        const double& getVegaAdjustedDelta() const;

        const double& getThetaAdjustedDelta() const;

        const double& getGammaVegaAdjustedDelta() const;

        
        void calculateOptionPriceIV(double impliedVolatility) const;

        void calculateOptionPriceGamma() const;

        void calculateOptionPriceVega() const;

        void calculateOptionPriceTheta() const;

        void calculateOptionPriceGammaVega() const;

        void calculateIVAdjustedDelta(double impliedVolatility) const;

        void calculateGammaAdjustedDelta() const;

        void calculateVegaAdjustedDelta() const;

        void calculateThetaAdjustedDelta() const;

        void calculateGammaVegaAdjustedDelta() const;

    private:
        mutable double _ivAdjustedDelta;
        mutable double _gammaAdjustedDelta;
        mutable double _vegaAdjustedDelta;
        mutable double _thetaAdjustedDelta;
        mutable double _gammaVegaAdjustedDelta;
        
        mutable double _optionPriceIv;
        mutable double _optionPriceGamma;
        mutable double _optionPriceVega;
        mutable double _optionPriceTheta;
        mutable double _optionPriceGammaVega;

};


#endif//OPTIONGREEKSMODEL_H
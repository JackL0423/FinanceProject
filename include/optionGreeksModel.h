#ifndef OPTIONGREEKSMODEL_H
#define OPTIONGREEKSMODEL_H

#include "blackScholesModel.h"
#include "optionGreeks.h"

class optionGreeksModel : public virtual blackScholesModel, public optionGreeks
{
    optionGreeksModel();

    optionGreeksModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility);


    void setOptionPriveIV(double value) const;

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
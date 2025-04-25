#ifndef OPTIONGREEKSMODEL_H
#define OPTIONGREEKSMODEL_H

#include "blackScholesModel.h"
#include "optionGreeks.h"

/**
 * @class optionGreeksModel
 * @brief A model that extends Black-Scholes and option Greeks calculations.
 * 
 * This class provides methods to calculate various option price adjustments
 * and deltas based on implied volatility, gamma, vega, and theta.
 */
class optionGreeksModel : public virtual blackScholesModel, public optionGreeks
{
    public:
        /**
         * @brief Default constructor for optionGreeksModel.
         */
        optionGreeksModel();

        /**
         * @brief Parameterized constructor for optionGreeksModel.
         * 
         * @param underlyingPrice The price of the underlying asset.
         * @param strikePrice The strike price of the option.
         * @param timeToExperation Time to expiration in years.
         * @param riskFreeRate The risk-free interest rate.
         * @param volatility The volatility of the underlying asset.
         */
        optionGreeksModel(double underlyingPrice, double strikePrice, double timeToExperation, double riskFreeRate, double volatility);

        /**
         * @brief Sets the option price based on implied volatility.
         * @param value The calculated option price IV.
         */
        void setOptionPriceIV(const double& value) const;

        /**
         * @brief Sets the option price gamma.
         * @param value The calculated option price gamma.
         */
        void setOptionPriceGamma(const double& value) const;

        /**
         * @brief Sets the option price vega.
         * @param value The calculated option price vega.
         */
        void setOptionPriceVega(const double& value) const;

        /**
         * @brief Sets the option price theta.
         * @param value The calculated option price theta.
         */
        void setOptionPriceTheta(const double& value) const;

        /**
         * @brief Sets the option price gamma-vega.
         * @param value The calculated option price gamma-vega.
         */
        void setOptionPriceGammaVega(const double& value) const;

        /**
         * @brief Sets the IV adjusted delta.
         * @param value The calculated IV adjusted delta.
         */
        void setIVAdjustedDelta(const double& value) const;

        /**
         * @brief Sets the gamma adjusted delta.
         * @param value The calculated gamma adjusted delta.
         */
        void setGammaAdjustedDelta(const double& value) const;

        /**
         * @brief Sets the vega adjusted delta.
         * @param value The calculated vega adjusted delta.
         */
        void setVegaAdjustedDelta(const double& value) const;

        /**
         * @brief Sets the theta adjusted delta.
         * @param value The calculated theta adjusted delta.
         */
        void setThetaAdjustedDelta(const double& value) const;

        /**
         * @brief Sets the gamma-vega adjusted delta.
         * @param value The calculated gamma-vega adjusted delta.
         */
        void setGammaVegaAdjustedDelta(const double& value) const;

        const double& getOptionPriceIV() const;

        const double& getOptionPriceGamma() const;

        const double& getOptionPriceVega() const;

        const double& getOptionPriceTheta() const;

        const double& getOptionPriceGammaVega() const;

        const double& getIVAdjustedDelta() const;

        /**
         * @brief Gets the gamma adjusted delta.
         * @return The gamma adjusted delta.
         */
        const double& getGammaAdjustedDelta() const;

        /**
         * @brief Gets the vega adjusted delta.
         * @return The vega adjusted delta.
         */
        const double& getVegaAdjustedDelta() const;

        /**
         * @brief Gets the theta adjusted delta.
         * @return The theta adjusted delta.
         */
        const double& getThetaAdjustedDelta() const;

        /**
         * @brief Gets the gamma-vega adjusted delta.
         * @return The gamma-vega adjusted delta.
         */
        const double& getGammaVegaAdjustedDelta() const;


        /**
         * @brief Calculates the implied volatility.
         * @return The calculated implied volatility.
         */
        double calculateImpliedVolatility() const;

        
        /**
         * @brief Calculates the option price based on implied volatility.
         */
        void calculateOptionPriceIV() const;

        /**
         * @brief Calculates the option price gamma.
         */
        void calculateOptionPriceGamma() const;

        /**
         * @brief Calculates the option price vega.
         */
        void calculateOptionPriceVega() const;

        /**
         * @brief Calculates the option price theta.
         */
        void calculateOptionPriceTheta() const;

        /**
         * @brief Calculates the option price gamma-vega.
         */
        void calculateOptionPriceGammaVega() const;

        /**
         * @brief Calculates the IV adjusted delta.
         */
        void calculateIVAdjustedDelta() const;

        /**
         * @brief Calculates the gamma adjusted delta.
         */
        void calculateGammaAdjustedDelta() const;

        /**
         * @brief Calculates the vega adjusted delta.
         */
        void calculateVegaAdjustedDelta() const;

        /**
         * @brief Calculates the theta adjusted delta.
         */
        void calculateThetaAdjustedDelta() const;

        /**
         * @brief Calculates the gamma-vega adjusted delta.
         */
        void calculateGammaVegaAdjustedDelta() const;

        /**
         * @brief Calculates d1 used in various option pricing models.
         * @param impliedVolatility The implied volatility to be used in the calculation.
         */
        void calculateD1(double impliedVolatility) const;

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

#endif // OPTIONGREEKSMODEL_H
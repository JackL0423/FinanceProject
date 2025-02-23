#ifndef OPTIONGREEKS_H
#define OPTIONGREEKS_H

#include <cmath>

#include "blackScholesModel.h"

/**
 * @class optionGreeks
 * @brief A class to calculate and store the Greeks for options pricing using the Black-Scholes model.
 *
 * The optionGreeks class provides methods to calculate the five main Greeks (Delta, Gamma, Vega, Theta, and Rho)
 * which are used to measure the sensitivity of the option's price to various factors.
 * 
 * @note This class inherits from the blackScholesModel class.
 */

/**
 * @brief Default constructor for optionGreeks.
 */
 
/**
 * @brief Parameterized constructor for optionGreeks.
 * 
 * @param underlyingPrice The current price of the underlying asset.
 * @param strikePrice The strike price of the option.
 * @param timeToExperation The time to expiration of the option in years.
 * @param riskFreeRate The risk-free interest rate.
 * @param volatility The volatility of the underlying asset.
 */

/**
 * @brief Calculate the Delta of the option.
 */
 
/**
 * @brief Calculate the Gamma of the option.
 */
 
/**
 * @brief Calculate the Vega of the option.
 */
 
/**
 * @brief Calculate the Theta of the option.
 */
 
/**
 * @brief Calculate the Rho of the option.
 */

/**
 * @brief Set the Delta value.
 * 
 * @param val The Delta value to set.
 */

/**
 * @brief Set the Gamma value.
 * 
 * @param val The Gamma value to set.
 */

/**
 * @brief Set the Vega value.
 * 
 * @param val The Vega value to set.
 */

/**
 * @brief Set the Theta value.
 * 
 * @param val The Theta value to set.
 */

/**
 * @brief Set the Rho value.
 * 
 * @param val The Rho value to set.
 */

/**
 * @brief Get the Delta value.
 * 
 * @return The Delta value.
 */

/**
 * @brief Get the Gamma value.
 * 
 * @return The Gamma value.
 */

/**
 * @brief Get the Vega value.
 * 
 * @return The Vega value.
 */

/**
 * @brief Get the Theta value.
 * 
 * @return The Theta value.
 */

/**
 * @brief Get the Rho value.
 * 
 * @return The Rho value.
 */
class optionGreeks : public virtual blackScholesModel
{
    public:
        optionGreeks();

        optionGreeks(double underlyingPrice, double strikePrice, double timeToExperation,
                     double riskFreeRate, double volatility);

        void calculateDelta();

        void calculateGamma();

        void calculateVega();

        void calculateTheta();

        void calculateRho();


        void setDelta(const double& val) const { _delta = val; }

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
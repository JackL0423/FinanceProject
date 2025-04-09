#ifndef BLACKSCHOLESMODEL_H
#define BLACKSCHOLESMODEL_H

#include<iostream>
#include "optionType.h"

using namespace std;

/**
 * @class blackScholesModel
 * @brief A class to model the Black-Scholes option pricing model.
 *
 * This class provides methods to calculate the price of European call and put options
 * using the Black-Scholes formula. It also provides methods to set and get various
 * parameters required for the calculation.
 *
 * @note This class is designed to be used with European options only.
 *
 * @enum OptionType
 * @brief Enum to represent the type of option.
 * @var OptionType::CALL
 * Represents a call option.
 * @var OptionType::PUT
 * Represents a put option.
 *
 * @friend class inputReader
 * @friend class optionGreeks
 * @friend class optionGreeksModel
 * @friend class hestonModel
 *
 * @onstructor blackScholesModel()
 * Default constructor.
 *
 * @constructor blackScholesModel(double underlyingPrice, double strikePrice, double timeToExpiration,
 *                                double riskFreeRate, double volatility)
 * Parameterized constructor to initialize the model with given parameters.
 *
 * @constructor blackScholesModel(double underlyingPrice, double strikePrice, double timeToExperation,
 *                                double riskFreeRate, double volatility, OptionType optionType)
 * Parameterized constructor to initialize the model with given parameters and option type.
 *
 * @fn double calculateOptionPrice()
 * @brief Calculates the price of the option using the Black-Scholes formula.
 * @return The calculated option price.
 *
 * @fn double normalCDF(double d) const
 * @brief Calculates the cumulative distribution function of the standard normal distribution.
 * @param d The value to calculate the CDF for.
 * @return The CDF value.
 *
 * @fn void setUnderlyingPrice(const double& value)
 * @brief Sets the underlying price of the asset.
 * @param value The underlying price.
 *
 * @fn void setStrikePrice(const double& value)
 * @brief Sets the strike price of the option.
 * @param value The strike price.
 *
 * @fn void setTimeToExperation(const double& value)
 * @brief Sets the time to expiration of the option.
 * @param value The time to expiration.
 *
 * @fn void setRiskFreeRate(const double& value)
 * @brief Sets the risk-free interest rate.
 * @param value The risk-free rate.
 *
 * @fn void setVolatility(const double& value)
 * @brief Sets the volatility of the underlying asset.
 * @param value The volatility.
 *
 * @fn void setOptionType(const OptionType& option)
 * @brief Sets the type of the option (CALL or PUT).
 * @param option The option type.
 *
 * @fn void setD1(const double& value) const
 * @brief Sets the value of d1.
 * @param value The value of d1.
 *
 * @fn void setD2(const double& value) const
 * @brief Sets the value of d2.
 * @param value The value of d2.
 *
 * @fn void setK(const double& value) const
 * @brief Sets the value of K.
 * @param value The value of K.
 *
 * @fn const double& getUnderlyingPrice() const
 * @brief Gets the underlying price of the asset.
 * @return The underlying price.
 *
 * @fn const double& getStrikePrice() const
 * @brief Gets the strike price of the option.
 * @return The strike price.
 *
 * @fn const double& getTimeToExperation() const
 * @brief Gets the time to expiration of the option.
 * @return The time to expiration.
 *
 * @fn const double& getRiskFreeRate() const
 * @brief Gets the risk-free interest rate.
 * @return The risk-free rate.
 *
 * @fn const double& getVolatility() const
 * @brief Gets the volatility of the underlying asset.
 * @return The volatility.
 *
 * @fn const OptionType getOptionType() const
 * @brief Gets the type of the option (CALL or PUT).
 * @return The option type.
 *
 * @fn const double& getD1() const
 * @brief Gets the value of d1.
 * @return The value of d1.
 *
 * @fn const double& getD2() const
 * @brief Gets the value of d2.
 * @return The value of d2.
 *
 * @fn const double& getK() const
 * @brief Gets the value of K.
 * @return The value of K.
 */
class blackScholesModel
{
    mutable double _d1;
    mutable double _d2;
    mutable double _d;
    mutable double _K;


    void calculateD1();

    void calculateD2();

    void calculateK();

    friend class inputReader;
    friend class optionGreeks;
    friend class optionGreeksModel;
    friend class hestonModel;

    public:
        blackScholesModel();

        blackScholesModel(double underlyingPrice, double strikePrice, double timeToExpiration,
                      double riskFreeRate, double volatility);
    
        blackScholesModel(double underlyingPrice, double strikePrice, double timeToExperation,
                      double riskFreeRate, double volatility, OptionType optionType);


        double calculateOptionPrice();

        const double normalCDF(const double& d) const;


        void setUnderlyingPrice(const double& value);

        void setStrikePrice(const double& value);

        void setTimeToExperation(const double& value);

        void setRiskFreeRate(const double& value);

        void setVolatility(const double& value);

        void setOptionType(const OptionType& option);

        void setD1(const double& value) const;

        void setD2(const double& value) const;

        void setK(const double& value) const;


        const double& getUnderlyingPrice() const;

        const double& getStrikePrice() const;

        const double& getTimeToExperation() const;

        const double& getRiskFreeRate() const;

        const double& getVolatility() const;

        const OptionType getOptionType() const;

        const double& getD1() const;

        const double& getD2() const;

        const double& getK() const;

    private:
        double _underlyingPrice;
        double _strikePrice;
        double _timeToExperation;
        double _riskFreeRate;
        double _volatility;      //  One varaible that cannot be predicted, future market risk
        OptionType _optionType;
        
};  //  BlackScholesModel Class 

#endif  //  BLACKSCHOLESMODEL_H 
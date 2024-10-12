#ifndef BLACKSCHOLESMODEL_H
#define BLACKSCHOLESMODEL_H

#include<iostream>

using namespace std;

class blackScholesModel
{
    double underlyingPrice;
    double strikePrice;
    double timeToExperation;
    double riskFreeRate;
    double volatility;      //  One varaible that cannot be predicted, future market risk


    mutable double _d1;
    mutable double _d2;
    mutable double _d;
    mutable double _K;


    double calculateD1(double underlyingPrice, double strikePrice, double timeToExpiration, double riskFreeRate, double volatility) const;

    double calculateD2(double underlyingPrice, double strikePrice, double timeToExpiration, double riskFreeRate, double volatility) const;

    double calculateK(double d) const;

    public:
        enum OptionType {
            CALL,
            PUT
        } optionType;

    friend class inputReader;
    friend class optionGreek;
    friend class optionGreekModel;
    friend class hestonModel;


    blackScholesModel();

    blackScholesModel(double underlyingPrice, double strikePrice, double timeToExpiration,
                      double riskFreeRate, double volatility);
    
    blackScholesModel(double underlyingPrice, double strikePrice, double riskFreeRate,
                      double timeToExpiration, double volatility, OptionType optionType);


    double calculateOptionPrice();

    double normalCDF(double d) const;

    void setUnderlyingPrice(const double& value);

    void setStrikePrice(const double& value);

    void setTimeToExperation(const double& value);

    void setRiskFreeRate(const double& value);

    void setVolatility(const double& value);

    void setOptionType(const OptionType& option);

    void setD1(const double& value);


};  //  BlackScholesModel Class 

#endif  //  BLACKSCHOLESMODEL_H 
//
//
//  Need to create a PUT system in order to do american systems, follows same proccess as mechanical vibrations ODE's

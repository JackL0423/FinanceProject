#include<iostream>
#include "../include/hestonModel.h"
#include <chrono>

using namespace std;

//double newOptionPriceCalc(double, double, double, double, double, double, double, double, double, double, optionGreeksModel::OptionType);

// aiming for ~6.682601659241039
int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    double underlyingPrice = 50.0;
    double strikePrice = 45.0;
    double timeToExperation = 0.0822;
    double riskFreeRate = 0.05;
    double volatility = 0.2;
    double v0 = 0.005;
    double kappa = 0.8;
    double theta = 0.04;
    double sigma = 0.3;
    double rho = 0.2;
    optionGreeksModel::OptionType optionType = optionGreeksModel::OptionType::CALL;
    hestonModel model(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility, v0, kappa, theta, sigma, rho, optionType);

    cout << "QuantLib option price: 6.682601659241039" << endl;
    cout << "Heston model without monte-carlo: " << model.calculateOptionPrice(false, 10000, 1000) << endl;
    cout << "Heston model with monte-carlo: " << model.calculateOptionPrice(true, 10, 1000) << endl;

    // Test for the black scholes model class
    blackScholesModel bsModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility, optionType);
    cout << "Black-Scholes model option price: " << bsModel.calculateOptionPrice() << endl;

    // Test for the optionGreeksModel class
    optionGreeksModel greeksModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    cout << "Option Greeks model option price: " << greeksModel.getOptionPriceIV() << endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;

    return 0;
}

// TODO: Develop more edge case testing for optionGreeks, optionGreeksModel, and BlackScholesModel

// TODO: In-depth review of base-case parameters for model creation

// TODO: Relook at hestonModel::calculateOptionPrice
//      * lambda functions may be unnecessary, look at using parent functions

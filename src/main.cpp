#include<iostream>
#include "../include/hestonModel.h"
#include <random>

using namespace std;

//double newOptionPriceCalc(double, double, double, double, double, double, double, double, double, double, optionGreeksModel::OptionType);

// aiming for ~6.682601659241039
int main()
{
    hestonModel model(100.0, 100.0, 1.0, 0.05, 0.2, 0.04, 2.0, 0.04, 0.3, -0.7, optionGreeksModel::OptionType::PUT);

    cout << "QuantLib option price: 6.682601659241039" << endl;
    cout << "Current model option price: " << model.calculateOptionPrice() << endl;
    cout << "New model option price: " << model.newCalculateOptionPrice() << endl;

    return 0;
}

// TODO: Develop more edge case testing for optionGreeks, optionGreeksModel, and BlackScholesModel

// TODO: In-depth review of base-case parameters for model creation

// TODO: Relook at hestonModel::calculateOptionPrice
//      * lambda functions may be unnecessary, look at using parent functions

#include<iostream>
#include<fstream>
#include <chrono>
#include <vector>
#include <string>

#include "../include/hestonModel.h"
#include "../include/RMSE.h"
#include "../include/inputReader.h"

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
    cout << "Heston model with monte-carlo: " << model.calculateOptionPrice(true, 10000, 1000) << endl;

    // Test for the black scholes model class
    blackScholesModel bsModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility, optionType);
    cout << "Black-Scholes model option price: " << bsModel.calculateOptionPrice() << endl;

    // Test for the optionGreeksModel class
    optionGreeksModel greeksModel(underlyingPrice, strikePrice, timeToExperation, riskFreeRate, volatility);
    cout << "Option Greeks model option price: " << greeksModel.getOptionPriceIV() << endl;


    // Testing for the inputReader class and RMSE function.
    std::string filename = "/Users/jacklight/FinanceProject/Test_Data.csv";

    std::vector<CSVData> data = CSVDataReader(filename);
    if (data.empty())
    {
        cout << "No data found in the file." << endl;
    }
    else
    {
        cout << "Data from CSV file:" << endl;
        for (const auto& row : data)
        {
            row.print();
        }

        // get data from CSV file and plug into models
        std::vector<double> estimatedPrices;
        for (const auto& row : data)
        {
            double expiration = row.getExpiration();
            double stockPrice = row.getStockPrice();
            double CSVstrikePrice = row.getStrikePrice();
            double callPrice = row.getCallPrice();

            blackScholesModel bModel(stockPrice, CSVstrikePrice, expiration, 6.50e-10, 2.38e-3, blackScholesModel::OptionType::CALL);
            double estimatedPrice = bModel.calculateOptionPrice();
            estimatedPrices.push_back(estimatedPrice);
        }


        // calculate the RMSE
        std::vector<double> actualPrices;
        for (const auto& row : data)
        {
            actualPrices.push_back(row.getCallPrice());
        }
        double rmse = rootMeanSquareError(estimatedPrices, actualPrices);
        cout << "RMSE: " << rmse << endl;        
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;

    return 0;
}

// TODO: Relook at hestonModel::calculateOptionPrice
//      * lambda functions may be unnecessary, look at using parent functions
// TODO: implement lattice walks
// TODO: implement Wierner process for higher R^n dimensionality
/*
sqrt(sum(estimated price - actual price) ^ 2)/n)
*/
#include "../include/RMSE.h"
#include "../include/ErrorHandler.h"

double rootMeanSquareError(const std::vector<double>& estimatedPrices, const std::vector<double>& actualPrices)
{
    try
    {
        if (estimatedPrices.size() != actualPrices.size())
        {
            throw std::invalid_argument("Error: The size of estimatedPrices and actualPrices must be the same.");
        }
        if (estimatedPrices.empty() || actualPrices.empty())
        {
            throw std::invalid_argument("Error: The input vectors must not be empty.");
        }
        double sumSquareErrors = 0.0;
        for (const auto& estimatedPrice : estimatedPrices)
        {
            if (isnan(estimatedPrice))
            {
                throw std::invalid_argument("Error: Estimated price contains NaN values.");
            }
            if (isnan(actualPrices[&estimatedPrice - &estimatedPrices[0]]))
            {
                throw std::invalid_argument("Error: Actual price contains NaN values.");
            }
            sumSquareErrors += pow(estimatedPrice - actualPrices[&estimatedPrice - &estimatedPrices[0]], 2);
        }
        double meanSquareError = sumSquareErrors / estimatedPrices.size();
        if (meanSquareError < 0)
        {
            throw std::invalid_argument("Error: Mean square error cannot be negative.");
        }
        return sqrt(meanSquareError);
    }
    catch(const std::exception& e)
    {
        ErrorHandler::logError("Error in rootMeanSquareError: " + std::string(e.what()));
        return nan("");
    }
    
}
/*
sqrt(sum(estimated price - actual price) ^ 2)/n)
*/
#include "../include/RMSE.h"
#include "../include/ErrorHandler.h"
#include <cmath>

double rootMeanSquareError(const std::vector<double>& observed, const std::vector<double>& predicted)
{
    // Check if the vectors are the same size
    if (observed.size() != predicted.size())
    {
        throw std::invalid_argument("Vectors must be of the same size");
    }

    double sum = 0.0;
    for (size_t i = 0; i < observed.size(); ++i)
    {
        double diff = observed[i] - predicted[i];
        sum += diff * diff;
    }

    return std::sqrt(sum / observed.size());
}


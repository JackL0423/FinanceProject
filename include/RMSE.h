#ifndef RMSE_H
#define RMSE_H

#include <vector>
#include <cmath>

double rootMeanSquareError(const std::vector<double>& estimatedPrices, const std::vector<double>& actualPrices);

#endif//RMSE_H
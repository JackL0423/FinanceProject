#ifndef RMSE_H
#define RMSE_H

#include <vector>
#include <cmath>

double rootMeanSquareError(const std::vector<double>& observed, const std::vector<double>& predicted);

#endif // RMSE_H
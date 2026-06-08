#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../include/optionType.h"
#include "../include/blackScholesModel.h"
#include "../include/optionGreeks.h"
#include "../include/optionGreeksModel.h"
#include "../include/hestonModel.h"

namespace py = pybind11;

PYBIND11_MODULE(pricing_core, m)
{
    m.doc() = "C++ financial pricing core — Black-Scholes, Heston, and option Greeks";

    // ----------------------------------------------------------------
    // OptionType enum
    // ----------------------------------------------------------------
    py::enum_<OptionType>(m, "OptionType")
        .value("CALL", OptionType::CALL)
        .value("PUT",  OptionType::PUT)
        .export_values();

    // ----------------------------------------------------------------
    // BlackScholesModel
    // ----------------------------------------------------------------
    py::class_<blackScholesModel>(m, "BlackScholesModel")
        .def(py::init<>())
        .def(py::init<double, double, double, double, double>(),
             py::arg("underlying_price"),
             py::arg("strike_price"),
             py::arg("time_to_expiration"),
             py::arg("risk_free_rate"),
             py::arg("volatility"))
        .def(py::init<double, double, double, double, double, OptionType>(),
             py::arg("underlying_price"),
             py::arg("strike_price"),
             py::arg("time_to_expiration"),
             py::arg("risk_free_rate"),
             py::arg("volatility"),
             py::arg("option_type"))

        .def("calculate_option_price", &blackScholesModel::calculateOptionPrice)
        .def("normal_cdf",             &blackScholesModel::normalCDF, py::arg("d"))

        .def("set_underlying_price",    &blackScholesModel::setUnderlyingPrice,   py::arg("value"))
        .def("set_strike_price",        &blackScholesModel::setStrikePrice,       py::arg("value"))
        .def("set_time_to_expiration",  &blackScholesModel::setTimeToExperation,  py::arg("value"))
        .def("set_risk_free_rate",      &blackScholesModel::setRiskFreeRate,      py::arg("value"))
        .def("set_volatility",          &blackScholesModel::setVolatility,        py::arg("value"))
        .def("set_option_type",         &blackScholesModel::setOptionType,        py::arg("option_type"))

        .def_property_readonly("underlying_price",   &blackScholesModel::getUnderlyingPrice)
        .def_property_readonly("strike_price",       &blackScholesModel::getStrikePrice)
        .def_property_readonly("time_to_expiration", &blackScholesModel::getTimeToExperation)
        .def_property_readonly("risk_free_rate",     &blackScholesModel::getRiskFreeRate)
        .def_property_readonly("volatility",         &blackScholesModel::getVolatility)
        .def_property_readonly("option_type",        &blackScholesModel::getOptionType)
        .def_property_readonly("d1",                 &blackScholesModel::getD1)
        .def_property_readonly("d2",                 &blackScholesModel::getD2)

        .def("__repr__", [](const blackScholesModel& m) {
            return "<BlackScholesModel S=" + std::to_string(m.getUnderlyingPrice())
                 + " K=" + std::to_string(m.getStrikePrice())
                 + " T=" + std::to_string(m.getTimeToExperation()) + ">";
        });

    // ----------------------------------------------------------------
    // OptionGreeks  (inherits BlackScholesModel)
    // ----------------------------------------------------------------
    py::class_<optionGreeks, blackScholesModel>(m, "OptionGreeks")
        .def(py::init<>())
        .def(py::init<double, double, double, double, double>(),
             py::arg("underlying_price"),
             py::arg("strike_price"),
             py::arg("time_to_expiration"),
             py::arg("risk_free_rate"),
             py::arg("volatility"))

        .def("calculate_delta", &optionGreeks::calculateDelta)
        .def("calculate_gamma", &optionGreeks::calculateGamma)
        .def("calculate_vega",  &optionGreeks::calculateVega)
        .def("calculate_theta", &optionGreeks::calculateTheta)
        .def("calculate_rho",   &optionGreeks::calculateRho)

        .def_property_readonly("delta", &optionGreeks::getDelta)
        .def_property_readonly("gamma", &optionGreeks::getGamma)
        .def_property_readonly("vega",  &optionGreeks::getVega)
        .def_property_readonly("theta", &optionGreeks::getTheta)
        .def_property_readonly("rho",   &optionGreeks::getRho)

        .def("__repr__", [](const optionGreeks& g) {
            return "<OptionGreeks delta=" + std::to_string(g.getDelta())
                 + " gamma=" + std::to_string(g.getGamma())
                 + " vega="  + std::to_string(g.getVega()) + ">";
        });

    // ----------------------------------------------------------------
    // OptionGreeksModel  (inherits OptionGreeks)
    // ----------------------------------------------------------------
    py::class_<optionGreeksModel, optionGreeks>(m, "OptionGreeksModel")
        .def(py::init<>())
        .def(py::init<double, double, double, double, double>(),
             py::arg("underlying_price"),
             py::arg("strike_price"),
             py::arg("time_to_expiration"),
             py::arg("risk_free_rate"),
             py::arg("volatility"))

        .def("calculate_implied_volatility",    &optionGreeksModel::calculateImpliedVolatility)
        .def("calculate_option_price_iv",       &optionGreeksModel::calculateOptionPriceIV)
        .def("calculate_option_price_gamma",    &optionGreeksModel::calculateOptionPriceGamma)
        .def("calculate_option_price_vega",     &optionGreeksModel::calculateOptionPriceVega)
        .def("calculate_option_price_theta",    &optionGreeksModel::calculateOptionPriceTheta)
        .def("calculate_option_price_gammavega",&optionGreeksModel::calculateOptionPriceGammaVega)

        .def_property_readonly("option_price_iv",        &optionGreeksModel::getOptionPriceIV)
        .def_property_readonly("option_price_gamma",     &optionGreeksModel::getOptionPriceGamma)
        .def_property_readonly("option_price_vega",      &optionGreeksModel::getOptionPriceVega)
        .def_property_readonly("option_price_theta",     &optionGreeksModel::getOptionPriceTheta)
        .def_property_readonly("option_price_gammavega", &optionGreeksModel::getOptionPriceGammaVega)
        .def_property_readonly("iv_adjusted_delta",      &optionGreeksModel::getIVAdjustedDelta)
        .def_property_readonly("gamma_adjusted_delta",   &optionGreeksModel::getGammaAdjustedDelta)
        .def_property_readonly("vega_adjusted_delta",    &optionGreeksModel::getVegaAdjustedDelta)
        .def_property_readonly("theta_adjusted_delta",   &optionGreeksModel::getThetaAdjustedDelta)
        .def_property_readonly("gammavega_adjusted_delta",&optionGreeksModel::getGammaVegaAdjustedDelta);

    // ----------------------------------------------------------------
    // HestonModel  (inherits BlackScholesModel)
    // ----------------------------------------------------------------
    py::class_<hestonModel, blackScholesModel>(m, "HestonModel")
        .def(py::init<>())
        .def(py::init<double, double, double, double, double,
                      double, double, double, double, double, OptionType>(),
             py::arg("underlying_price"),
             py::arg("strike_price"),
             py::arg("time_to_expiration"),
             py::arg("risk_free_rate"),
             py::arg("volatility"),
             py::arg("v0"),
             py::arg("kappa"),
             py::arg("theta"),
             py::arg("sigma"),
             py::arg("rho"),
             py::arg("option_type"))

        .def("calculate_option_price",
             &hestonModel::calculateOptionPrice,
             py::arg("use_monte_carlo") = true,
             py::arg("num_simulations") = 10000,
             py::arg("num_time_steps")  = 100)

        .def("set_v0",    &hestonModel::setV0,    py::arg("value"))
        .def("set_kappa", &hestonModel::setKappa, py::arg("value"))
        .def("set_theta", &hestonModel::setTheta, py::arg("value"))
        .def("set_sigma", &hestonModel::setSigma, py::arg("value"))
        .def("set_rho",   &hestonModel::setRho,   py::arg("value"))

        .def_property_readonly("v0",    &hestonModel::getV0)
        .def_property_readonly("kappa", &hestonModel::getKappa)
        .def_property_readonly("theta", &hestonModel::getTheta)
        .def_property_readonly("sigma", &hestonModel::getSigma)
        .def_property_readonly("rho",   &hestonModel::getRho)

        .def("__repr__", [](const hestonModel& h) {
            return "<HestonModel S=" + std::to_string(h.getUnderlyingPrice())
                 + " K=" + std::to_string(h.getStrikePrice())
                 + " v0=" + std::to_string(h.getV0()) + ">";
        });
}

#include "gtest/gtest.h"
#include "../include/optionGreeksModel.h"
#include <cmath>

class optionGreeksModelTest : public testing::Test
{
    protected:
        optionGreeksModelTest()
        {
        }
        optionGreeksModel model;
};

TEST_F(optionGreeksModelTest, DefaultConstructor)
{
    EXPECT_TRUE(isnan(model.getUnderlyingPrice()));
    EXPECT_TRUE(isnan(model.getStrikePrice()));
    EXPECT_TRUE(isnan(model.getTimeToExperation()));
    EXPECT_TRUE(isnan(model.getRiskFreeRate()));
    EXPECT_TRUE(isnan(model.getVolatility()));
}

TEST_F(optionGreeksModelTest, ParameterizedConstructor)
{
    optionGreeksModel paramModel(100.0, 100.0, 1.0, 0.05, 0.2);
    EXPECT_DOUBLE_EQ(paramModel.getUnderlyingPrice(), 100.0);
    EXPECT_DOUBLE_EQ(paramModel.getStrikePrice(), 100.0);
    EXPECT_DOUBLE_EQ(paramModel.getTimeToExperation(), 1.0);
    EXPECT_DOUBLE_EQ(paramModel.getRiskFreeRate(), 0.05);
    EXPECT_DOUBLE_EQ(paramModel.getVolatility(), 0.2);
}

TEST_F(optionGreeksModelTest, SetOptionPriceIV)
{
    model.setOptionPriceIV(1.23);
    EXPECT_DOUBLE_EQ(model.getOptionPriceIV(), 1.23);
}

TEST_F(optionGreeksModelTest, SetOptionPriceGamma)
{
    model.setOptionPriceGamma(2.34);
    EXPECT_DOUBLE_EQ(model.getOptionPriceGamma(), 2.34);
}

TEST_F(optionGreeksModelTest, SetOptionPriceVega)
{
    model.setOptionPriceVega(3.45);
    EXPECT_DOUBLE_EQ(model.getOptionPriceVega(), 3.45);
}

TEST_F(optionGreeksModelTest, SetOptionPriceTheta)
{
    model.setOptionPriceTheta(4.56);
    EXPECT_DOUBLE_EQ(model.getOptionPriceTheta(), 4.56);
}

TEST_F(optionGreeksModelTest, SetOptionPriceGammaVega)
{
    model.setOptionPriceGammaVega(5.67);
    EXPECT_DOUBLE_EQ(model.getOptionPriceGammaVega(), 5.67);
}

TEST_F(optionGreeksModelTest, SetIVAdjustedDelta)
{
    model.setIVAdjustedDelta(6.78);
    EXPECT_DOUBLE_EQ(model.getIVAdjustedDelta(), 6.78);
}

TEST_F(optionGreeksModelTest, SetGammaAdjustedDelta)
{
    model.setGammaAdjustedDelta(7.89);
    EXPECT_DOUBLE_EQ(model.getGammaAdjustedDelta(), 7.89);
}

TEST_F(optionGreeksModelTest, SetVegaAdjustedDelta)
{
    model.setVegaAdjustedDelta(8.90);
    EXPECT_DOUBLE_EQ(model.getVegaAdjustedDelta(), 8.90);
}

TEST_F(optionGreeksModelTest, SetThetaAdjustedDelta)
{
    model.setThetaAdjustedDelta(9.01);
    EXPECT_DOUBLE_EQ(model.getThetaAdjustedDelta(), 9.01);
}

TEST_F(optionGreeksModelTest, SetGammaVegaAdjustedDelta)
{
    model.setGammaVegaAdjustedDelta(10.12);
    EXPECT_DOUBLE_EQ(model.getGammaVegaAdjustedDelta(), 10.12);
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceGamma)
{
    model.calculateOptionPriceGamma();
    // Add assertions to check if the option price Gamma is correctly calculated
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceVega)
{
    model.calculateOptionPriceVega();
    // Add assertions to check if the option price Vega is correctly calculated
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceTheta)
{
    model.calculateOptionPriceTheta();
    // Add assertions to check if the option price Theta is correctly calculated
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceGammaVega)
{
    model.calculateOptionPriceGammaVega();
    // Add assertions to check if the option price Gamma Vega is correctly calculated
}

TEST_F(optionGreeksModelTest, CalculateGammaAdjustedDelta)
{
    model.calculateGammaAdjustedDelta();
    // Add assertions to check if the Gamma adjusted delta is correctly calculated
}

TEST_F(optionGreeksModelTest, CalculateVegaAdjustedDelta)
{
    model.calculateVegaAdjustedDelta();
    // Add assertions to check if the Vega adjusted delta is correctly calculated
}

TEST_F(optionGreeksModelTest, CalculateThetaAdjustedDelta)
{
    model.calculateThetaAdjustedDelta();
    // Add assertions to check if the Theta adjusted delta is correctly calculated
}

TEST_F(optionGreeksModelTest, CalculateGammaVegaAdjustedDelta)
{
    model.calculateGammaVegaAdjustedDelta();
    // Add assertions to check if the Gamma Vega adjusted delta is correctly calculated
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceIVWithNaN)
{
    model.setOptionPriceIV(NAN);
    EXPECT_TRUE(isnan(model.getOptionPriceIV()));
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceGammaWithNaN)
{
    model.setOptionPriceGamma(NAN);
    model.calculateOptionPriceGamma();
    EXPECT_TRUE(isnan(model.getOptionPriceGamma()));
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceVegaWithNaN)
{
    model.setOptionPriceVega(NAN);
    model.calculateOptionPriceVega();
    EXPECT_TRUE(isnan(model.getOptionPriceVega()));
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceThetaWithNaN)
{
    model.setOptionPriceTheta(NAN);
    model.calculateOptionPriceTheta();
    EXPECT_TRUE(isnan(model.getOptionPriceTheta()));
}

TEST_F(optionGreeksModelTest, CalculateOptionPriceGammaVegaWithNaN)
{
    model.setOptionPriceGammaVega(NAN);
    model.calculateOptionPriceGammaVega();
    EXPECT_TRUE(isnan(model.getOptionPriceGammaVega()));
}

TEST_F(optionGreeksModelTest, CalculateIVAdjustedDeltaWithNaN)
{
    model.setIVAdjustedDelta(NAN);
    EXPECT_TRUE(isnan(model.getIVAdjustedDelta()));
}

TEST_F(optionGreeksModelTest, CalculateGammaAdjustedDeltaWithNaN)
{
    model.setGammaAdjustedDelta(NAN);
    model.calculateGammaAdjustedDelta();
    EXPECT_TRUE(isnan(model.getGammaAdjustedDelta()));
}

TEST_F(optionGreeksModelTest, CalculateVegaAdjustedDeltaWithNaN)
{
    model.setVegaAdjustedDelta(NAN);
    model.calculateVegaAdjustedDelta();
    EXPECT_TRUE(isnan(model.getVegaAdjustedDelta()));
}

TEST_F(optionGreeksModelTest, CalculateThetaAdjustedDeltaWithNaN)
{
    model.setThetaAdjustedDelta(NAN);
    model.calculateThetaAdjustedDelta();
    EXPECT_TRUE(isnan(model.getThetaAdjustedDelta()));
}

TEST_F(optionGreeksModelTest, CalculateGammaVegaAdjustedDeltaWithNaN)
{
    model.setGammaVegaAdjustedDelta(NAN);
    model.calculateGammaVegaAdjustedDelta();
    EXPECT_TRUE(isnan(model.getGammaVegaAdjustedDelta()));
}

TEST_F(optionGreeksModelTest, CalculateD1WithNaN)
{
    model.calculateD1(NAN);
    EXPECT_TRUE(isnan(model.getD1()));
}
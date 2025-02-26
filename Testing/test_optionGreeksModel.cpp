#include "gtest/gtest.h"
#include "../include/optionGreeksModel.h"

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
    EXPECT_DOUBLE_EQ(model.getOptionPriceIV(), 0.0);
    EXPECT_DOUBLE_EQ(model.getOptionPriceGamma(), 0.0);
    EXPECT_DOUBLE_EQ(model.getOptionPriceVega(), 0.0);
    EXPECT_DOUBLE_EQ(model.getOptionPriceTheta(), 0.0);
    EXPECT_DOUBLE_EQ(model.getOptionPriceGammaVega(), 0.0);
    EXPECT_DOUBLE_EQ(model.getIVAdjustedDelta(), 0.0);
    EXPECT_DOUBLE_EQ(model.getGammaAdjustedDelta(), 0.0);
    EXPECT_DOUBLE_EQ(model.getVegaAdjustedDelta(), 0.0);
    EXPECT_DOUBLE_EQ(model.getThetaAdjustedDelta(), 0.0);
    EXPECT_DOUBLE_EQ(model.getGammaVegaAdjustedDelta(), 0.0);
}

TEST_F(optionGreeksModelTest, ParameterizedConstructor)
{
    optionGreeksModel paramModel(100.0, 100.0, 1.0, 0.05, 0.2);
    // Add assertions to check if the parameters are correctly initialized
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

// TODO: #10 [testing] Add tests for the calculate functions in optionGreeksModel
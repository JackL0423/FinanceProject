#include "gtest/gtest.h"
#include "../include/hestonModel.h"

class hestonModelTest : public testing::Test
{
    protected:
        hestonModelTest() : model1(), model2(100.0, 100.0, 1.0, 0.05, 0.2, 0.04, 2.0, 0.04, 0.3, -0.7, optionGreeksModel::OptionType::CALL), model3(model2)
        {
        }

        hestonModel model1;
        hestonModel model2;
        hestonModel model3;
};

TEST_F(hestonModelTest, DefaultConstructor)
{
    EXPECT_DOUBLE_EQ(model1.getUnderlyingPrice(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getStrikePrice(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getTimeToExperation(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getRiskFreeRate(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getVolatility(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getV0(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getKappa(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getTheta(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getSigma(), 0.0);
    EXPECT_DOUBLE_EQ(model1.getRho(), 0.0);
}

TEST_F(hestonModelTest, ParameterizedConstructor)
{
    EXPECT_DOUBLE_EQ(model2.getUnderlyingPrice(), 100.0);
    EXPECT_DOUBLE_EQ(model2.getStrikePrice(), 100.0);
    EXPECT_DOUBLE_EQ(model2.getTimeToExperation(), 1.0);
    EXPECT_DOUBLE_EQ(model2.getRiskFreeRate(), 0.05);
    EXPECT_DOUBLE_EQ(model2.getVolatility(), 0.2);
    EXPECT_DOUBLE_EQ(model2.getV0(), 0.04);
    EXPECT_DOUBLE_EQ(model2.getKappa(), 2.0);
    EXPECT_DOUBLE_EQ(model2.getTheta(), 0.04);
    EXPECT_DOUBLE_EQ(model2.getSigma(), 0.3);
    EXPECT_DOUBLE_EQ(model2.getRho(), -0.7);
}

TEST_F(hestonModelTest, CopyConstructor)
{
    EXPECT_DOUBLE_EQ(model3.getUnderlyingPrice(), 100.0);
    EXPECT_DOUBLE_EQ(model3.getStrikePrice(), 100.0);
    EXPECT_DOUBLE_EQ(model3.getTimeToExperation(), 1.0);
    EXPECT_DOUBLE_EQ(model3.getRiskFreeRate(), 0.05);
    EXPECT_DOUBLE_EQ(model3.getVolatility(), 0.2);
    EXPECT_DOUBLE_EQ(model3.getV0(), 0.04);
    EXPECT_DOUBLE_EQ(model3.getKappa(), 2.0);
    EXPECT_DOUBLE_EQ(model3.getTheta(), 0.04);
    EXPECT_DOUBLE_EQ(model3.getSigma(), 0.3);
    EXPECT_DOUBLE_EQ(model3.getRho(), -0.7);
}

TEST_F(hestonModelTest, SettersAndGetters)
{
    model1.setV0(0.04);
    EXPECT_DOUBLE_EQ(model1.getV0(), 0.04);

    model1.setKappa(2.0);
    EXPECT_DOUBLE_EQ(model1.getKappa(), 2.0);

    model1.setTheta(0.04);
    EXPECT_DOUBLE_EQ(model1.getTheta(), 0.04);

    model1.setSigma(0.3);
    EXPECT_DOUBLE_EQ(model1.getSigma(), 0.3);

    model1.setRho(-0.7);
    EXPECT_DOUBLE_EQ(model1.getRho(), -0.7);
}

TEST_F(hestonModelTest, CalculateOptionPrice)
{
    double optionPrice = model2.calculateOptionPrice();
    EXPECT_NEAR(optionPrice, 10.0, 1.0); // Adjust the expected value and tolerance as needed
}

// Additional tests

TEST_F(hestonModelTest, EdgeCaseZeroVolatility)
{
    hestonModel model(100.0, 100.0, 1.0, 0.05, 0.0, 0.04, 2.0, 0.04, 0.0, -0.7, optionGreeksModel::OptionType::CALL);
    double optionPrice = model.calculateOptionPrice();
    EXPECT_NEAR(optionPrice, 0.0, 1.0); // Adjust the expected value and tolerance as needed
}

TEST_F(hestonModelTest, EdgeCaseHighVolatility)
{
    hestonModel model(100.0, 100.0, 1.0, 0.05, 1.0, 0.04, 2.0, 0.04, 1.0, -0.7, optionGreeksModel::OptionType::CALL);
    double optionPrice = model.calculateOptionPrice();
    EXPECT_NEAR(optionPrice, 50.0, 10.0); // Adjust the expected value and tolerance as needed
}

TEST_F(hestonModelTest, InvalidNegativeVolatility)
{
    hestonModel model(100.0, 100.0, 1.0, 0.05, -0.2, 0.04, 2.0, 0.04, 0.3, -0.7, optionGreeksModel::OptionType::CALL);
    EXPECT_THROW(model.calculateOptionPrice(), std::invalid_argument);
}

TEST_F(hestonModelTest, BoundaryConditionZeroTimeToExpiration)
{
    hestonModel model(100.0, 100.0, 0.0, 0.05, 0.2, 0.04, 2.0, 0.04, 0.3, -0.7, optionGreeksModel::OptionType::CALL);
    double optionPrice = model.calculateOptionPrice();
    EXPECT_NEAR(optionPrice, 0.0, 1.0); // Adjust the expected value and tolerance as needed
}

TEST_F(hestonModelTest, BoundaryConditionLongTimeToExpiration)
{
    hestonModel model(100.0, 100.0, 10.0, 0.05, 0.2, 0.04, 2.0, 0.04, 0.3, -0.7, optionGreeksModel::OptionType::CALL);
    double optionPrice = model.calculateOptionPrice();
    EXPECT_NEAR(optionPrice, 50.0, 10.0); // Adjust the expected value and tolerance as needed
}
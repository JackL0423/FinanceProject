#include "gtest/gtest.h"
#include "../include/blackScholesModel.h"

class blackScholesModelTest : public testing::Test
{
protected:
    blackScholesModelTest() {}
    blackScholesModel a = blackScholesModel(16.2, 13.3, 18.0, 6.2, 4.5, blackScholesModel::OptionType::PUT);
    blackScholesModel b = blackScholesModel(5.6, 4.2, 45.3, 3.14, 2.71);
    blackScholesModel c = blackScholesModel();
};

TEST_F(blackScholesModelTest, ConstructorValidInputs)
{
    EXPECT_EQ(a.getUnderlyingPrice(), 16.2);
    EXPECT_EQ(a.getStrikePrice(), 13.3);
    EXPECT_EQ(a.getTimeToExperation(), 18.0);
    EXPECT_EQ(a.getRiskFreeRate(), 6.2);
    EXPECT_EQ(a.getVolatility(), 4.5);
    EXPECT_EQ(a.getOptionType(), blackScholesModel::OptionType::PUT);
}

TEST_F(blackScholesModelTest, ConstructorDefaultOptionType)
{
    EXPECT_EQ(b.getUnderlyingPrice(), 5.6);
    EXPECT_EQ(b.getStrikePrice(), 4.2);
    EXPECT_EQ(b.getTimeToExperation(), 45.3);
    EXPECT_EQ(b.getRiskFreeRate(), 3.14);
    EXPECT_EQ(b.getVolatility(), 2.71);
    EXPECT_EQ(b.getOptionType(), blackScholesModel::OptionType::CALL);
}

TEST_F(blackScholesModelTest, ConstructorDefaultValues)
{
    EXPECT_TRUE(isnan(c.getUnderlyingPrice()));
    EXPECT_TRUE(isnan(c.getStrikePrice()));
    EXPECT_TRUE(isnan(c.getTimeToExperation()));
    EXPECT_TRUE(isnan(c.getRiskFreeRate()));
    EXPECT_TRUE(isnan(c.getVolatility()));
    EXPECT_EQ(c.getOptionType(), blackScholesModel::OptionType::CALL);
}

TEST_F(blackScholesModelTest, CalculateD1ValidInputs)
{
    double expectedD1 = (log(16.2 / 13.3) + (6.2 + 0.5 * 4.5 * 4.5) * 18.0) / (4.5 * sqrt(18.0));
    EXPECT_NEAR(a.getD1(), expectedD1, 1e-6);
}

TEST_F(blackScholesModelTest, CalculateD1InvalidInputs)
{
    EXPECT_TRUE(isnan(c.getD1()));
}

TEST_F(blackScholesModelTest, CalculateD2ValidInputs)
{
    double expectedD2 = a.getD1() - 4.5 * sqrt(18.0);
    EXPECT_NEAR(a.getD2(), expectedD2, 1e-6);
}

TEST_F(blackScholesModelTest, CalculateD2InvalidInputs)
{
    EXPECT_TRUE(isnan(c.getD2()));
}

TEST_F(blackScholesModelTest, CalculateOptionPriceValidInputs)
{
    double expectedOptionPrice = 13.3 * exp(-6.2 * 18.0) * a.normalCDF(-a.getD2()) - 16.2 * a.normalCDF(-a.getD1());
    EXPECT_NEAR(a.calculateOptionPrice(), expectedOptionPrice, 1e-6);
}

TEST_F(blackScholesModelTest, CalculateOptionPriceInvalidInputs)
{
    EXPECT_TRUE(isnan(c.calculateOptionPrice()));
}

TEST_F(blackScholesModelTest, CalculateKValidInputs)
{
    double expectedK = 1.0 / (1.0 + 0.2316419 * abs(a.getD1()));
    EXPECT_NEAR(a.getK(), expectedK, 1e-6);
}

TEST_F(blackScholesModelTest, CalculateKInvalidInputs)
{
    EXPECT_TRUE(isnan(c.getK()));
}

TEST_F(blackScholesModelTest, NormalCDFLargePositiveValue)
{
    double value = 10.0;
    double expectedNormalCDF = 1.0; // Should be very close to 1
    EXPECT_NEAR(a.normalCDF(value), expectedNormalCDF, 1e-6);
}

TEST_F(blackScholesModelTest, NormalCDFLargeNegativeValue)
{
    double value = -10.0;
    double expectedNormalCDF = 0.0;
    EXPECT_NEAR(b.normalCDF(value), expectedNormalCDF, 1e-6);
}

TEST_F(blackScholesModelTest, NormalCDFPositiveValue)
{
    double value = 1.0;
    double expectedNormalCDF = 0.841344746;
    EXPECT_NEAR(a.normalCDF(value), expectedNormalCDF, 1e-6);
}

TEST_F(blackScholesModelTest, NormalCDFNegativeValue)
{
    double value = -1.0;
    double expectedNormalCDF = 0.158655254;
    EXPECT_NEAR(b.normalCDF(value), expectedNormalCDF, 1e-6);
}

TEST_F(blackScholesModelTest, NormalCDFInvalidInput)
{
    EXPECT_TRUE(isnan(c.normalCDF(nan(""))));
}

TEST_F(blackScholesModelTest, EdgeCaseNegativeVolatility)
{
    blackScholesModel edge = blackScholesModel(16.2, 13.3, 18.0, 6.2, -4.5, blackScholesModel::OptionType::PUT);
    EXPECT_TRUE(isnan(edge.getVolatility()));
}

TEST_F(blackScholesModelTest, EdgeCaseNegativeTimeToExpiration)
{
    blackScholesModel edge = blackScholesModel(16.2, 13.3, -18.0, 6.2, 4.5, blackScholesModel::OptionType::PUT);
    EXPECT_TRUE(isnan(edge.getTimeToExperation()));
}

TEST_F(blackScholesModelTest, EdgeCaseZeroVolatility)
{
    blackScholesModel zeroVolatilityModel(16.2, 13.3, 18.0, 6.2, 0.0, blackScholesModel::OptionType::PUT);
    EXPECT_NO_THROW(zeroVolatilityModel.calculateOptionPrice());
}

TEST_F(blackScholesModelTest, EdgeCaseZeroTimeToExpiration)
{
    blackScholesModel zeroTimeModel(16.2, 13.3, 0.0, 6.2, 4.5, blackScholesModel::OptionType::PUT);
    EXPECT_NO_THROW(zeroTimeModel.calculateOptionPrice());
}

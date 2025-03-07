#include "gtest/gtest.h"
#include "../include/optionGreeks.h"

class OptionGreeksTest : public testing::Test
{
    protected:
        OptionGreeksTest()
        {    
        }
    
    optionGreeks a = optionGreeks(16.2, 13.3, 18.0, 6.2, 4.5);
    optionGreeks b = optionGreeks();
};

TEST_F(OptionGreeksTest, optionGreeksTest_constructor_Test1)
{
    EXPECT_EQ(a.getUnderlyingPrice(), 16.2);
    EXPECT_EQ(a.getStrikePrice(), 13.3);
    EXPECT_EQ(a.getTimeToExperation(), 18.0);
    EXPECT_EQ(a.getRiskFreeRate(), 6.2);
    EXPECT_EQ(a.getVolatility(), 4.5);
}

TEST_F(OptionGreeksTest, optionGreeksTest_constructor_Test2)
{
    EXPECT_EQ(b.getUnderlyingPrice(), 0.0);
    EXPECT_EQ(b.getStrikePrice(), 0.0);
    EXPECT_EQ(b.getTimeToExperation(), 0.0);
    EXPECT_EQ(b.getRiskFreeRate(), 0.0);
    EXPECT_EQ(b.getVolatility(), 0.0);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateDelta_Test1)
{
    double ExpectedDelta = exp(-a.getD1() * a.getD1() / 2) / sqrt(2 * M_PI);
    EXPECT_NEAR(a.getDelta(), ExpectedDelta, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateDelta_Test2)
{
    double ExpectedDelta = exp(-b.getD2() * b.getD2() / 2) / sqrt(2 * M_PI);
    EXPECT_NEAR(b.getDelta(), ExpectedDelta, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateGamma_Test1)
{
    double ExpectedGamma = exp(-a.getD2() * a.getD1() / 2) / (sqrt(2 * M_PI) * a.getUnderlyingPrice() * a.getVolatility() * sqrt(a.getTimeToExperation()));
    EXPECT_NEAR(a.getGamma(), ExpectedGamma, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateGamma_Test2)
{
    double ExpectedGamma = exp(-b.getD2() * b.getD1() / 2) / (sqrt(2 * M_PI) * b.getUnderlyingPrice() * b.getVolatility() * sqrt(b.getTimeToExperation()));
    EXPECT_NEAR(b.getGamma(), ExpectedGamma, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateVega_Test1)
{
    double ExpectedVega = a.getUnderlyingPrice() * exp(-a.getD1() * a.getD1() / 2) * sqrt(a.getTimeToExperation());
    EXPECT_NEAR(a.getVega(), ExpectedVega, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateVega_Test2)
{
    double ExpectedVega = b.getUnderlyingPrice() * exp(-b.getD1() * b.getD1() / 2) * sqrt(b.getTimeToExperation());
    EXPECT_NEAR(b.getVega(), ExpectedVega, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateTheta_Test1)
{
    double ExpectedTheta = -a.getUnderlyingPrice() * exp(-a.getD1() * a.getD1() / 2) * a.getVolatility() / (2* sqrt(a.getTimeToExperation())) - a.getRiskFreeRate() * a.getK() * exp(-a.getRiskFreeRate() * a.getTimeToExperation()) * a.normalCDF(a.getD2());
    EXPECT_NEAR(a.getTheta(), ExpectedTheta, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateTheta_Test2)
{
    double ExpectedTheta = -b.getUnderlyingPrice() * exp(-b.getD1() * b.getD1() / 2) * b.getVolatility() / (2* sqrt(b.getTimeToExperation())) - b.getRiskFreeRate() * b.getK() * exp(-b.getRiskFreeRate() * b.getTimeToExperation()) * b.normalCDF(b.getD2());
    EXPECT_NEAR(b.getTheta(), ExpectedTheta, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateRho_Test1)
{
    double ExpectedRho = a.getK() * a.getTimeToExperation() * exp(-a.getRiskFreeRate() * a.getTimeToExperation()) * a.normalCDF(a.getD2());
    EXPECT_NEAR(a.getRho(), ExpectedRho, 1e-6);
}

TEST_F(OptionGreeksTest, optionGreeksTest_calculateRho_Test2)
{
    double ExpectedRho = b.getK() * b.getTimeToExperation() * exp(-b.getRiskFreeRate() * b.getTimeToExperation()) * b.normalCDF(b.getD2());
    EXPECT_NEAR(b.getRho(), ExpectedRho, 1e-6);
}

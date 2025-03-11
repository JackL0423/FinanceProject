#include "gtest/gtest.h"
#include "../include/blackScholesModel.h"

class blackScholesModelTest : public testing::Test
{
    protected:
        blackScholesModelTest()
        {
        }
    //  Model with all parameters filled.
    blackScholesModel a = blackScholesModel(16.2, 13.3, 18.0, 6.2, 4.5, blackScholesModel::OptionType::PUT);
    //  Model with no OptionType given.
    blackScholesModel b = blackScholesModel(5.6, 4.2, 45.3, 3.14, 2.71);
    //  Model to test default values.
    blackScholesModel c = blackScholesModel();
};


TEST_F(blackScholesModelTest, blackScholesModelTest_constructor_Test1)
{
    EXPECT_EQ(a.getUnderlyingPrice(), 16.2);
    EXPECT_EQ(a.getStrikePrice(), 13.3);
    EXPECT_EQ(a.getTimeToExperation(), 18.0);
    EXPECT_EQ(a.getRiskFreeRate(), 6.2);
    EXPECT_EQ(a.getVolatility(), 4.5);
    EXPECT_EQ(a.getOptionType(), blackScholesModel::OptionType::PUT);
}

TEST_F(blackScholesModelTest, blackScholesModelTest_constructor_Test2)
{
    EXPECT_EQ(b.getUnderlyingPrice(), 5.6);
    EXPECT_EQ(b.getStrikePrice(), 4.2);
    EXPECT_EQ(b.getTimeToExperation(), 45.3);
    EXPECT_EQ(b.getRiskFreeRate(), 3.14);
    EXPECT_EQ(b.getVolatility(), 2.71);
    EXPECT_EQ(b.getOptionType(), blackScholesModel::OptionType::CALL);
}

TEST_F(blackScholesModelTest, blackScholesModelTest_constructor_Test3)
{
    EXPECT_EQ(c.getUnderlyingPrice(), 0.0);
    EXPECT_EQ(c.getStrikePrice(), 0.0);
    EXPECT_EQ(c.getTimeToExperation(), 0.0);
    EXPECT_EQ(c.getRiskFreeRate(), 0.0);
    EXPECT_EQ(c.getVolatility(), 0.0);
    EXPECT_EQ(c.getOptionType(), blackScholesModel::OptionType::CALL);
}

TEST_F(blackScholesModelTest, calculateD1Test1)
{
    double expectedD1 = (log(16.2 / 13.3) + (6.2 + 0.5 * 4.5 * 4.5) * 18.0) / (4.5 * sqrt(18.0));
    EXPECT_NEAR(a.getD1(), expectedD1, 1e-6);
}

TEST_F(blackScholesModelTest, calculateD1Test2)
{
    double expectedD1 = (log(5.6 / 4.2) + (3.14 + 0.5 * 2.71 * 2.71) * 45.3) / (2.71 * sqrt(45.3));
    EXPECT_NEAR(b.getD1(), expectedD1, 1e-6);
}

TEST_F(blackScholesModelTest, calculateD1Test3)
{
    EXPECT_TRUE(isnan(c.getD1()));
}

TEST_F(blackScholesModelTest, calculateD2Test1)
{
    double expectedD2 = a.getD1() - 4.5 * sqrt(18.0);
    EXPECT_NEAR(a.getD2(), expectedD2, 1e-6);
}

TEST_F(blackScholesModelTest, calculateD2Test2)
{
    double expectedD2 = b.getD1() - 2.71 * sqrt(45.3);
    EXPECT_NEAR(b.getD2(), expectedD2, 1e-6);
}

TEST_F(blackScholesModelTest, calculateD2Test3)
{
    EXPECT_TRUE(isnan(c.getD2()));
}

TEST_F(blackScholesModelTest, calculateOptionPriceTest1)
{
    double expectedOptionPrice = 13.3 * exp(-6.2 * 18.0) * a.normalCDF(-a.getD2()) - 16.2 * a.normalCDF(-a.getD1());
    EXPECT_NEAR(a.calculateOptionPrice(), expectedOptionPrice, 1e-6);
}

TEST_F(blackScholesModelTest, calculateOptionPriceTest2)
{
    double expectedOptionPrice = b.getUnderlyingPrice() * b.normalCDF(b.getD1()) - b.getStrikePrice() * exp(-b.getRiskFreeRate() * b.getTimeToExperation()) * b.normalCDF(b.getD2());
    EXPECT_NEAR(b.calculateOptionPrice(), expectedOptionPrice, 1e-6);
}

TEST_F(blackScholesModelTest, calculateOptionPriceTest3)
{
    EXPECT_TRUE(isnan(c.calculateOptionPrice()));
}

TEST_F(blackScholesModelTest, calculateKTest1)
{
    double expectedK = 1.0 / (1.0 + 0.2316419 * abs(a.getD1()));
    EXPECT_NEAR(a.getK(), expectedK, 1e-6);
}

TEST_F(blackScholesModelTest, calculateKTest2)
{
    double expectedK = 1.0 / (1.0 + 0.2316419 * abs(b.getD1()));
    EXPECT_NEAR(b.getK(), expectedK, 1e-6);
}

TEST_F(blackScholesModelTest, calculateKTest3)
{
    EXPECT_TRUE(isnan(c.getK()));
}

TEST_F(blackScholesModelTest, normalCDFTestLargePositiveValue)
{
    double value = 10.0;
    double expectedNormalCDF = 1.0; // Should be very close to 1
    EXPECT_NEAR(a.normalCDF(value), expectedNormalCDF, 1e-6);
}

TEST_F(blackScholesModelTest, normalCDFTestLargeNegativeValue)
{
    double value = -10.0;
    double expectedNormalCDF = 0.0;
    EXPECT_NEAR(b.normalCDF(value), expectedNormalCDF, 1e-6);
}

TEST_F(blackScholesModelTest, normalCDFTestPositiveValue)
{
    double value = 1.0;
    double expectedNormalCDF = 0.841344746;
    EXPECT_NEAR(a.normalCDF(value), expectedNormalCDF, 1e-6);
}

TEST_F(blackScholesModelTest, normalCDFTestNegativeValue)
{
    double value = -1.0;
    double expectedNormalCDF = 0.158655254;
    EXPECT_NEAR(b.normalCDF(value), expectedNormalCDF, 1e-6);
}

TEST_F(blackScholesModelTest, normalCDFTestDefault)
{
    EXPECT_TRUE(isnan(c.normalCDF(nan(""))));
}


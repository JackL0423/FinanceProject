#include "gtest/gtest.h"
#include "../include/blackScholesModel.h"


TEST(ScholesModelTests, defaultScholesModelConstructorValues)
{
    blackScholesModel b;
    EXPECT_EQ(b.getUnderlyingPrice(), 0.0);

    EXPECT_EQ(b.getStrikePrice(), 0.0);

    EXPECT_EQ(b.getTimeToExperation(), 0.0);

    EXPECT_EQ(b.getRiskFreeRate(), 0.0);

    EXPECT_EQ(b.getVolatility(), 0.0);
}

TEST(ScholesModelTests, ScholesModelConstructorWithValuesNoOption)
{
    blackScholesModel b(5.6, 4.2, 45.3, 3.14, 2.71);
    EXPECT_EQ(b.getUnderlyingPrice(), 5.6);

    EXPECT_EQ(b.getStrikePrice(), 4.2);

    EXPECT_EQ(b.getTimeToExperation(), 45.3);

    EXPECT_EQ(b.getRiskFreeRate(), 3.14);

    EXPECT_EQ(b.getVolatility(), 2.71);
}

TEST(ScholesModelTests, ScholesModelConstructorWithValuesAndOption)
{
    blackScholesModel b(5.6, 4.2, 45.3, 3.14, 2.71, blackScholesModel::OptionType::CALL);
    EXPECT_EQ(b.getUnderlyingPrice(), 5.6);

    EXPECT_EQ(b.getStrikePrice(), 4.2);

    EXPECT_EQ(b.getTimeToExperation(), 45.3);

    EXPECT_EQ(b.getRiskFreeRate(), 3.14);

    EXPECT_EQ(b.getVolatility(), 2.71);

    EXPECT_EQ(b.getOptionType(), blackScholesModel::OptionType::CALL);
}

TEST(ScholesModelTests, calculateOptionPrice)
{
    blackScholesModel a;
    EXPECT_EQ(a.calculateOptionPrice(), 0.0);

    //  Add more asserts
}

TEST(ScholesModelTests, normalCDFcalculation)
{
    blackScholesModel b;
    EXPECT_EQ(b.normalCDF(12.7), 0.0);

    EXPECT_EQ(b.normalCDF(15.2), 0.0);
}

TEST(ScholesModelTests, calculateD1)
{
    blackScholesModel a;
    EXPECT_EQ(a.getD1(), 0.0);

    blackScholesModel b;
    EXPECT_EQ(b.getD1(), 0.0);

    blackScholesModel c;
    EXPECT_EQ(c.getD1(), 0.0);
}

// Check these as comparing double to double and may need a tolerance.
TEST(ScholesModelTests, calculateD2)
{
    blackScholesModel a;
    EXPECT_EQ(a.getD2(), 0.0);

    blackScholesModel b;
    EXPECT_EQ(b.getD2(), 0.0);

    blackScholesModel c;
    EXPECT_EQ(c.getD2(), 0.0);
}

TEST(ScholesModelTests, calculateK)
{
    blackScholesModel a;
    EXPECT_EQ(a.getK(), 0.0);

    blackScholesModel b;
    EXPECT_EQ(b.getK(), 0.0);

    blackScholesModel c;
    EXPECT_EQ(c.getK(), 0.0);
}


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

//
// Created by tripa on 7/3/2022.
//
#include "gtest/gtest.h"
#include "../common_lib/biginteger_arithmetic.h"
#include "gmock/gmock-matchers.h"


using ::testing::ElementsAreArray;
using namespace common_lib;
class BigIntegerArithmeticTest : public ::testing::Test {
};

TEST_F(BigIntegerArithmeticTest, ConvertSmallNumericDecimalStringToBigInteger) {
    const std::string input = "13";
    const absl::StatusOr<BigInteger> output = convertNumericDecimalStringToBigInteger(input);
    EXPECT_TRUE(output.ok());
    EXPECT_EQ(output->value_size(), 1);
    EXPECT_EQ(output->value(0), 13);
}

TEST_F(BigIntegerArithmeticTest, ConvertLargeNumericDecimalStringToBigInteger) {
    const std::string input = "18446744073709551615";
    const absl::StatusOr<BigInteger> output = convertNumericDecimalStringToBigInteger(input);
    EXPECT_TRUE(output.ok());
    EXPECT_EQ(output->value_size(), 2);
    EXPECT_EQ(output->value(0), 4294967295);
    EXPECT_EQ(output->value(1), 4294967295);
}
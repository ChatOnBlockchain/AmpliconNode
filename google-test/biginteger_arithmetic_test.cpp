//
// Created by tripa on 7/3/2022.
//
#include "gtest/gtest.h"
#include "../common_lib/biginteger_arithmetic.h"

using namespace common_lib;

class BigIntegerArithmeticTest : public ::testing::Test {
};

TEST_F(BigIntegerArithmeticTest, ConvertSmallNumericDecimalStringToBigInteger) {
    const std::string input = "13";
    const absl::StatusOr<BigInteger> output = convert_numeric_decimal_string_to_biginteger(input);
    EXPECT_TRUE(output.ok());
    EXPECT_EQ(output->value_size(), 1);
    EXPECT_EQ(output->value(0), 13);
    EXPECT_FALSE(output->is_negative());

    const std::string input2 = "-13";
    const absl::StatusOr<BigInteger> output2 = convert_numeric_decimal_string_to_biginteger(input2);
    EXPECT_TRUE(output2.ok());
    EXPECT_EQ(output2->value_size(), 1);
    EXPECT_EQ(output2->value(0), 13);
    EXPECT_TRUE(output2->is_negative());
}

TEST_F(BigIntegerArithmeticTest, ConvertLargeNumericDecimalStringToBigInteger) {
    const std::string input = "18446744073709551615";
    const absl::StatusOr<BigInteger> output = convert_numeric_decimal_string_to_biginteger(input);
    EXPECT_TRUE(output.ok());
    EXPECT_EQ(output->value_size(), 2);
    EXPECT_EQ(output->value(0), 4294967295);
    EXPECT_EQ(output->value(1), 4294967295);
    EXPECT_FALSE(output->is_negative());

    const std::string input2 = "-18446744073709551615";
    const absl::StatusOr<BigInteger> output2 = convert_numeric_decimal_string_to_biginteger(input2);
    EXPECT_TRUE(output2.ok());
    EXPECT_EQ(output2->value_size(), 2);
    EXPECT_EQ(output2->value(0), 4294967295);
    EXPECT_EQ(output2->value(1), 4294967295);
    EXPECT_TRUE(output2->is_negative());
}

TEST_F(BigIntegerArithmeticTest, ConvertBigIntegerToLargeNumericDecimalString) {
    const std::string input = "18446744073709551615";
    const absl::StatusOr<BigInteger> output1 = convert_numeric_decimal_string_to_biginteger(input);
    EXPECT_TRUE(output1.ok());
    const absl::StatusOr<std::string> output2 = convert_biginteger_to_decimal_string(*output1);
    EXPECT_TRUE(output2.ok());
    EXPECT_EQ(*output2, input);

    const std::string input2 = "-18446744073709551615";
    const absl::StatusOr<BigInteger> output3 = convert_numeric_decimal_string_to_biginteger(input2);
    EXPECT_TRUE(output3.ok());
    const absl::StatusOr<std::string> output4 = convert_biginteger_to_decimal_string(*output3);
    EXPECT_TRUE(output4.ok());
    EXPECT_EQ(*output4, input2);
}

TEST_F(BigIntegerArithmeticTest, AddBigIntegersFromSmallNumericStrings) {
    const std::string a = "128";
    const std::string b = "256";
    const absl::StatusOr<BigInteger> big_a = convert_numeric_decimal_string_to_biginteger(a);
    EXPECT_TRUE(big_a.ok());
    const absl::StatusOr<BigInteger> big_b = convert_numeric_decimal_string_to_biginteger(b);
    EXPECT_TRUE(big_b.ok());
    const absl::StatusOr<BigInteger> big_c = add(*big_a, *big_b);

    EXPECT_TRUE(big_c.ok());
    EXPECT_EQ(big_c->value_size(), 1);
    EXPECT_EQ(big_c->value(0), 384);
}

TEST_F(BigIntegerArithmeticTest, AddBigIntegersFromLargeNumericStrings) {
    const std::string a = "4294967295"; // UINT32_MAX
    const std::string b = "256";
    const absl::StatusOr<BigInteger> big_a = convert_numeric_decimal_string_to_biginteger(a);
    EXPECT_TRUE(big_a.ok());
    const absl::StatusOr<BigInteger> big_b = convert_numeric_decimal_string_to_biginteger(b);
    EXPECT_TRUE(big_b.ok());

    const absl::StatusOr<BigInteger> big_c = add(*big_a, *big_b);
    EXPECT_TRUE(big_c.ok());
    EXPECT_EQ(big_c->value_size(), 2);

    const absl::StatusOr<std::string> value = convert_biginteger_to_decimal_string(*big_c);
    EXPECT_TRUE(value.ok());
    EXPECT_EQ(*value, "4294967551");
}

TEST_F(BigIntegerArithmeticTest, MultiplyBigIntegersFromSmallNumericStrings) {
    const std::string a = "256";
    const std::string b = "256";
    const absl::StatusOr<BigInteger> big_a = convert_numeric_decimal_string_to_biginteger(a);
    EXPECT_TRUE(big_a.ok());
    const absl::StatusOr<BigInteger> big_b = convert_numeric_decimal_string_to_biginteger(b);
    EXPECT_TRUE(big_b.ok());

    const absl::StatusOr<BigInteger> big_c = multiply(*big_a, *big_b);
    EXPECT_TRUE(big_c.ok());
    EXPECT_EQ(big_c->value_size(), 1);

    const absl::StatusOr<std::string> value = convert_biginteger_to_decimal_string(*big_c);
    EXPECT_TRUE(value.ok());
    EXPECT_EQ(*value, "65536");
}

TEST_F(BigIntegerArithmeticTest, MultiplyBigIntegersFromLargeNumericStrings) {
    const std::string a = "4294967551";
    const std::string b = "4294967551";
    const absl::StatusOr<BigInteger> big_a = convert_numeric_decimal_string_to_biginteger(a);
    EXPECT_TRUE(big_a.ok());
    const absl::StatusOr<BigInteger> big_b = convert_numeric_decimal_string_to_biginteger(b);
    EXPECT_TRUE(big_b.ok());

    const absl::StatusOr<BigInteger> big_c = multiply(*big_a, *big_b);
    EXPECT_TRUE(big_c.ok());
    EXPECT_EQ(big_c->value_size(), 3);

    const absl::StatusOr<std::string> value = convert_biginteger_to_decimal_string(*big_c);
    EXPECT_TRUE(value.ok());
    EXPECT_EQ(*value, "18446746264142937601");
}
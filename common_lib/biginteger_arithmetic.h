//
// Created by tripa on 7/2/2022.
//

#ifndef ECC_BIGINTEGER_ARITHMETIC_H
#define ECC_BIGINTEGER_ARITHMETIC_H

#include "absl/status/statusor.h"
#include "biginteger.pb.h"

namespace common_lib {
    absl::StatusOr<BigInteger>
    convert_numeric_decimal_string_to_biginteger(const std::string &input);

    absl::StatusOr<std::string>
    convert_biginteger_to_decimal_string(const BigInteger &input);

    absl::StatusOr<BigInteger> add(const BigInteger &a, const BigInteger &b,
                                   const int max_uint32_blocks = -1);

    absl::StatusOr<BigInteger> subtract(const BigInteger &a, const BigInteger &b,
                                        const int max_uint32_blocks = -1);

    absl::StatusOr<BigInteger> multiply(const BigInteger &a, const BigInteger &b,
                                        const int max_uint_32_blocks = -1);
} // namespace common_lib
#endif // ECC_BIGINTEGER_ARITHMETIC_H

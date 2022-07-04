//
// Created by tripa on 7/2/2022.
//

#ifndef ECC_BIGINTEGER_ARITHMETIC_H
#define ECC_BIGINTEGER_ARITHMETIC_H

#include "absl/status/statusor.h"
#include "biginteger.pb.h"

namespace common_lib {
absl::StatusOr<BigInteger>
convertNumericDecimalStringToBigInteger(const std::string &input);
absl::StatusOr<std::string>
convertBigIntegerToDecimalString(const BigInteger &input);
absl::StatusOr<BigInteger> add(const BigInteger &a, const BigInteger &b,
                               const int max_uint_32_blocks = -1);
} // namespace common_lib
#endif // ECC_BIGINTEGER_ARITHMETIC_H

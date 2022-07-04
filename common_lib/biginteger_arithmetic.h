//
// Created by tripa on 7/2/2022.
//

#ifndef ECC_BIGINTEGER_ARITHMETIC_H
#define ECC_BIGINTEGER_ARITHMETIC_H

#include "biginteger.pb.h"
#include "absl/status/statusor.h"

namespace common_lib {
    absl::StatusOr<BigInteger> convertNumericDecimalStringToBigInteger(const std::string &input);
    absl::StatusOr<std::string> convertBigIntegerToDecimalString(const BigInteger& input);
    BigInteger add(const BigInteger& a, const BigInteger& b);
}
#endif //ECC_BIGINTEGER_ARITHMETIC_H

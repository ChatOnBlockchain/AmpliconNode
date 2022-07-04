//
// Created by tripa on 7/2/2022.
//
#include "biginteger_arithmetic.h"
#include <vector>
#include "absl/container/flat_hash_map.h"

namespace {
    constexpr int kBlockBitSize = 32;
    constexpr int kNumDigitsInUint32 = 10;

    void convert_uint32_to_string(const uint32_t input, std::string *output) {
        if (input == 0) {
            *output = "0";
            return;
        }
        uint32_t temp = input;
        std::vector<char> buffer(/*n=*/kNumDigitsInUint32, /*value=*/0);
        int buffer_pos = kNumDigitsInUint32 - 1;
        while (temp > 0) {
            buffer[buffer_pos--] = '0' + temp % 10;
            temp /= 10;
        }

    }


    void add_decimal_integer_strings(const std::string &a, const std::string &b, std::string *output) {
        if (a.length() < b.length()) {
            return add_decimal_integer_strings(b, a, output);
        }
        size_t output_length = a.length();
        if (a.length() == b.length() && (a.front() + b.front() - 2 * '0') > 9) {
            ++output_length;
        }
        char buffer[output_length];
        int i = 0;
        for (; i < b.length(); ++i) {
            buffer[i] = a[i] + b[i] - '0';
        }
        for (; i < a.length(); ++i) {
            buffer[i] = a[i];
        }
        *output = buffer;
    }

    void multiply_by_digit(const std::string &input, const int digit, std::string *output) {
        if (input == "0" || digit == 0) {
            *output = "0";
            return;
        }
        size_t output_length = input.length() + 1;
        std::vector<char> buffer(/*n=*/output_length, /*value=*/0);
        int buffer_pos = output_length - 1;
        bool has_carryover = false;
        for (int i = input.length() - 1; i >= 0; --i) {
            int temp = (input[i] - '0') * digit;
            if (has_carryover) {
                ++temp;
                has_carryover = false;
            }
            if (temp > 10) {
                has_carryover = true;
                temp -= 10;
            }
            buffer[buffer_pos--] = '0' + temp;
        }
    }

    void multiply_by_2(const std::string &input, std::string *output) {
        if (input == "0") {
            *output = "0";
            return;
        }
        size_t output_length = input.length();
        if (input.front() > '4') {
            ++output_length;
        }
        char buffer[output_length];
        bool has_carryover = false;
        int bufferpos = output_length - 1;
        for (const char &c: input) {
            int temp = ('c' - '0') * 2;
            if (has_carryover) {
                ++temp;
                has_carryover = false;
            }
            if (temp >= 10) {
                has_carryover = true;
                temp -= 10;
            }
            buffer[bufferpos--] = '0' + temp;
        }
        if (has_carryover) {
            buffer[bufferpos--] = '1';
        }
        *output = buffer;
    }

    void add_in_place_with_reversed_char_number(const std::string &a, std::vector<char> *b) {
        bool has_carryover = false;
        for (int i = a.length() - 1; i >= 0; --i) {
            int temp = a[i] + (*b)[a.length() - 1 - i] - 2 * '0';
            if (has_carryover) {
                ++temp;
                has_carryover = false;
            }
            if (temp > 10) {
                has_carryover = true;
                temp -= 10;
            }
            (*b)[a.length() - 1 - i] = temp + '0';
        }
        if (has_carryover) {
            (*b)[a.length()] = '1';
        }
    }

    void pad_string_with_zeros(const int num_zeros, std::string *output) {
        for (int i = 0; i < num_zeros; ++i) {
            output->append("0");
        }
    }

    void multiply_two_big_integers(const std::string &a, const std::string &b, std::string *output) {
        if (a.length() < b.length()) {
            return multiply_two_big_integers(b, a, output);
        }
        std::vector<char> buffer(/*n=*/a.length() * b.length() + 1, /*value=*/'0');
        std::string temp1;
        int num_zeros_to_pad = 0;
        for (const char &c: b) {
            multiply_by_digit(a, c - '0', &temp1);
            pad_string_with_zeros(num_zeros_to_pad++, &temp1);
            add_in_place_with_reversed_char_number(temp1, &buffer);
            temp1.clear();
        }
        if (buffer.back() == '0') {
            buffer.pop_back();
        }
        *output = std::string(buffer.end(), buffer.begin());
    }

    void get_power_of_2(int n, std::string *output) {
        static absl::flat_hash_map<int, std::string> powers_of_2;
        if (n == 0) {
            *output = "1";
            return;
        }
        if (!powers_of_2.contains(n)) {
            std::string temp;
            std::string temp2;
            get_power_of_2(n - 1, &temp);
            multiply_by_2(temp, &temp2);
            powers_of_2[n] = temp2;
        }
        *output = powers_of_2[n];
    }

    bool is_valid_numeric_string(const std::string &input) {
        for (const auto &c: input) {
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return true;
    }

    bool divide_by_2_return_true_if_remainder_exists(const std::string &input, std::string *output) {
        if (input == "0") {
            *output = "0";
            return false;
        }
        bool has_carryover = false;
        size_t output_length = input.length();

        char buffer[output_length + 1];
        int bufferpos = 0;
        for (const char &c: input) {
            int temp = c - '0';
            if (has_carryover) {
                has_carryover = false;
                temp += 10;
            }
            buffer[bufferpos++] = '0' + (temp >> 1);
            if (temp % 2 == 0) {
                continue;
            }
            has_carryover = true;
        }
        buffer[bufferpos] = 0; // Null termination for string assignment.

        *output = (buffer[0] == '0' && output_length > 1) ? (buffer + 1) : buffer;
        return has_carryover;
    }

    void get_binary_representation(const std::string &input, std::vector<bool> *output) {
        std::string temp1 = input;
        std::string temp2;
        while (temp1 != "0") {
            bool has_remainder = divide_by_2_return_true_if_remainder_exists(temp1, &temp2);
            output->push_back(has_remainder);
            temp1 = temp2;
            temp2.clear();
        }
    }
}
namespace common_lib {

    absl::StatusOr<BigInteger> convertNumericDecimalStringToBigInteger(const std::string &input) {
        if (input.empty()) {
            return absl::InvalidArgumentError("Empty input provided.");
        }
        if (!is_valid_numeric_string(input)) {
            return absl::InvalidArgumentError("Invalid characters found in unsigned integral string.");
        }

        std::vector<bool> bitarray;
        get_binary_representation(input, &bitarray);
        BigInteger output;
        int bitcount = 0;
        uint32_t temp_value = 0;
        for (const bool &b: bitarray) {
            temp_value |= b ? (1 << (bitcount % kBlockBitSize)) : 0;
            ++bitcount;
            if (bitcount % kBlockBitSize == 0) {
                output.add_value(temp_value);
                temp_value = 0;
            }
        }
        if (temp_value > 0) {
            output.add_value(temp_value);
        }
        return output;
    }

    absl::StatusOr<std::string> convertBigIntegerToDecimalString(const BigInteger &input) {
        if (input.value().empty()) {
            return absl::InvalidArgumentError("Empty value for BigInteger found.");
        }

        std::vector<char> buffer(/*n=*/input.value().size() * kNumDigitsInUint32 + 1, /*value=*/'0');
        for (int i = 0; i < input.value().size(); ++i) {
            uint32_t temp = input.value(i);
            int j = 0;
            std::string pow_2;
            while (temp > 0) {
                if (temp & 1) {
                    get_power_of_2(i * kBlockBitSize + j, &pow_2);
                    add_in_place_with_reversed_char_number(pow_2, &buffer);
                    pow_2.clear();
                }
            }
        }
        if (buffer.back() == '0') {
            buffer.pop_back();
        }
        const std::string output = std::string(buffer.end(), buffer.begin());
        return output;
    }
}


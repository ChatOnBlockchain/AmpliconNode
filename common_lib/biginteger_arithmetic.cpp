//
// Created by tripa on 7/2/2022.
//
#include "biginteger_arithmetic.h"
#include <vector>
#include "absl/container/flat_hash_map.h"

namespace common_lib {

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
            std::vector<char> buffer(/*n=*/output_length, /*value=*/'0');
            int buffer_pos = output_length - 1;
            bool has_carryover = false;
            for (int i = input.length() - 1; i >= 0; --i) {
                int temp = (input[i] - '0') * digit;
                if (has_carryover) {
                    ++temp;
                    has_carryover = false;
                }
                if (temp >= 10) {
                    has_carryover = true;
                    temp -= 10;
                }
                buffer[buffer_pos--] = '0' + temp;
            }
            if (has_carryover) {
                buffer[buffer_pos] = '1';
            }
            const int offset = buffer.front() == '0' ? 1 : 0;
            *output = std::string(buffer.begin() + offset, buffer.end());
        }


        void add_in_place_with_reversed_char_number(const std::string &a, std::vector<char> *b) {
            bool has_carryover = false;
            for (int i = a.length() - 1; i >= 0; --i) {
                int temp = a[i] + (*b)[a.length() - 1 - i] - 2 * '0';
                if (has_carryover) {
                    ++temp;
                    has_carryover = false;
                }
                if (temp >= 10) {
                    has_carryover = true;
                    temp -= 10;
                }
                (*b)[a.length() - 1 - i] = temp + '0';
            }

            int j = a.length();
            while (has_carryover && j < b->size()) {
                int temp = (*b)[j] - '0' + 1;
                has_carryover = false;
                if (temp >= 10) {
                    has_carryover = true;
                    temp -= 10;
                }
                (*b)[j] = '0' + temp;
                ++j;
            }
        }

        void pad_string_with_zeros(const int num_zeros, std::string *output) {
            for (int i = 0; i < num_zeros; ++i) {
                output->append("0");
            }
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
                multiply_by_digit(temp,/*digit=*/2, &temp2);
                powers_of_2[n] = temp2;
            }
            *output = powers_of_2[n];
        }

        bool is_valid_numeric_string(const std::string &input) {
            bool found_sign = false;
            for (const auto &c: input) {
                if (!found_sign && (c == '-' || c == '+')) {
                    found_sign = true;
                    continue;
                }
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

        void multiply_big_integer_with_uint32(const BigInteger &a, const uint32_t b, BigInteger *output) {
            uint64_t carry_over = 0;
            for (int i = 0; i < a.value_size(); ++i) {
                uint64_t temp = a.value(i) * b;
                if (carry_over > 0) {
                    temp += carry_over;
                    carry_over = 0;
                }
                carry_over = temp >> kBlockBitSize;
                output->add_value((uint32_t) (temp & UINT32_MAX));
            }
            if (carry_over > 0) {
                output->add_value(carry_over);
            }
        }

        void shift_blocks_left(const BigInteger &input, const int num_blocks, const int max_output_blocks,
                               BigInteger *output) {
            int max_blocks = input.value_size() + num_blocks;
            if (max_output_blocks > 0 && max_blocks > max_output_blocks) {
                max_blocks = max_output_blocks;
            }
            int i = 0;
            while (i < num_blocks && i < max_blocks) {
                output->add_value(0);
                ++i;
            }
            while (i < input.value_size() + num_blocks && i < max_blocks) {
                output->add_value(input.value(i - num_blocks));
                ++i;
            }
        }

        bool have_same_sign(const BigInteger &a, const BigInteger &b) {
            return a.is_negative() == b.is_negative();
        }

        void toggle_sign(const BigInteger &input, BigInteger* output){
            *output = input;
            output->set_is_negative(!input.is_negative());
        }

    } //namespace

    absl::StatusOr<BigInteger> convert_numeric_decimal_string_to_biginteger(const std::string &input) {
        if (input.empty()) {
            return absl::InvalidArgumentError("Empty input provided.");
        }
        if (!is_valid_numeric_string(input)) {
            return absl::InvalidArgumentError("Invalid characters found in unsigned integral string.");
        }

        std::string integral_string_without_sign = (input.front() == '-' || input.front() == '+') ? std::string(
                input.begin() + 1, input.end()) : input;
        std::vector<bool> bitarray;
        get_binary_representation(integral_string_without_sign, &bitarray);
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
        if(input.front() == '-'){
            output.set_is_negative(true);
        }
        return output;
    }

    absl::StatusOr<std::string> convert_biginteger_to_decimal_string(const BigInteger &input) {
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
                temp = temp >> 1;
                ++j;
            }
        }
        while (buffer.back() == '0') {
            buffer.pop_back();
        }
        if(input.is_negative()){
            buffer.push_back('-');
        }
        std::reverse(buffer.begin(), buffer.end());
        std::string output = std::string(buffer.begin(), buffer.end());
        return output;
    }

    absl::StatusOr<BigInteger> add(const BigInteger &a, const BigInteger &b, const int max_uint32_blocks) {
        if (a.value().empty() || b.value().empty()) {
            return absl::InvalidArgumentError("Empty values cannot be added.");
        }
        if (a.value_size() < b.value_size()) {
            return add(b, a, max_uint32_blocks);
        }
        if (!have_same_sign(a, b)) {
            BigInteger toggled_input;
            if(a.is_negative()){
                toggle_sign(a, &toggled_input);
                return subtract(b, toggled_input, max_uint32_blocks);
            }
            toggle_sign(b, &toggled_input);
            return subtract(a, toggled_input, max_uint32_blocks);
        }

        int max_blocks = a.value_size() + 1;
        if (max_uint32_blocks > 0 && max_uint32_blocks < max_blocks) {
            max_blocks = max_uint32_blocks;
        }
        int min_blocks = b.value_size();
        if (min_blocks > max_blocks) {
            min_blocks = max_blocks;
        }
        bool has_carryover = false;
        BigInteger output;
        int i = 0;
        for (; i < min_blocks; ++i) {
            uint64_t temp = (uint64_t) a.value(i) + (uint64_t) b.value(i);
            if (has_carryover) {
                ++temp;
                has_carryover = false;
            }
            if (temp > UINT32_MAX) {
                has_carryover = true;
                temp = temp % ((uint64_t) UINT32_MAX + 1);
            }
            output.add_value((uint32_t) temp);
        }
        for (; i < a.value_size(); ++i) {
            uint64_t temp = a.value(i);
            if (has_carryover) {
                ++temp;
                has_carryover = false;
            }
            if (temp > UINT32_MAX) {
                has_carryover = true;
                temp = temp % UINT32_MAX;
            }
            output.add_value((uint32_t) temp);
        }
        if (has_carryover && i < max_blocks) {
            output.add_value(/*value=*/1);
        }
        return output;
    }

    absl::StatusOr<BigInteger> subtract(const BigInteger &a, const BigInteger &b, const int max_uint32_blocks) {
        if (a.value().empty() || b.value().empty()) {
            return absl::InvalidArgumentError("Empty values cannot be added.");
        }
        if (a.value_size() < b.value_size()) {
            return add(b, a, max_uint32_blocks);
        }
        if (!have_same_sign(a, b)) {
            BigInteger toggled_input;
            toggle_sign(b, &toggled_input);
            return add(a, toggled_input, max_uint32_blocks);
        }

        BigInteger output;
        output.add_value(0); // Zero initialization
        return output;
    }

    absl::StatusOr<BigInteger> multiply(const BigInteger &a, const BigInteger &b,
                                        const int max_uint_32_blocks) {
        if (a.value().empty() || b.value().empty()) {
            return absl::InvalidArgumentError("Empty values cannot be multiplied.");
        }
        if (a.value_size() < b.value_size()) {
            return multiply(b, a, max_uint_32_blocks);
        }

        int max_blocks = a.value_size() * b.value_size() + 1;
        if (max_uint_32_blocks > 0 && max_uint_32_blocks < max_blocks) {
            max_blocks = max_uint_32_blocks;
        }
        BigInteger output;
        output.add_value(0); // Zero initialization

        if (b.value_size() == 1 && b.value(0) == 0) {
            return output;
        }

        for (int i = 0; i < b.value_size(); ++i) {
            BigInteger temp1;
            multiply_big_integer_with_uint32(a, b.value(i), &temp1);
            BigInteger temp2;
            shift_blocks_left(temp1, i, max_blocks, &temp2);
            const absl::StatusOr<BigInteger> temp_result = add(output, temp2, max_blocks);
            output = *temp_result;
        }
        if (!have_same_sign(a, b)) {
            output.set_is_negative(true);
        }
        return output;
    }
} //namespace common_lib



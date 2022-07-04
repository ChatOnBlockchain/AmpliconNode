//
// Created by tripa on 6/11/2022.
//

#include <algorithm>
#include <cassert>
#include "kmp_search.h"

namespace common_lib {
    kmp_search::kmp_search(const std::string &query, bool enable_reverse_order_search) {
        kmp_table_ = nullptr;
        kmp_reverse_table_ = nullptr;
        if (query.empty()) {
            return;
        }
        query_ = query;
        kmp_table_ = new std::vector<int>(query.length() + 1, 0);
        _make_kmp_table(query, kmp_table_);
        if (enable_reverse_order_search) {
            reversed_query_ = query;
            std::reverse(reversed_query_.begin(), reversed_query_.end());
            kmp_reverse_table_ = new std::vector<int>(query.length() + 1, 0);
            _make_kmp_table(reversed_query_, kmp_reverse_table_);
        }
    }

    kmp_search::~kmp_search() {
        if (kmp_table_ != nullptr) {
            delete kmp_table_;
        }
        if (kmp_reverse_table_ != nullptr) {
            delete kmp_reverse_table_;
        }
    }

    void kmp_search::_make_kmp_table(const std::string &input, std::vector<int> *output) {
        if (input.empty()) { return; }

        int output_table_position = 1;
        int query_candidate_position = 0;
        int input_string_length = input.length();

        (*output)[0] = -1;

        while (output_table_position < input_string_length) {
            if (input[output_table_position] == input[query_candidate_position]) {
                (*output)[output_table_position] = (*output)[query_candidate_position];
            } else {
                (*output)[output_table_position] = query_candidate_position;
                while (query_candidate_position >= 0 &&
                       input[query_candidate_position] != input[output_table_position]) {
                    query_candidate_position = (*output)[query_candidate_position];
                }
            }
            ++output_table_position;
            ++query_candidate_position;
        }
        (*output)[output_table_position] = query_candidate_position;
    }

    int
    kmp_search::_perform_kmp_search(const std::string &input, const std::string &query,
                                    const std::vector<int> *kmp_table,
                                    bool return_after_first_match, std::vector<int> *found_match_indices) {
        assert(return_after_first_match == true || found_match_indices != nullptr);
        assert(kmp_table != nullptr);

        if (input.empty()) { return -1; }
        int position_input = 0;
        int position_query = 0;
        int first_match_position = -1;
        int query_length = query.length();

        while (position_input < input.length()) {
            if (input[position_input] == query[position_query]) {
                ++position_input;
                ++position_query;
                if (position_query == query_length) {
                    // match found
                    if (first_match_position == -1 && return_after_first_match) {
                        return position_input - position_query;
                    } else if (first_match_position == -1) {
                        first_match_position = (position_input - position_query);
                    }
                    found_match_indices->push_back((position_input - position_query));

                    position_query = (*kmp_table)[position_query];
                }
            } else {
                position_query = (*kmp_table)[position_query];
                if (position_query < 0) {
                    ++position_query;
                    ++position_input;
                }
            }
        }
        return first_match_position;
    }

    int kmp_search::find_first_instance(const std::string &input) {
        return _perform_kmp_search(input, query_, kmp_table_, /*return_after_first_match=*/
                                   true, /*found_match_indices=*/
                                   nullptr);
    }

    int kmp_search::find_last_instance(const std::string &input) {
        std::string reversed_input = input;
        std::reverse(reversed_input.begin(), reversed_input.end());
        int output = _perform_kmp_search(reversed_input, reversed_query_,
                                         kmp_reverse_table_, /*return_after_first_match=*/
                                         true, /*found_match_indices=*/
                                         nullptr);
        if (output != -1) {
            output = input.length() - output - reversed_query_.length();
        }
        return output;
    }

    int kmp_search::find_all_instances(const std::string &input, std::vector<int> *output) {
        return _perform_kmp_search(input, query_, kmp_table_, /*return_after_first_match=*/false, output);
    }
}
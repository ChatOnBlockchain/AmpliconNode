//
// Created by tripa on 6/11/2022.
//

#include "gtest/gtest.h"
#include "../common_lib/kmp_search.h"
#include "gmock/gmock-matchers.h"

using ::testing::ElementsAreArray;
using namespace common_lib;
class KmpSearchTest : public ::testing::Test {
};

TEST_F(KmpSearchTest, CheckFindFirstMatch) {
    const std::string input = "Why is this a good test?";
    const std::string query = "is";

    const int first_match_position = kmp_search(query, /*enable_reverse_order_search=*/false).find_first_instance(
            input);

    EXPECT_EQ(first_match_position, 4);
}

TEST_F(KmpSearchTest, CheckFindLastMatch) {
    const std::string input = "Why is this a good test?";
    const std::string query = "is";

    const int last_match_position = kmp_search(query, /*enable_reverse_order_search=*/true).find_last_instance(
            input);

    EXPECT_EQ(last_match_position, 9);
}

TEST_F(KmpSearchTest, CheckFindAllMatches) {
    const std::string input = "Why is this a good test?";
    const std::string query = "is";
    std::vector<int> match_indices;
    const int first_match_position = kmp_search(query, /*enable_reverse_order_search=*/false).find_all_instances(
            input, &match_indices);

    EXPECT_THAT(match_indices, ElementsAreArray({4, 9}));
}

TEST_F(KmpSearchTest, CheckFindNoMatches) {
    const std::string input = "Why is this a good test?";
    const std::string query = "all is";
    std::vector<int> match_indices;
    const int first_match_position = kmp_search(query, /*enable_reverse_order_search=*/false).find_all_instances(
            input, &match_indices);

    EXPECT_EQ(first_match_position, -1);
    EXPECT_TRUE(match_indices.empty());
}
//
// Created by tripa on 6/11/2022.
//

#ifndef AMPLICONSIMULATION_KMP_SEARCH_H
#define AMPLICONSIMULATION_KMP_SEARCH_H

#include <string>
#include <vector>

namespace common_lib {
class kmp_search {
public:
  kmp_search(const std::string &query,
             bool enable_reverse_order_search = false);

  ~kmp_search();

  int find_first_instance(const std::string &input);

  int find_last_instance(const std::string &input);

  int find_all_instances(const std::string &input, std::vector<int> *output);

private:
  std::vector<int> *kmp_table_;
  std::vector<int> *kmp_reverse_table_;
  std::string query_;
  std::string reversed_query_;

  void _make_kmp_table(const std::string &input, std::vector<int> *output);

  int _perform_kmp_search(const std::string &input, const std::string &query,
                          const std::vector<int> *kmp_table,
                          bool return_after_first_match,
                          std::vector<int> *found_match_indices);
};
} // namespace common_lib

#endif // AMPLICONSIMULATION_KMP_SEARCH_H

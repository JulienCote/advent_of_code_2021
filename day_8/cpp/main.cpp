#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <regex>

#include <unordered_map>

namespace
{
  // first solution with recursion/cache, then I learned math existed
  int get_triangular_sum_rec(int n)
  {
    static std::vector<int> cache {0, 1};
    if (cache.size() > n)
      return cache[n];
    int new_sum = n + get_triangular_sum_rec(n - 1);
    cache.emplace_back(new_sum);
    return new_sum;
  }
  
  int get_triangular_sum(int n)
  {
    return n * (n + 1) / 2;
  }

  // assumes sorted crabs
  int first_star(const std::vector<int>& crabs)
  {
    const int median_crab = crabs.at(crabs.size() / 2);

    int total_distance_from_median = 0;
    for (auto c : crabs)
      total_distance_from_median += std::abs(c - median_crab);

    return total_distance_from_median;
  }

  // assumes sorted crabs
  int second_star(const std::vector<int>& crabs)
  {
    const int min_crab = crabs.front();
    const int max_crab = crabs.back();
    const int crabRange = max_crab - min_crab;

    //TODO: should be able to do a binary search. The outputs should be quadratically distributed around the answer
    std::vector<int> gas_sums_by_position;
    gas_sums_by_position.resize(crabRange + 1);

    for (auto c : crabs)
    {
      for (int i = c; i > min_crab; --i)
        gas_sums_by_position[i] += get_triangular_sum(std::abs(c - i));

      for (int i = c; i < max_crab; ++i)
        gas_sums_by_position[i] += get_triangular_sum(std::abs(c - i));
    }

   return *std::min_element(gas_sums_by_position.cbegin(), gas_sums_by_position.cend());
  }
}

int main()
{
  auto file = load_data::load_file("../data", ',', 0, std::numeric_limits<size_t>::max());
  std::vector<int> crabs = load_data::parse_string<int>(file[0]);
  std::sort(crabs.begin(), crabs.end()); // everyone needs sorted data

  std::cout << "First Answer: " << first_star(crabs) << std::endl;
  std::cout << "Second Answer: " << second_star(crabs) << std::endl;
}

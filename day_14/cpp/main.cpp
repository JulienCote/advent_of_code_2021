#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

#include <list>
#include <map>
#include <unordered_map>
#include <array>
#include <algorithm>

namespace
{
  // return a pair containing both the min and max values out of the map
  template<typename K, typename V>
  std::pair<V, V> min_max(const std::unordered_map<K, V>& poly_count)
  {
    V min = std::numeric_limits<V>::max();
    V max = std::numeric_limits<V>::min();
    for (const auto& count : poly_count)
    {
      min = std::min(count.second, min);
      max = std::max(count.second, max);
    }

    return std::make_pair(min, max);
  }

  // lenghten polymer according to the insertion rules
  void grow_polymer(std::list<char>& polymer, const std::map<std::array<char,2>, char>& insertion_rules)
  {
    for (auto it = polymer.begin(); it != std::prev(polymer.end()); std::advance(it, 2))
    {
      const char curr = *it;
      const auto nextIt = std::next(it);
      const auto rule = insertion_rules.at({curr, *nextIt});
      polymer.insert(nextIt, rule);
    }
  }

  // brute force -> generate the whole polymer sequence
  int first_star(std::list<char> polymer, const std::map<std::array<char,2>, char>& insertion_rules)
  {
    for (size_t i = 0; i < 10; ++i)
      grow_polymer(polymer, insertion_rules);

    std::unordered_map<char, long> poly_count;
    poly_count.reserve(26);
    for (const auto& p : polymer)
      ++poly_count[p];

    const auto extremums = min_max(poly_count);
    return extremums.second - extremums.first;
  }

  // count number of times a pair of polymer is found at each step
  // insert as many new elements as there are pairs
  // generate new set of pairs with the new counts and repeat N(40 in this case) times
  long second_star(std::list<char> polymer, const std::map<std::array<char,2>, char>& insertion_rules)
  {
    std::unordered_map<char, long> poly_count;
    poly_count.reserve(26);

    for (const auto& element : polymer)
      ++poly_count[element];

    std::map<std::array<char,2>, size_t> element_pair_count;

    //bootstrap it with the initial pairs
    for (auto it = polymer.begin(); it != std::prev(polymer.end()); ++it)
    {
      const char curr = *it;
      const char next = *std::next(it);
      ++element_pair_count[std::to_array({curr, next})];
    }

    for (size_t i = 0; i < 40; ++i)
    {
      std::map<std::array<char,2>, size_t> next_pairs;

      for (const auto& pair : element_pair_count)
      {
        const char inserted = insertion_rules.at(pair.first);
        poly_count[inserted] += pair.second;

        next_pairs[std::to_array({pair.first[0], inserted})] += pair.second;
        next_pairs[std::to_array({inserted, pair.first[1]})] += pair.second;
      }

      element_pair_count = std::move(next_pairs);
    }

    const auto extremums = min_max(poly_count);
    return extremums.second - extremums.first;
  }
}

int main()
{
  auto file = load_data::load_file("../data");


  std::list<char> polymer;
  std::map<std::array<char,2>, char> insertion_rules;

  bool parsing_polymer = true;
  for (const auto& line : file)
  {
    if (line.empty())
    {
      parsing_polymer = false;
    }
    else if (parsing_polymer)
    {
      for (const auto& c : line)
        polymer.emplace_back(c);
    }
    else
    {
      insertion_rules.emplace(std::to_array({line[0], line[1]}), line[6]);
    }
  }

  std::cout << "First Answer: " << first_star(polymer, insertion_rules) << std::endl;
  std::cout << "First Answer: " << second_star(polymer, insertion_rules) << std::endl;

}
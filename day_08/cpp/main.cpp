#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

#include <unordered_map>
#include <set>
#include <algorithm>
#include <cmath>

namespace
{
  int first_star(const std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>>& ios)
  {
    int number_count = 0;
    for (const auto& io : ios)
    {
      for (const auto& o : io.second)
      {
        // 2, 4, 3, and 7 are the (unique) lengths for numbers 1, 4, 7, and 8
        if (o.length() == 2 || o.length() == 4 || o.length() == 3 || o.length() == 7)
          number_count += 1;
      }
    }
    return number_count;
  }

  int second_star(const std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>>& ios)
  {
    int result = 0;
    for (const auto& io : ios)
    {
      auto inputs = io.first;
      const auto& outputs = io.second;

      std::unordered_map<short, std::set<char>> mapping;
      for (const auto& in : inputs)
      {
        if (in.length() == 2)
          mapping[1] = std::set(in.begin(), in.end());
        else if (in.length() == 4)
          mapping[4] = std::set(in.begin(), in.end());
        // we don't need to intersect with 7 to solve this
        // 8 doesn't actually reveal anything about anything so let's skip it
      }

      int value = 0;
      int factor = 1000;
      for (const auto& out : outputs)
      {
        if (out.length() == 2)      // 1 has 2 segments
          value += 1 * factor;
        else if (out.length() == 4) // 4 has 4 segments
          value += 4 * factor;
        else if (out.length() == 3) // 7 has 3 segments
          value += 7 * factor;
        else if (out.length() == 7) // 8 has 7 segments
          value += 8 * factor;
        else
        {
          std::set<char> set(out.begin(), out.end());
          std::set<char> intersection_with_1;
          std::set<char> intersection_with_4;
          std::set_intersection(set.begin(), set.end(), mapping[1].begin(), mapping[1].end(), std::inserter(intersection_with_1, intersection_with_1.begin()));
          std::set_intersection(set.begin(), set.end(), mapping[4].begin(), mapping[4].end(), std::inserter(intersection_with_4, intersection_with_4.begin()));

          if (out.length() == 5)  // 2, 3, and 6 have 5 segments
          {
            if (intersection_with_1.size() == 2) // 1 and 3 have 2 segments that intersect
              value += 3 * factor;
            else if (intersection_with_4.size() == 2) // 2 and 4 have 2 segments that intersect
              value += 2 * factor;
            else
              value += 5 * factor;
          }
          else if (out.length() == 6) // 0, 6, and 9 and 6 segments
          {
            if (intersection_with_1.size() == 1) // 1 and 6 have 1 segment that intersect
              value += 6 * factor;
            else if (intersection_with_4.size() == 4) // 9 and 4 have 4 segments that intersect
              value += 9 * factor;
          }
        }

        factor = std::round(factor/10.);
      }
      result += value;
    }
    return result;
  }
}

int main()
{
  auto file = load_data::load_file("../data", '|');

  std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> ios;
  for (const auto& line : file)
    ios.emplace_back(std::make_pair(load_data::split(line[0], ' '), load_data::split(line[1], ' ')));

  std::cout << "First Answer: " << first_star(ios) << std::endl;
  std::cout << "Second Answer: " << second_star(ios) << std::endl;
}

#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <regex>

#include <unordered_map>
#include <unordered_set>
#include <stack>

namespace
{
  const std::unordered_set<char> opening_characters {'(', '[', '{', '<'};
  const std::unordered_set<char> closing_characters {')', ']', '}', '>'};
  const std::unordered_map<char, char> matching_characters { {')','('}, {']','['}, {'}','{'}, {'>','<'} };
  const std::unordered_map<char, int> point_per_error {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137},  // corruption errors
                                                       {'(', 1}, {'[',  2}, {'{',    3}, {'<',     4}}; // incomplete errors

  int first_star(const std::vector<std::string>& lines)
  {
    int points = 0;
    for (const auto& line : lines)
    {
      std::stack<char> chunk_pairing;
      for (const auto& c : line)
      {
        if (opening_characters.contains(c))
          chunk_pairing.push(c);
        else if (closing_characters.contains(c))
        {
          if (matching_characters.at(c) != chunk_pairing.top())
          {
            points += point_per_error.at(c);
            break; // only consider the first error of any line
          }
          chunk_pairing.pop();
        }
        else
        {
          throw std::runtime_error("Unexpected character received");
        }
      }
    }

    return points;
  }

  // TODO: refactor to compute both first and second star values in a single pass
  long second_star(const std::vector<std::string>& lines)
  {
    std::vector<long> points;
    points.reserve(lines.size());

    for (const auto& line : lines)
    {
      std::stack<char> chunk_pairing;
      bool corrupted_line = false;
      for (const auto& c : line)
      {
        if (opening_characters.contains(c))
          chunk_pairing.push(c);
        else if (closing_characters.contains(c))
        {
          if (matching_characters.at(c) != chunk_pairing.top())
          {
            corrupted_line = true;
            break; // ignore corrupted entries
          }
          chunk_pairing.pop();
        }
        else
        {
          throw std::runtime_error("Unexpected character received");
        }
      }

      if (corrupted_line)
        continue; // ignore corrupted entries

      constexpr short point_factor_per_error = 5;
      long line_points = 0;
      while (!chunk_pairing.empty())
      {
        line_points *= point_factor_per_error;
        line_points += point_per_error.at(chunk_pairing.top());
        chunk_pairing.pop();
      }
      points.emplace_back(line_points);
    }

    std::sort(points.begin(), points.end());
    return points.at(points.size() / 2); // the valid score is the median score (always guaranteed an odd number of answers to choose from)
  }
}

int main()
{
  auto lines = load_data::load_file("../data");

  std::cout << "First Answer: " << first_star(lines) << std::endl;
  std::cout << "Second Answer: " << second_star(lines) << std::endl;

}
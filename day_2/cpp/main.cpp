#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

namespace
{
  int first_star(const std::vector<std::string>& actions, const std::vector<int>& values)
  {
    int horizontal = 0;
    int depth = 0;
    for (size_t i = 0; i < actions.size(); ++i)
    {
      const std::string& a = actions[i];

      if (a == "up")
        depth -= values[i];
      else if (a == "down")
        depth += values[i];
      else if (a == "forward")
        horizontal += values[i];
      else
        throw std::runtime_error("unexpected action");
    }
    return horizontal * depth;
  }

  int64_t second_star(const std::vector<std::string>& actions, const std::vector<int>& values)
  {
    int64_t horizontal = 0;
    int64_t depth = 0;
    int64_t aim = 0;
    for (size_t i = 0; i < actions.size(); ++i)
    {
      const std::string& a = actions[i];

      if (a == "up")
      {
        aim -= values[i];
      }
      else if (a == "down")
      {
        aim += values[i];
      }
      else if (a == "forward")
      {
        horizontal += values[i];
        depth += values[i] * aim;
      }
      else
      {
        throw std::runtime_error("unexpected action");
      }
    }
    return horizontal * depth;
  }
}

int main()
{
  auto file = load_data::load_file_csv("../data", ' ');
  std::vector<std::string> actions = std::move(file[0]);
  std::vector<int> values = load_data::parse_string<int>(file[1]);

  if (actions.size() != values.size())
    throw std::runtime_error("There must be the same number of actions and values. Action count: " + std::to_string(actions.size()) + ". "
      + "Values count: " + std::to_string(values.size()) + ". " );

  auto first_start_answer = first_star(actions, values);
  std::cout << "First Answer: " << first_start_answer << std::endl;

  auto second_start_answer = second_star(actions, values);
  std::cout << "Second Answer: " << second_start_answer << std::endl;

}
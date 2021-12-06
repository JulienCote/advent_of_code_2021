#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <regex>

int main()
{
  auto file = load_data::load_file("../data");
  const std::regex parse_coordinates("^([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+)$");

  std::vector<std::array<int,4>> coordinates;
  for (const auto& str : file)
  {
    auto it_begin = std::sregex_iterator(str.begin(), str.end(), parse_coordinates);

    //TODO: this doesn't work, only the first value is valid
    coordinates.emplace_back(
      std::array<int,4>{
        std::atoi(it_begin->str().c_str()),
        std::atoi((++it_begin)->str().c_str()),
        std::atoi((++it_begin)->str().c_str()),
        std::atoi((++it_begin)->str().c_str())});
  }

  std::map<std::pair<int,int>, size_t> count_straight;
  std::map<std::pair<int,int>, size_t> count_all;

  for (auto& c : coordinates)
  {
    const bool same_x = c[0] == c[2];
    const bool same_y = c[1] == c[3];
    const bool is_diagonal = same_x || same_y;
    const int delta_x = (same_x ? 0 : (c[0] > c[2] ? 1 : -1));
    const int delta_y = (same_y ? 0 : (c[1] > c[3] ? 1 : -1));


    while (c[0] != c[2] || c[1] != c[3])
    {
      std::cout << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3] << std::endl;

      auto key = std::make_pair(c[0], c[1]);
      if (!is_diagonal)
        ++count_straight[key];
      ++count_all[key];
      c[0] += delta_x;
      c[1] += delta_y;
    }
  }

  auto count_dangerous_vents =
    [](const auto& container)
    {
      size_t answer = 0;
      for (const auto& c : container)
      {
        if (c.second >= 2)
          ++answer;
      }
      return answer;
    };

  size_t answer_one = count_dangerous_vents(count_straight);
  size_t answer_two = count_dangerous_vents(count_all);


  std::cout << "First Answer: " << answer_one << std::endl;
  std::cout << "Second Answer: " << answer_two << std::endl;

}
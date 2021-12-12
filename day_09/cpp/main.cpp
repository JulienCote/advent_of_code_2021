#include "../../common/cpp/load_data.h"
#include "lava_tubes.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <regex>
#include <algorithm>

namespace
{
  template <typename T, size_t X, size_t Y, AdjacencyType ...AdjTypes>
  int first_star(LavaTubes<T,X,Y,AdjTypes...> tubes)
  {
    auto mins = tubes.get_low_points();
    return std::accumulate(mins.begin(), mins.end(), static_cast<T>(0)) + mins.size();
  }

  template <typename T, size_t X, size_t Y, AdjacencyType ...AdjTypes>
  long second_star(LavaTubes<T,X,Y,AdjTypes...> tubes)
  {
    auto basins = tubes.get_basins(9);
    std::sort(basins.begin(), basins.end(), std::greater<long>());

    return basins[0] * basins[1] * basins[2];
  }
}

int main()
{
  auto lines = load_data::load_file_single_char("../data");

  std::vector<std::vector<short>> heights;
  for (const auto& line : lines)
    heights.emplace_back(load_data::parse_string<short>(line));

  LavaTubes<short, 100, 100, AdjacencyType::CARDINAL> tubes(heights);

  std::cout << "First Answer: " << first_star(tubes) << std::endl;
  std::cout << "Second Answer: " << second_star(tubes) << std::endl;

}
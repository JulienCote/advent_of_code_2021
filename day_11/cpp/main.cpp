#include "../../common/cpp/load_data.h"
#include "octopus_flash.h"

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
  template <typename T, size_t X, size_t Y>
  int first_star(OctopusFlash<T,X,Y> octopodes, size_t number_steps)
  {
    while (number_steps--)
      octopodes.step_forward();

    return octopodes.get_flash_count();
  }

  template <typename T, size_t X, size_t Y>
  long second_star(OctopusFlash<T,X,Y> octopodes)
  {
    constexpr size_t octopodes_count = octopodes.get_octopus_count();

    size_t step_count = 0;
    do //assumes the initial state is never perfectly aligned
    {
      ++step_count;
    } while (octopodes_count != octopodes.step_forward());

    return step_count;
  }
}

int main()
{
  auto lines = load_data::load_file_single_char("../data");

  std::vector<std::vector<short>> starting_energy_levels;
  for (const auto& line : lines)
    starting_energy_levels.emplace_back(load_data::parse_string<short>(line));

  OctopusFlash<short, 10, 10> octopodes(starting_energy_levels, 10);

  std::cout << "First Answer: " << first_star(octopodes, 100) << std::endl;
  std::cout << "Second Answer: " << second_star(octopodes) << std::endl;

}
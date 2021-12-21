#include "../../common/cpp/load_data.h"
#include "chiton_navigator.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>



namespace
{

  int first_star(ChitonNavigator<short, 100, 100>& navigator)
  {
    return navigator.navigate(0, (100*100) - 1);
  }

  long long second_star(ChitonNavigator<short, 500, 500>& navigator)
  {
    return navigator.navigate(0, (500*500) - 1);
  }
}

int main()
{
  auto lines = load_data::load_file_single_char("../data");

  std::vector<std::vector<short>> chiton_density;
  chiton_density.reserve(lines.size());

  for (const auto& line : lines)
    chiton_density.emplace_back(load_data::parse_string<short>(line));

  std::vector<std::vector<short>> extended_chiton_density;
  extended_chiton_density.reserve(500);

  for (const auto& chiton_line : chiton_density)
  {
    extended_chiton_density.emplace_back();
    extended_chiton_density.back().reserve(500);
    for (size_t i = 0; i < 5; ++i)
    {
      for (const auto& v : chiton_line)
      {
        const short new_v = 1 + (((v-1) + i) % 9);
        extended_chiton_density.back().emplace_back(new_v);
      }
    }
  }

  for (size_t i = 1; i < 5; ++i)
  {
    for (size_t j = 0; j < 100; ++j)
    {
      extended_chiton_density.emplace_back();
      extended_chiton_density.back().reserve(500);
      for (const auto& v : extended_chiton_density[j])
      {
        const short new_v = 1 + (((v-1) + i) % 9);
        extended_chiton_density.back().emplace_back(new_v);
      }
    }
  }

  ChitonNavigator<short, 100, 100> navigator(chiton_density);
  ChitonNavigator<short, 500, 500> extended_navigator(extended_chiton_density);

  std::cout << "First Answer: " << first_star(navigator) << std::endl;
  std::cout << "Second Answer: " << second_star(extended_navigator) << std::endl;

}
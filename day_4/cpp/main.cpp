#include "../../common/cpp/load_data.h"

#include "bingo.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

namespace
{
  int first_star(const std::vector<std::vector<int>>& values)
  {
    return 0;
  }

  int second_star(const std::vector<std::vector<int>>& values)
  {
    return 0;
  }
}

int main()
{
  auto moves_raw = load_data::load_file("../data", ',', 0, 1);
  std::vector<int> moves = load_data::parse_string<int>(moves_raw.front());
  for (const auto& m : moves)
    std::cout << m << ", ";

  auto grids_raw = load_data::load_file_by_blocks("../data", ' ', 1, std::numeric_limits<size_t>::max());

  Bingo<int, 5, 5> bingo(std::vector<std::vector<int>>{1});

  bingo.print_indexes();

  // std::cout << "First Answer: " << first_star(values) << std::endl;
  // std::cout << "Second Answer: " << second_star(values) << std::endl;

}
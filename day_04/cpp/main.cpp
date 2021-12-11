#include "../../common/cpp/load_data.h"

#include "bingo.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

int main()
{
  auto moves_raw = load_data::load_file("../data", ',', 0, 1);
  std::vector<int> moves = load_data::parse_string<int>(moves_raw.front());

  auto grids_raw = load_data::load_file_by_blocks("../data", ' ', 1, std::numeric_limits<size_t>::max());
  std::vector<Bingo<int, 5, 5>> bingos;
  for (const auto& raw_grid : grids_raw)
  {
    std::vector<std::vector<int>> g;
    for (const auto& raw_line : raw_grid)
      g.emplace_back(load_data::parse_string<int>(raw_line));
    bingos.emplace_back(std::move(g));
  }

  int first_score = 0;
  int last_score = 0;
  bool winner_found = false;
  for (const auto& move : moves)
  {
    for (auto& bingo : bingos)
    {
      if(bingo.draw_number(move))
      {
        if (!winner_found)
          first_score = bingo.get_score();
        last_score = bingo.get_score();
        winner_found = true;
      }
    }
  }

  std::cout << "First Answer: " << first_score << std::endl;
  std::cout << "Second Answer: " << last_score << std::endl;

}
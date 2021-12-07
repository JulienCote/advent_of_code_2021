#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <regex>

#include <algorithm>

namespace
{
  // fun bruteforce with many flavours of <algorithm>
  size_t first_star(std::vector<int> fishes, size_t day_count)
  {
    while (day_count--)
    {
      std::for_each(fishes.begin(), fishes.end(), [](auto& f){ f -= 1; });
      const size_t births = std::count_if(fishes.cbegin(), fishes.cend(), [](const auto& f){ return f == -1; });
      std::replace_if(fishes.begin(), fishes.end(), [](const auto& f){ return f == -1; }, 6);
      std::generate_n(std::back_inserter(fishes), births, [](){ return 8; });
    }
    return fishes.size();
  }

  // actually do something clever -> aggregate fishes and batch-process them
  size_t second_star(std::vector<int> fishes, size_t day_count)
  {
    constexpr size_t baby_start_at = 8;
    constexpr size_t post_birth_at = 6;
    std::array<long, 9> fish_counts{0,0,0,0,0,0,0,0,0};
    for (const auto& f : fishes)
      ++fish_counts[f];

    while (day_count--)
    {
      const auto births = fish_counts[0];
      for (size_t i = 0; i < fish_counts.size() - 1; ++i)
        fish_counts[i] = fish_counts[i+1];

      fish_counts[baby_start_at] = births;
      fish_counts[post_birth_at] += births;
    }

    return std::accumulate(fish_counts.begin(), fish_counts.end(), 0L);
  }
}

int main()
{
  auto file = load_data::load_file("../data", ',', 0, std::numeric_limits<size_t>::max());
  std::vector<int> fishes = load_data::parse_string<int>(file[0]);

  std::cout << "First Answer: " << first_star(fishes, 80) << std::endl;
  std::cout << "Second Answer: " << second_star(fishes, 256) << std::endl;

}
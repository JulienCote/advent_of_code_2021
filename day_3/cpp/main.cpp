#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

namespace
{
  enum class ExtractionType { OXYGEN, SCRUBBER };

  std::vector<int> sum_bits(const std::vector<std::vector<int>>& values)
  {
    std::vector<int> bit_sums;
    bit_sums.resize(values.front().size());
    for (const auto& v : values)
    {
      for (size_t i = 0; i < v.size(); ++i)
        bit_sums[i] += v[i];
    }
    return bit_sums;
  }

  int sum_bits(const std::vector<std::vector<int>>& values, size_t idx)
  {
    int bit_sum = 0;
    for (const auto& v : values)
      bit_sum += v[idx];
    return bit_sum;
  }

  int first_star(const std::vector<std::vector<int>>& values)
  {
    int gamma = 0;
    int epsilon = 0;

    const int majority_at = values.size() / 2;
    int bit_offset = values.front().size() - 1;

    std::vector<int> bit_sums = sum_bits(values);
    for (const auto s : bit_sums)
    {
      if (s > majority_at)
        gamma += 1 << bit_offset;
      else
        epsilon += 1 << bit_offset;
      --bit_offset;
    }

    return gamma * epsilon;
  }

  int decode(std::vector<std::vector<int>>& values, size_t idx, ExtractionType extraction_type)
  {
    if (values.size() == 0)
      throw std::runtime_error("No values left");

    if (values.size() == 1)
    {
      int value = 0;
      size_t bit_offset = values.front().size() - 1;
      for (size_t i = 0; i < values.front().size(); ++i)
        value += values.front()[i] << bit_offset--;
      return value;
    }

    const int sum = sum_bits(values, idx);
    const double majority_at = values.size() / 2.;
    int erase_value = 0;
    switch(extraction_type)
    {
      case ExtractionType::OXYGEN:
        erase_value = static_cast<int>(sum < majority_at);
        break;
      case ExtractionType::SCRUBBER:
        erase_value = static_cast<int>(sum >= majority_at);
        break;
    }

    std::vector<size_t> erase_idxs;
    for (size_t i = 0; i < values.size(); ++i)
    {
      if (values[i][idx] == erase_value)
        erase_idxs.emplace_back(i);
    }

    for (auto it = erase_idxs.rbegin(); it != erase_idxs.rend(); ++it)
      values.erase(values.begin() + *it);

    return decode(values, ++idx, extraction_type);
  }

  int second_star(const std::vector<std::vector<int>>& values)
  {
    //wrap 'decode' in lambda that captures by copy, the recursive functions destroys the data for the second extraction
    const auto extract = [](std::vector<std::vector<int>> values, ExtractionType type){ return decode(values, 0, type ); };
    const int oxygen = extract(values, ExtractionType::OXYGEN);
    const int scrubber = extract(values, ExtractionType::SCRUBBER);

    return oxygen * scrubber;
  }
}

int main()
{
  auto file = load_data::load_file_single_char("../data");
  std::vector<std::vector<int>> values;
  for (const auto& line : file)
    values.emplace_back(load_data::parse_string<int>(line));

  std::cout << "First Answer: " << first_star(values) << std::endl;
  std::cout << "Second Answer: " << second_star(values) << std::endl;

}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

namespace
{
  std::vector<int> load_values_from_path(const std::string& path)
  {
    std::vector<int> out;
    std::ifstream fs(path);
    std::string line;
    while (std::getline(fs, line))
      out.emplace_back(std::atoi(line.c_str()));
    return out;
  }

  template<typename T>
  size_t count_increases(std::vector<T> values)
  {
    size_t increase_count = 0;
    for (size_t i = 1; i < values.size(); ++i)
    {
      if (values[i] > values[i-1])
        ++increase_count;
    }
    return increase_count;
  }

  template<typename T>
  std::vector<T> get_sliding_windows(std::vector<T> values, size_t window_size)
  {
    if (window_size == 0)
      throw std::domain_error("window_size must be greater than 0");

    std::vector<T> out;
    out.reserve(values.size() - (window_size - 1));
    for (size_t i = 0; i < values.size() - (window_size - 1); ++i)
    {
      T val = 0;
      for (size_t j = 0; j < window_size; ++j)
        val += values[i+j];
      out.emplace_back(val);
    }
    return out;
  }
}

int main()
{
  auto values = load_values_from_path("../data");

  std::cout << "Part 1: Number of time the measurement increases"
            << std::endl
            << count_increases(values)
            << std::endl;

  std::cout << std::endl;

  auto windows = get_sliding_windows(values, 3);
  std::cout << "Part 2: Number of time the measurement increases while considering a sliding windows of 3 measurements"
          << std::endl
          << count_increases(windows)
          << std::endl;

  return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>

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
    if (window_size == 0 || window_size > values.size())
      throw std::domain_error("window_size must be greater than 0 and not larger than the number of elements");

    std::vector<T> out;
    out.reserve(values.size() - (window_size - 1));

    size_t begin = 0;
    size_t end = window_size;
    T current_window = std::accumulate(values.begin(), values.begin() + (end - 1), static_cast<T>(0));
    out.emplace_back(current_window);

    for (size_t i = 1; i < values.size() - (window_size - 1); ++i)
    {
      current_window -= values[begin];
      current_window += values[end];
      out.emplace_back(current_window);
      ++begin;
      ++end;
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
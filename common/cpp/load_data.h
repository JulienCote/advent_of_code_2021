#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <type_traits>

#include <cstdlib>

namespace load_data
{

  std::vector<std::string> load_file(const std::string& path)
  {
    std::vector<std::string> out;
    std::ifstream fs(path);
    std::string line;
    while (std::getline(fs, line))
      out.emplace_back(std::move(line));
    return out;
  }

  // This functions flips the values to become columnar. "a,1\nb,2" becomes {{"a","b"},{"1","2"}}
  // This assumes a fixed number of columns accross rows
  std::vector<std::vector<std::string>> load_file(const std::string& path, char field_delimiter, size_t field_count)
  {
    std::vector<std::vector<std::string>> out;
    out.resize(field_count);
    std::ifstream fs(path);
    std::string line;
    while (std::getline(fs, line, '\n'))
    {
      std::stringstream ss(std::move(line));
      std::string field;
      size_t at_field = 0;
      while (std::getline(ss, field, field_delimiter))
        out[at_field++].emplace_back(std::move(field));
    }
    return out;
  }

  template<typename T>
  std::vector<T> parse_string(const std::vector<std::string>& strs)
  {
    std::vector<T> out;
    for (const auto& str : strs)
    {
      if constexpr (std::is_integral_v<T>)
        out.emplace_back(std::atoi(str.c_str()));
      else if (std::is_floating_point_v<T>)
        out.emplace_back(std::stod(str));
    }
    return out;
  }
}
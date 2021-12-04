#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <type_traits>

#include <cstdlib>
#include <cstring>

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
  // This assumes a fixed number of columns across rows
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

  //similar to load_file(std::string, char, size_t) but each char is a field, instead of using a delim
  std::vector<std::vector<char>> load_file_single_char(const std::string& path)
  {
    std::vector<std::vector<char>> out;
    std::ifstream fs(path);
    std::string line;
    while (std::getline(fs, line))
    {
      out.emplace_back();
      for (size_t i = 0; i < line.length(); ++i)
        out.back().emplace_back(line[i]);
    }
    return out;
  }

  template<typename T_OUT, typename T_IN>
  std::vector<T_OUT> parse_string(const std::vector<T_IN>& strs)
  {
    std::vector<T_OUT> out;
    for (const auto& str : strs)
    {
      if constexpr (std::is_same_v<T_IN, char>)
      {
        out.emplace_back(str != '0');
        continue;
      }
      else
      {
        if constexpr (std::is_same_v<T_OUT, bool>)
          out.emplace_back(!(str == "0" || strcasecmp(str.c_str(), "false")));
        else if(std::is_integral_v<T_OUT>)
          out.emplace_back(std::atoi(str.c_str()));
        else if (std::is_floating_point_v<T_OUT>)
          out.emplace_back(std::stod(str));
      }
    }
    return out;
  }
}
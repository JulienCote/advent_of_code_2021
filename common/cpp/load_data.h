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
  template<typename Stream>
  void skip_first_n(Stream& stream, size_t first_line, char delim)
  {
    while (first_line--) // skip until first line we care about
      stream.ignore(std::numeric_limits<std::streamsize>::max()), delim;
  }

  std::vector<std::string> load_file(const std::string& path)
  {
    std::vector<std::string> out;
    std::ifstream fs(path);
    std::string line;
    while (std::getline(fs, line))
      out.emplace_back(std::move(line));
    return out;
  }

  std::vector<std::vector<std::string>> load_file(const std::string& path, char field_delimiter, size_t first_line, size_t last_line)
  {
    std::vector<std::vector<std::string>> out;
    std::ifstream fs(path);
    skip_first_n(fs, first_line, '\n'); // skip lines until at first_line

    std::string line;
    while (first_line++ < last_line && std::getline(fs, line, '\n')) // until last_line or out of lines in file
    {
      out.emplace_back();
      std::stringstream ss(std::move(line));
      std::string field;
      while (std::getline(ss, field, field_delimiter))
        out.back().emplace_back(std::move(field));
    }
    return out;
  }

  // This functions flips the values to become columnar. "a,1\nb,2" becomes {{"a","b"},{"1","2"}}
  std::vector<std::vector<std::string>> load_file_csv(const std::string& path, char field_delimiter)
  {
    std::vector<std::vector<std::string>> out;
    std::ifstream fs(path);
    std::string line;
    while (std::getline(fs, line, '\n'))
    {
      std::stringstream ss(std::move(line));
      std::string field;
      size_t at_field = 0;
      while (std::getline(ss, field, field_delimiter))
      {
        if (out.size() == at_field)
          out.emplace_back();
        out[at_field++].emplace_back(std::move(field));
      }
    }
    return out;
  }

  std::vector<std::vector<std::vector<std::string>>> load_file_by_blocks(const std::string& path, char field_delimiter, size_t first_line, size_t last_line)
  {
    std::vector<std::vector<std::vector<std::string>>> out;
    out.emplace_back(); //start with a first block

    std::ifstream fs(path);
    skip_first_n(fs, first_line, '\n'); // skip lines until at first_line

    std::string line;
    while (std::getline(fs, line, '\n'))
    {
      if (line.empty()) //then start new block
        out.emplace_back();
      else // then add new line to last block
        out.back().emplace_back();

      std::stringstream ss(std::move(line));
      std::string field;
      while (std::getline(ss, field, field_delimiter))
        out.back().back().emplace_back(std::move(field));
    }
    return out;
  }

  //similar to load_file_csv(std::string, char) but each char is a field, instead of using a delim. No flip
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
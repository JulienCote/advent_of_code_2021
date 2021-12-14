#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

#include <set>
#include <algorithm>

namespace
{
  template<typename T1, typename T2>
  std::set<std::pair<T1,T2>> get_unique_pairs(const std::vector<T1>& v1, const std::vector<T2>& v2)
  {
    if (v1.size() != v2.size())
      throw std::runtime_error("v1 and v2 must be of same size");

    std::set<std::pair<T1,T2>> pairs;
    for (size_t i = 0; i < v1.size(); ++i)
      pairs.emplace(v1[i], v2[i]);

    return pairs;
  }

  void do_fold(std::vector<int>& xs, std::vector<int>& ys, const std::pair<char,int>& fold)
  {
    if (fold.first != 'x' && fold.first != 'y')
      throw std::runtime_error("Only 'x' or 'y' axis are allowed for folding");

    auto& vref = fold.first == 'x' ? xs : ys;
    for (auto& v : vref)
    {
      const int distance_from_fold = v - fold.second;
      if (distance_from_fold <= 0) //only consider values past the fold
        continue;

      v -= 2 * distance_from_fold;
    }
  }

  int first_star(std::vector<int> xs, std::vector<int> ys, const std::pair<char,int>& fold)
  {
    do_fold(xs, ys, fold);
    return get_unique_pairs(xs, ys).size();
  }

  int second_star(std::vector<int> xs, std::vector<int> ys, const std::vector<std::pair<char,int>>& folds)
  {
    for (const auto& fold : folds)
      do_fold(xs, ys, fold);
    auto unique_pairs = get_unique_pairs(xs, ys);

    int max_x = 0;
    int max_y = 0;
    for (const auto& pair : unique_pairs)
    {
      max_x = std::max<int>(pair.first, max_x);
      max_y = std::max<int>(pair.second, max_y);
    }

    // if max is 5, then there are 6 lines/rows including 0
    max_x += 1;
    max_y += 1;

    std::vector<char> image;
    image.resize(max_x * max_y, '.');
    for (const auto& pair : unique_pairs)
    {
      const size_t i = (pair.second * max_x + pair.first);
      std::cout << pair.first << ", " << pair.second <<  " -> " << i << std::endl;;
      image[i] = '#';
    }

    //TODO: this doesn't display quite well for some reason, something about calculating the 'i' on line 69 I think
    for (size_t i = 0; i < image.size(); ++i)
    {
      std::cout << image[i];
      if (i && 0 == i % max_x)
        std::cout << std::endl;
    }

    std::cout << std::endl;

    return 0;
  }
}
int main()
{
  auto file = load_data::load_file("../data");
  std::vector<int> x;
  std::vector<int> y;
  std::vector<std::pair<char, int>> folds;

  bool parsing_coordinates = true;
  for (const auto& line : file)
  {
    if (line.empty())
    {
      parsing_coordinates = false;
      continue;
    }

    if (parsing_coordinates)
    {
      const auto delim_pos = line.find(',');
      x.emplace_back(std::stoi(std::string(line.begin(), line.begin() + delim_pos)));
      y.emplace_back(std::stoi(std::string(line.begin() + delim_pos + 1, line.end())));
    }
    else
    {
      const auto delim_pos = line.find('=');
      folds.emplace_back(std::make_pair(line[delim_pos - 1], std::stoi(std::string(line.begin() + delim_pos + 1, line.end()))));
    }
  }

  std::cout << "First Answer: " << first_star(x, y, folds.front()) << std::endl;

  second_star(x, y, folds); // this visually prints the answer. requires human-powered interpretation
  // std::cout << "Second Answer: " << second_star(x, y, folds) << std::endl;

}
#include "../../common/cpp/load_data.h"
#include "cave_explorer.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>

namespace
{
  int first_star(const CaveExplorer& caves)
  {
    return caves.crawl_caves(false);
  }

  long second_star(const CaveExplorer& caves)
  {
    return caves.crawl_caves(true);
  }
}

int main()
{
  auto lines = load_data::load_file("../data", '-');

  std::vector<std::pair<std::string, std::string>> connections;
  for (const auto& line : lines)
    connections.emplace_back(std::make_pair(line[0], line[1]));

  CaveExplorer caves(connections, "start", "end");

  std::cout << "First Answer: " << first_star(caves) << std::endl;
  std::cout << "Second Answer: " << second_star(caves) << std::endl;

}
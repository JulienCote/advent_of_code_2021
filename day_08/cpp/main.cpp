#include "../../common/cpp/load_data.h"

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <regex>

#include <unordered_map>

namespace
{

  //TODO: implement this
  int first_star()
  {

  }

  //TODO: implement this
  int second_star()
  {
  }
}

int main()
{
  auto file = load_data::load_file("../data");

  std::cout << "First Answer: " << first_star() << std::endl;
  std::cout << "Second Answer: " << second_star() << std::endl;
}

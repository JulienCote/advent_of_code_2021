#pragma once

#include "../../common/cpp/adjacency_grid.h"

#include <unordered_set>

template <typename T, size_t X, size_t Y, AdjacencyType ...AdjTypes>
class LavaTubes : public AdjacencyGrid<T,X,Y,AdjTypes...>
{
  public:
    LavaTubes() = delete;

    explicit LavaTubes(const std::vector<std::vector<T>>& starting_energy_levels)
      : AdjacencyGrid<T,X,Y,AdjTypes...>(starting_energy_levels)
    {
      static_assert(std::is_integral_v<T>);
    }

    std::vector<T> get_low_points() const
    {
      std::vector<T> values;
      for (size_t i = 0; i < this->size(); ++i)
      {
        bool is_local_minimum = true; //assume true until proven wrong
        const auto& adjacent_cells = this->m_adjacent_cells_per_index[i];
        for (const auto& cell : adjacent_cells)
        {
          if (!this->is_index_valid(cell))
            continue;
          if (this->m_grid[i] >= this->m_grid[cell])
          {
            is_local_minimum = false;
            break;
          }
        }

        if (is_local_minimum)
          values.emplace_back(this->m_grid[i]);
      }
      return values;
    }

    std::vector<long> get_basins(T max_value) const
    {
      std::vector<long> basins;
      std::unordered_set<size_t> visited_cells;
      basins.reserve(this->size());
      visited_cells.reserve(this->size());

      for (size_t i = 0; i < this->size(); ++i)
      {
        if (visited_cells.contains(i))
          continue;
        basins.emplace_back(crawl_basins(visited_cells, i, max_value));
      }
      return basins;
    }

  private:
    long crawl_basins(std::unordered_set<size_t>& visited_cells, size_t cell, T max_value) const
    {
      if (visited_cells.contains(cell) || !this->is_index_valid(cell) || this->m_grid[cell] == max_value)
          return 0;

      visited_cells.emplace(cell);

      long partial_size = 1;
      const auto& adjacent_cells = this->m_adjacent_cells_per_index[cell];
      for (const auto& adjacent_cell : adjacent_cells)
        partial_size += crawl_basins(visited_cells, adjacent_cell, max_value);
      return partial_size;
    }

};
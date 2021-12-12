#pragma once

#include "../../common/cpp/adjacency_grid.h"

#include <array>
#include <vector>
#include <limits>
#include <type_traits>
#include <utility>

template <typename T, size_t X, size_t Y>
class OctopusFlash : public AdjacencyGrid<T,X,Y>
{
  public:
    OctopusFlash() = delete;

    explicit OctopusFlash(const std::vector<std::vector<T>>& starting_energy_levels, T octopus_activates_at)
      : AdjacencyGrid<T,X,Y>(starting_energy_levels)
      , m_octopus_activates_at(std::move(octopus_activates_at))
    {
      static_assert(std::is_integral_v<T>);
    }

    // returns the number of flashes that happened this turn
    size_t step_forward()
    {
      for (size_t i = 0; i < this->size(); ++i)
      {
        if (++this->m_grid[i] == m_octopus_activates_at)
          activate_octopus(i);
      }

      size_t number_flashes = 0;
      // reset energy for next turn
      for (size_t i = 0; i < this->size(); ++i)
      {
        if (this->m_grid[i] >= m_octopus_activates_at)
        {
          ++number_flashes;
          this->m_grid[i] = 0;
        }
      }

      return number_flashes;
    }

    long get_flash_count() const
    {
      return m_flash_count;
    }

  private:
    const T m_octopus_activates_at;
    long m_flash_count = 0;

  private:
    void activate_octopus(size_t index)
    {
      ++m_flash_count;
      const auto& adjacent_indices = this->m_adjacent_coordinates_per_index[index];
      for (const auto& adjacent_index : adjacent_indices)
      {
        if (!this->is_index_valid(adjacent_index)) // invalid coordinate(ie, the current vallue is at an edge), so skip it
          continue;
        if (++this->m_grid[adjacent_index] == m_octopus_activates_at)
          activate_octopus(adjacent_index);
      }
    }
};
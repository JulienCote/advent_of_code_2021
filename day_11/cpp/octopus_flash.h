#pragma once

#include <array>
#include <vector>
#include <limits>
#include <type_traits>
#include <utility>

template <typename T, size_t X, size_t Y>
class OctopusFlash final
{
  public:
    static constexpr size_t m_octopus_count = X * Y;

  public:
    OctopusFlash() = delete;
    explicit OctopusFlash(const std::vector<std::vector<T>>& starting_energy_levels, T octopus_activates_at)
      : m_octopus_activates_at(std::move(octopus_activates_at))
    {
      static_assert(std::is_integral_v<T>);
      size_t at = 0;
      for (const auto& row : starting_energy_levels)
      {
        for (const auto& energy : row)
          m_energy_levels[at++] = energy;
      }
    }

    // returns the number of flashes that happened this turn
    size_t step_forward()
    {
      for (size_t i = 0; i < m_octopus_count; ++i)
      {
        if (++m_energy_levels[i] == m_octopus_activates_at)
          activate_octopus(i);
      }

      size_t number_flashes = 0;
      // reset energy for next turn
      for (size_t i = 0; i < m_octopus_count; ++i)
      {
        if (m_energy_levels[i] >= m_octopus_activates_at)
        {
          ++number_flashes;
          m_energy_levels[i] = 0;
        }
      }

      return number_flashes;
    }

    long get_flash_count() const
    {
      return m_flash_count;
    }

    static constexpr size_t get_octopus_count()
    {
      return m_octopus_count;
    }

  private:
    const T m_octopus_activates_at;
    std::array<T, m_octopus_count> m_energy_levels;
    long m_flash_count = 0;

  private:
    void activate_octopus(size_t index)
    {
      ++m_flash_count;
      const auto& adjacent_indices = m_adjacent_coordinates_per_index[index];
      for (const auto& adjacent_index : adjacent_indices)
      {
        if (adjacent_index == std::numeric_limits<size_t>::max()) // invalid coordinate(ie, the current vallue is at an edge), so skip it
          continue;
        if (++m_energy_levels[adjacent_index] == m_octopus_activates_at)
          activate_octopus(adjacent_index);
      }
    }

  private:
    // bunch of static constexpr stuff to preload indices of neighboring octopus

    static constexpr size_t get_x_from_idx(size_t idx)
    {
      return idx % X;
    }

    static constexpr size_t get_y_from_idx(size_t idx)
    {
      return idx / X;
    }

    static constexpr std::array<size_t, 8> get_adjacent_coordinates_at(size_t idx)
    {
      std::array<std::pair<short, short>, 8> offsets {{
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}}};

      std::array<size_t, 8> coordinates;
      size_t x = get_x_from_idx(idx);
      size_t y = get_y_from_idx(idx);

      for (size_t i = 0; i < offsets.size(); ++i)
      {
        const short o_x = offsets[i].first;
        const short o_y = offsets[i].second;
        const size_t new_x = x + o_x;
        const size_t new_y = y + o_y;
        if (new_x >= X || new_y >= Y) // no value if any index is beyond the board
          coordinates[i] = std::numeric_limits<size_t>::max();
        else
          coordinates[i] = new_x + (X * new_y);
      }

      return coordinates;
    }

    static constexpr std::array<std::array<size_t, 8>, m_octopus_count> get_all_adjacent_coordinates()
    {
      std::array<std::array<size_t, 8>, m_octopus_count> coordinates;
      for (size_t i = 0; i < m_octopus_count; ++i)
        coordinates[i] = get_adjacent_coordinates_at(i);
      return coordinates;
    }

    // TODO: replace with std::array<std::vector<size_t>, m_octopus_count> when constexpr std::vector support exists in gcc or clang
    static constexpr std::array<std::array<size_t, 8>, m_octopus_count> m_adjacent_coordinates_per_index = get_all_adjacent_coordinates();
};
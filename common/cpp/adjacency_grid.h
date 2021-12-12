// Base class to easily implement 2D M*N grids where you need to refer to a cell's neighbours

#pragma once

#include <array>
#include <vector>
#include <limits>
#include <type_traits>
#include <utility>

template <typename T, size_t X, size_t Y>
class AdjacencyGrid
{
  protected:
    static constexpr size_t m_grid_size = X * Y;

  public:
    AdjacencyGrid() = delete;

    static constexpr size_t size()
    {
      return m_grid_size;
    }

  protected:
    explicit AdjacencyGrid(const std::vector<std::vector<T>>& values)
    {
      size_t at = 0;
      for (const auto& row : values)
      {
        for (const auto& v : row)
          m_grid[at++] = v;
      }
    }

    std::array<T, m_grid_size> m_grid; //1D representation of the grid

    static constexpr bool is_index_valid(size_t index)
    {
      return index < m_grid_size;
    }

    static constexpr size_t get_x_from_index(size_t index)
    {
      return index % X;
    }

    static constexpr size_t get_y_from_index(size_t index)
    {
      return index / X;
    }

    static constexpr std::array<size_t, 8> get_adjacent_coordinates_at(size_t index)
    {
      std::array<std::pair<short, short>, 8> offsets {{
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}}};

      std::array<size_t, 8> coordinates;
      const size_t x = get_x_from_index(index);
      const size_t y = get_y_from_index(index);

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

    static constexpr std::array<std::array<size_t, 8>, m_grid_size> get_all_adjacent_coordinates()
    {
      std::array<std::array<size_t, 8>, m_grid_size> coordinates;
      for (size_t i = 0; i < m_grid_size; ++i)
        coordinates[i] = get_adjacent_coordinates_at(i);
      return coordinates;
    }

    // TODO: replace with std::array<std::vector<size_t>, m_grid_size> when constexpr std::vector support exists in gcc or clang
    static constexpr std::array<std::array<size_t, 8>, m_grid_size> m_adjacent_coordinates_per_index = get_all_adjacent_coordinates();
};
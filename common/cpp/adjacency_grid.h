// Base class to easily implement 2D M*N grids where you need to refer to a cell's neighbours

#pragma once

#include <array>
#include <vector>
#include <limits>
#include <type_traits>
#include <utility>

enum class AdjacencyType
{
  CARDINAL, // up, down, left, right
  ORDINAL,  // diagonals inbetween Cardinal directions
}; // can be combined to represent all 8 directions

// TODO: should probably define long/complex types for readability's sake

template <typename T, size_t X, size_t Y, AdjacencyType ...AdjTypes>
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
      // TODO: Validate that all AdjacencyTypes passed are unique
      static_assert(sizeof...(AdjTypes) <= 2, "Can't have more AdjacencyType than there are types");
      static_assert(sizeof...(AdjTypes) > 0, "Must have at least one AdjacencyType");

      size_t at = 0;
      for (const auto& row : values)
      {
        for (const auto& v : row)
          m_grid[at++] = v;
      }
    }

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

    std::array<T, m_grid_size> m_grid; //1D representation of the grid

  private:
    static constexpr short get_neighbour_size(AdjacencyType adjacencyType)
    {
      if (adjacencyType == AdjacencyType::CARDINAL)
        return 4;
      else if (adjacencyType == AdjacencyType::ORDINAL)
        return 4;
    }

    static constexpr short neighbour_count()
    {
      return (get_neighbour_size(AdjTypes) + ...);
    }

    static constexpr void get_neighbour_offset_by_cardinality(
      const AdjacencyType& type, std::array<std::pair<short, short>,
      neighbour_count()>& offsets,
      size_t& at)
    {
      if (type == AdjacencyType::CARDINAL)
      {
        offsets[at++] = {0, 1};
        offsets[at++] = {1, 0};
        offsets[at++] = {0, -1};
        offsets[at++] = {-1, 0};
      }
      else if (type == AdjacencyType::ORDINAL)
      {
        offsets[at++] = {1, 1};
        offsets[at++] = {1, -1};
        offsets[at++] = {-1, -1};
        offsets[at++] = {-1, 1};
      }
    }

    static constexpr std::array<std::pair<short, short>, neighbour_count()> get_neighbour_offsets()
    {
      const auto adjencyTypes = {AdjTypes...};
      std::array<std::pair<short, short>, neighbour_count()> offsets;
      size_t at = 0;
      for (const auto& type : adjencyTypes)
        get_neighbour_offset_by_cardinality(type, offsets, at);
      return offsets;
    }

    static constexpr std::array<size_t, neighbour_count()> get_adjacent_cells_at(size_t index)
    {
      const std::array<std::pair<short, short>, neighbour_count()> offsets = get_neighbour_offsets();

      std::array<size_t, neighbour_count()> cells;
      const size_t x = get_x_from_index(index);
      const size_t y = get_y_from_index(index);

      for (size_t i = 0; i < offsets.size(); ++i)
      {
        const short o_x = offsets[i].first;
        const short o_y = offsets[i].second;
        const size_t new_x = x + o_x;
        const size_t new_y = y + o_y;
        if (new_x >= X || new_y >= Y) // no value if any index is beyond the board
          cells[i] = std::numeric_limits<size_t>::max();
        else
          cells[i] = new_x + (X * new_y);
      }

      return cells;
    }

    static constexpr std::array<std::array<size_t, neighbour_count()>, m_grid_size> get_all_adjacent_cells()
    {
      std::array<std::array<size_t, neighbour_count()>, m_grid_size> cells;
      for (size_t i = 0; i < m_grid_size; ++i)
        cells[i] = get_adjacent_cells_at(i);
      return cells;
    }

  protected:
    // TODO: replace with std::array<std::vector<size_t>, m_grid_size> when constexpr std::vector support exists in gcc or clang
    static constexpr std::array<std::array<size_t, neighbour_count()>, m_grid_size> m_adjacent_cells_per_index = get_all_adjacent_cells();
};
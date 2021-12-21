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

    static constexpr size_t get_index_from_coordinates(size_t x, size_t y)
    {
      return x + (X * y);
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
      const AdjacencyType& type,
      std::vector<std::pair<short, short>>& offsets)
    {
      if (type == AdjacencyType::CARDINAL)
      {
        offsets.emplace_back(std::make_pair(0, 1));
        offsets.emplace_back(std::make_pair(1, 0));
        offsets.emplace_back(std::make_pair(0, -1));
        offsets.emplace_back(std::make_pair(-1, 0));
      }
      else if (type == AdjacencyType::ORDINAL)
      {
        offsets.emplace_back(std::make_pair(1, 1));
        offsets.emplace_back(std::make_pair(1, -1));
        offsets.emplace_back(std::make_pair(-1, -1));
        offsets.emplace_back(std::make_pair(-1, 1));
      }
    }

    std::vector<std::pair<short, short>> get_neighbour_offsets() const
    {
      const auto adjencyTypes = {AdjTypes...};
      std::vector<std::pair<short, short>> offsets;
      for (const auto& type : adjencyTypes)
        get_neighbour_offset_by_cardinality(type, offsets);
      return offsets;
    }

    std::vector<size_t> get_adjacent_cells_at(size_t index) const
    {
      const std::vector<std::pair<short, short>> offsets = get_neighbour_offsets();

      std::vector<size_t> cells;
      const size_t x = get_x_from_index(index);
      const size_t y = get_y_from_index(index);

      for (size_t i = 0; i < offsets.size(); ++i)
      {
        const short o_x = offsets[i].first;
        const short o_y = offsets[i].second;
        const size_t new_x = x + o_x;
        const size_t new_y = y + o_y;
        if (new_x >= X || new_y >= Y) // no value if any index is beyond the board
          cells.emplace_back(std::numeric_limits<size_t>::max());
        else
          cells.emplace_back(get_index_from_coordinates(new_x, new_y));
      }

      return cells;
    }

    std::vector<std::vector<size_t>> get_all_adjacent_cells() const
    {
      std::vector<std::vector<size_t>> cells;
      for (size_t i = 0; i < m_grid_size; ++i)
        cells.emplace_back(get_adjacent_cells_at(i));
      return cells;
    }

  protected:
    // TODO: replace with std::array<std::vector<size_t>, m_grid_size> when constexpr std::vector support exists in gcc or clang
    std::vector<std::vector<size_t>> m_adjacent_cells_per_index = get_all_adjacent_cells();
};
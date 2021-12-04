#pragma once

#include <iostream>
#include <array>

template<size_t SIZE_X, size_t SIZE_Y>
class BingoBase
{
  protected:
    static constexpr size_t m_x = SIZE_X; // grid size along x-axis
    static constexpr size_t m_y = SIZE_Y; // grid size along y-axis

    static constexpr std::array<std::array<size_t, m_x>, m_y> get_row_indexes()
    {
      std::array<std::array<size_t, m_x>, m_y> rows;
      for (size_t i = 0; i < m_x * m_y; ++i)
      {
        size_t x = i % m_y;
        size_t y = i / m_y;
        rows[x][y] = i;
      }
      return rows;
    }

    static constexpr std::array<std::array<size_t, m_y>, m_x> get_line_indexes()
    {
     std::array<std::array<size_t, m_y>, m_x> lines;
      for (size_t i = 0; i < m_x * m_y; ++i)
      {
        size_t x = i % m_y;
        size_t y = i / m_y;
        lines[y][x] = i;
      }
      return lines;
    }
};

template<typename T, size_t SIZE_X, size_t SIZE_Y>
class Bingo : public BingoBase<SIZE_X, SIZE_Y>
{
  public:
    explicit Bingo(std::vector<std::vector<T>> grid)
    {
      // m_row_indexes = get_row_indexes();
      // m_line_indexes = get_line_indexes();
    }

    void print_indexes()
    {
      for (const auto& r : m_row_indexes)
      {
        for (const auto& v : r)
        {
          std::cout << v << ", " << std::endl;
        }
        std::cout << std::endl;
      }
    }

  private:
    std::vector<T> m_values(); // flat representation of the grid

    static constexpr std::array<std::array<size_t, SIZE_X>, SIZE_Y> m_row_indexes = BingoBase<SIZE_X,SIZE_Y>::get_row_indexes(); // list of indexes that represents a row within m_values
    static constexpr std::array<std::array<size_t, SIZE_Y>, SIZE_X> m_line_indexes = BingoBase<SIZE_X,SIZE_Y>::get_line_indexes(); // list of indexes that represents a line within m_values
};
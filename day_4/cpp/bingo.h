#pragma once

#include <iostream>
#include <array>
#include <unordered_map>

template<typename T, size_t SIZE_X, size_t SIZE_Y>
class Bingo
{
  public:
    explicit Bingo(const std::vector<std::vector<T>>& grid)
    {
      m_row_picked.fill(0);
      m_line_picked.fill(0);
      m_values.reserve(m_x * m_y);
      m_values_index.reserve(m_x * m_y);

      for (size_t i = 0; i < m_x * m_y; ++i)
      {
        size_t x = get_x_from_idx(i);
        size_t y = get_y_from_idx(i);
        const auto& v = grid[y][x];
        m_values.emplace_back(v);
        m_values_index.emplace(v, i); // TODO? If insertion fails then we have an invalid board(repeated number)
      }
    }

    // returns true if the move causes victory
    bool draw_number(const T& v)
    {
      if (has_board_won()) // don't draw numbers on a board that won
        return false;

      // find where the number is
      auto v_it = m_values_index.find(v);
      if (v_it == m_values_index.end())
        return false;

      // set value to 0 and count how many moves per row/line we have done
      size_t idx = v_it->second;
      size_t x = get_x_from_idx(idx);
      size_t y = get_y_from_idx(idx);
      ++m_row_picked[x];
      ++m_line_picked[y];
      m_values[idx] = 0;

      // if one of the affected row has been picked X or Y times, the board has won
      if (m_row_picked[x] == m_x || m_line_picked[y] == m_y)
      {
        win(v);
        return true;
      }

      return false;
    }

    bool has_board_won() const
    {
      return m_has_won;
    }

    T get_score() const
    {
      return m_score;
    }

    void print_values() const
    {
      for (const auto& v : m_values)
      {
        std::cout << v << ", ";
      }
      std::cout << std::endl;
    }

  private:
    size_t get_x_from_idx(size_t idx) const
    {
      return idx % m_x;
    }

    size_t get_y_from_idx(size_t idx) const
    {
      return idx / m_x;
    }

    void win(T last_picked)
    {
      m_score = std::accumulate(m_values.begin(), m_values.end(), static_cast<T>(0));
      m_score *= last_picked;
      m_has_won = true;
    }

  private:
    std::vector<T> m_values; // flat representation of the grid
    std::unordered_map<T, size_t> m_values_index; // track where in the vector each value is

    bool m_has_won = false;
    T m_score = 0;

  private:
    static constexpr size_t m_x = SIZE_X; // grid size along x-axis
    static constexpr size_t m_y = SIZE_Y; // grid size along y-axis
    std::array<T, m_x> m_row_picked;  // sums per row
    std::array<T, m_y> m_line_picked; // sums per line
};

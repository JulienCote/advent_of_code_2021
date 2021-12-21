#pragma once

#include "../../common/cpp/adjacency_grid_heap.h"

#include <unordered_map>

template <typename T, size_t X, size_t Y>
class ChitonNavigator final : public AdjacencyGrid<T,X,Y,AdjacencyType::CARDINAL>
{
  public:
    explicit ChitonNavigator(const std::vector<std::vector<T>>& chiton_density)
      : AdjacencyGrid<T,X,Y,AdjacencyType::CARDINAL>(chiton_density)
    {
      static_assert(std::is_integral_v<T>);
    }

    long navigate(size_t starting_point, size_t finish_point) const
    {
      std::unordered_map<size_t, long> risk_from_start;
      std::unordered_map<size_t, long> risk_to_finish;
      risk_from_start.reserve(this->size());
      risk_to_finish.reserve(this->size());
      for (size_t i = 0; i < this->m_grid_size; ++i)
        risk_from_start[i] = std::numeric_limits<long>::max();

      auto best = backtrack_risk(starting_point, finish_point, risk_from_start, risk_to_finish, 0);
      return risk_from_start[finish_point] - this->m_grid[starting_point]; // don't count starting point
    }

  private:
    // start from the end, backtrack to the begining computing at each step the cost to reach the end
    // store the best(lowest) cost of each index
    long backtrack_risk(size_t location,
                        size_t destination,
                        std::unordered_map<size_t, long>& risk_from_start,
                        std::unordered_map<size_t, long>& risk_to_finish,
                        long risk) const
    {
      risk += this->m_grid[location];

      // if we know what's the risk to destination from here, just use that
      const auto riskIt = risk_to_finish.find(location);
      if (riskIt != risk_to_finish.cend())
      {
        if (riskIt->second == std::numeric_limits<long>::max())
          return std::numeric_limits<long>::max();
        else
          return risk + riskIt->second;
      }

      // if we visited this cell with a smaller risk on a prior run, this time can't be any better
      if (risk_from_start[location] <= risk)
        return std::numeric_limits<long>::max();
      else
        risk_from_start[location] = risk;

      if (destination == location)
        return risk;

      // explore neighbouring cells, keep track of the best(lowest) risk to the end
      const auto& adjacent_cells = this->m_adjacent_cells_per_index[location];
      long best = std::numeric_limits<long>::max();
      for (const auto& cell : adjacent_cells)
      {
        if (this->is_index_valid(cell))
          best = std::min(best, backtrack_risk(cell, destination, risk_from_start, risk_to_finish, risk));
      }

      //if the best way from here is unusable, let's block this cell
      if (best == std::numeric_limits<long>::max())
      {
        risk_to_finish[location] = std::numeric_limits<long>::max();
        return std::numeric_limits<long>::max();
      }

      // the risk from here to the end is equal to the risk of the whole path minus the risk until here
      risk_to_finish[location] = best - risk;

      return best;
    }
};

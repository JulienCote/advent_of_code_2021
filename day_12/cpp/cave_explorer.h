#pragma once

#include "../../common/cpp/graph.h"

class CaveExplorer : public Graph<std::string,void>
{
  public:
    using Key = std::string;
    CaveExplorer() = delete;

    explicit CaveExplorer(const std::vector<std::pair<Key, Key>>& connections,
                          Key entrance,
                          Key exit)
      : Graph<Key, void>(connections)
      , m_entrance(std::move(entrance))
      , m_exit(std::move(exit))
    {
    }

    int crawl_caves(bool can_double_dip_a_single_minor_cave) const
    {
      const auto& entrance = this->m_nodes.at(m_entrance);
      std::unordered_map<Key, size_t /*times visited*/> visited_caves;

      return crawl(entrance, visited_caves, can_double_dip_a_single_minor_cave, false);
    }

  private:
    int crawl(const BaseNode& node, std::unordered_map<Key, size_t>& visits, bool can_double_dip_a_single_minor_cave, bool have_double_dipped) const
    {
      const Key& key = node.key;

      if (key == m_exit)
        return 1; //found the exit

      const size_t visit_count = visits[key];
      if (visit_count > 0)
      {
        if (key == m_entrance)
          return 0; //can't visit the entrance a second time
        if (is_cave_minor(key))
        {
          if (!can_double_dip_a_single_minor_cave)
            return 0; // can't double dip
          if (have_double_dipped)
            return 0; // can't double dip again
          if (visit_count > 1)
            return 0; // can't ever visit a minor cave more than twice
          have_double_dipped = true;
        }
      }

      ++visits[key];

      int paths_found = 0;
      for (const auto& connection : node.connections)
        paths_found += crawl(m_nodes.at(connection), visits, can_double_dip_a_single_minor_cave, have_double_dipped);

      --visits[key]; //as if we had never came for subsequent paths

      return paths_found;
    }

    bool is_cave_minor(const Key& key) const
    {
      for (const auto& c : key)
        if (c > 'z' || c < 'a')
          return false;
      return true;
    }

    const Key m_entrance;
    const Key m_exit;
};
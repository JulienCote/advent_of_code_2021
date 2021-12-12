#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

template <typename Key, typename Value>
class Graph
{
  public:
    Graph() = delete;

    size_t size() const
    {
      return m_nodes.size();
    }

  protected:
    Graph(const std::vector<std::pair<Key, Key>>& connections)
    {
      for (const auto& connection : connections)
      {
        for (const auto& p : {connection.first, connection.second} ) //a bit wasteful, but I like the convenience...
        {
          auto it = m_nodes.find(p);
          if (it == m_nodes.end())
            m_nodes.emplace(p, BaseNode{p, connection});
          else
            it->second.add_connection(connection);
        }
      }
    }

    Graph(const std::vector<std::pair<Key, Key>>& connections, const std::unordered_map<Key, Value>& values)
    {
      for (const auto& connection : connections)
      {
        for (const auto& p : {connection.first, connection.second} ) //a bit wasteful, but I like the convenience...
        {
          auto it = m_nodes.find(p);
          if (it == m_nodes.end())
          {
            const auto& v = values.at(p); // TODO: check if p has a value, otherwise let it default init?
            m_nodes.emplace(p, ValueNode{p, connection, v});
          }
          else
          {
            it->second.add_connection(connection);
          }
        }
      }
    }

    struct BaseNode
    {
      BaseNode (Key k, const std::pair<Key, Key>& connection)
        : key(std::move(k))
      {
        add_connection(connection);
      }

      void add_connection(const std::pair<Key, Key>& connection)
      {
        connections.emplace(connection.first == key ? connection.second : connection.first);
      }

      virtual const Value* const get_value() const { return nullptr; }
      virtual void set_value(const Value* const v) {}

      Key key;
      std::unordered_set<Key> connections;
    };

    struct ValueNode final : public BaseNode
    {
      ValueNode (Key k, const std::pair<Key, Key> connection, Value v)
        : BaseNode(std::move(k), connection)
        , value(std::move(v))
      {}

      const Value* const get_value() const final { return &value; }
      void set_value(const Value* const v) final { value = *v; }

      Value value;
    };

    std::unordered_map<Key, BaseNode> m_nodes;
};
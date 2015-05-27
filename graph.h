#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <cstddef>
#include <map>
#include <set>

typedef size_t NodeId;

/**
 * A folytonosan vannak id-zva a nodok, gyakorlati hasznalathoz kell 
 * meg egy reteg ami egy map-ben tarolja, a kinti es bent id prakat, 
 * de lehet az tul nagy overhead.
 *
 */

template <typename T>
class Graph {
 public:
  virtual int number_of_nodes() = 0;
  virtual int number_of_edges() = 0;
  virtual void add_edge(NodeId s, NodeId d) = 0;
  virtual void add_node(NodeId n) = 0;
  virtual void delete_edge(NodeId s, NodeId d) = 0;
  virtual T get_node_value(NodeId n) = 0;
  virtual void set_node_value(NodeId n, T value) = 0;
  virtual T get_edge_value(NodeId s, NodeId d) = 0;
  virtual void set_edge_value(NodeId s, NodeId d, T value) = 0;
  virtual bool node_in_graph(NodeId n) = 0;
  virtual bool edge_in_graph(NodeId n1, NodeId n2) = 0;
  /*
   * lehet valami neigbour iterator jobb lenne
   **/
  virtual std::vector<NodeId> get_neigbours(NodeId n) = 0;
  void depth_first(NodeId start);
  void breadth_first(NodeId start);
 private:
  Graph();
  Graph(int verteces);
  NodeId find_other_part(std::set<NodeId> &already_seen);
  template<typename STORAGE>
  void  search(NodeId start );
  std::vector<T> node_values_;
  std::map<std::pair<NodeId, NodeId>,T> edge_values_;
};

#endif

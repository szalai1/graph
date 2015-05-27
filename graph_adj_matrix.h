#include "graph.h"

template<typename T>
class GraphAdjecencyMatrix : public Graph {
 public:
  GraphAdjecencyMatrix(int number_of_nodes);
  int number_of_nodes();
  int number_of_edges();
  void add_edge(NodeId s, NodeId d);
  void add_node(NodeId n);
  void delete_edge(NodeId s, NodeId d);
  T get_node_value(NodeId n);
  void set_node_value(NodeId n, T value);
  T get_edge_value(NodeId s, NodeId d);
  void set_edge_value(NodeId s, NodeId d, T value);
  bool node_in_graph(NodeId n);
  bool edge_in_graph(NodeId n1, NodeId n2);
  std::vector<NodeId> get_neigbours(NodeId n);
 private:
  std::vector<std::vector<NodeId>> adjecency_matrix_;
};

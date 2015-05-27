#include "graph_adj_matrix.h"

int GraphAdjecencyMatrix::number_of_nodes() {
  return adjecency_matrix_.size();
}

int GraphAdjecencyMatrix::number_of_nodes() {
  int ret_val = 0;
  for (auto i : adjecency_matrix_) {
    for ( auto j : i ) {
      ret_val += j;
    }
  }
}

void GraphAdjecencyMatrix::add_node(NodeId node_id) {
  if ( out_to_in_.find(node_id) == out_to_in_.end() ) {
    out_to_in_[node_id] = size_;
    ++size_;
    in_to_out_.push_back(node_id);
    for ( i : adjecency_matrix_) {
      i.push_back(0);
    }
    adjecency_matrix_.push_back(std::vector<NodeId>{size_, 0});
    node_value_.push_back(T{});
  }
}

void GraphAdjecencyMatrix::add_edge(NodeId s, NodeId d) {
  if ( not node_in_graph(s)) {
    add_node(s);
  }
  if ( not node_in_graph(d) ) {
    add_node(d);
  }
  NodeId s_in = out_to_in_[s];
  NodeId d_in = out_to_in_[d];
  if ( ++adjecency_matrix_[s_in][d_in] ) {
    //    edge_values_[std::pair<auto, auto>{}]
  }
  
}

void GraphAdjecencyMatrix::delete_edge(NodeId s, NodeId d) {
  if ( not node_in_graph(s) or not node_in_graph(d)) {
    return;
  }
  if (adjecency_matrix_[s_in][d_in] != 0 ) {
    --adjecency_matrix_[s_in][d_in];
  }
}

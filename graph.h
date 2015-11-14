#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <cstddef>
#include <map>
#include <set>
#include <functional>

typedef size_t NodeId;

/**
 * Szomszedossagi matrix-szal megvalositott graph osztaly.
 * Akkor hatekony, ha az id-kat 0-tol folytonsan hasznaljuk.
 */
template<typename NODE_T, typename EDGE_T>
class GraphAdj {
 public:
 GraphAdj(size_t max_node) :adj_matrix_{max_node, std::vector<bool>{max_node, false}},
    used_node_{max_node, false}
    {}
  void add_node(NodeId, NODE_T &);
  void add_edge(NodeId, NodeId,  EDGE_T &);
  void delete_edge(NodeId, NodeId);
  void delete_node(NodeId);
  void depth_first(std::function<void(NodeId, NODE_T)> lambda);
  void breadth_first(std::function<void(NodeId, NODE_T)> lambda);
  bool node_exists(NodeId) const;
  bool edge_exists(NodeId, NodeId) const;
  class node_iterator{
  };
  class edge_iterator{
  };
 private:
  void expand(size_t max);
  std::vector<bool> used_node_;
  std::vector<std::vector<bool>> adj_matrix_;
  std::map<NodeId, NODE_T> node_data_;
  std::map<std::pair<NodeId, NodeId>, EDGE_T> edge_data_;
};

template<typename NODE_T, typename EDGE_T>
bool GraphAdj<NODE_T, EDGE_T>::node_exists(NodeId id) const {
  return used_node_.size() > id && used_node_[id];
}

template<typename NODE_T, typename EDGE_T>
bool GraphAdj<NODE_T, EDGE_T>::edge_exists(NodeId from, NodeId to) const {
  return node_exists(from) && node_exists(to) && adj_matrix_[from][to];
}

template<typename NODE_T, typename EDGE_T>
void GraphAdj<NODE_T, EDGE_T>::expand(size_t max) {
  if ( max < used_node_.size() ) {
    return;
  }
  used_node_.resize(max);
  adj_matrix_.resize(max);
  for ( size_t ii = 0; ii < max; ++ii) {
    adj_matrix_[ii].resize(max);
  }
}

template<typename NODE_T, typename EDGE_T>
void GraphAdj<NODE_T, EDGE_T>::add_node(NodeId id, NODE_T &t) {
  if ( id > used_node_.size() ) {
    used_node_.resize(id);
    adj_matrix_.resize(id);
    for ( int ii = 0; ii < id; ++ii) {
      adj_matrix_[ii].resize(id);
    }
    node_data_[id] = t;
  }
  else if ( used_node_[id] == false ) {
    used_node_[id] = true;
    node_data_[id] = t;
  }
  else {
    node_data_[id] = t;
  }
}

template<typename NODE_T, typename EDGE_T>
void GraphAdj<NODE_T, EDGE_T>::add_edge(NodeId from, NodeId to, EDGE_T &t) {
  expand(from > to ? from : to);
  used_node_[from] = true;
  used_node_[to] = true;
  adj_matrix_[from][to] = true;
  edge_data_ = t;
}

template<typename NODE_T, typename EDGE_T>
void GraphAdj<NODE_T, EDGE_T>::delete_edge(NodeId from, NodeId to) {
  if (edge_exists(from, to)) {
    adj_matrix_[from][to] = false;
    edge_data_.erase(std::make_pair(from, to));
  }
}


template<typename NODE_T, typename EDGE_T>
void GraphAdj<NODE_T, EDGE_T>::delete_node(NodeId id) {
  if (node_exists(id)) {
    used_node_[id] = false;
    node_data_.erase(id);
    for ( size_t ii = 0; ii < used_node_.size(); ++ii) {
      if (adj_matrix_[id][ii]) {
        delete_edge(id, ii);
      }
      if (adj_matrix_[ii][id]) {
        delete_edge(id, ii);
      }
    }
  }
}


#endif

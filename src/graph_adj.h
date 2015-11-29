#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <cstddef>
#include <map>
#include <set>
#include <functional>
#include <cstdlib>
#include <queue>

typedef size_t NodeId;
/**
 * Szomszedossagi matrix-szal megvalositott graph osztaly.
 * Akkor hatekony, ha az id-kat 0-tol folytonsan hasznaljuk.
 * NODE_T es EDGE_T -nek legyen default construktora
 */
template<typename NODE_T, typename EDGE_T>
class GraphAdj {
 public:
  GraphAdj(size_t max_node) {
    number_of_nodes_ = 0;
    number_of_edges_ = 0;
    expand(max_node);
  }
  NODE_T& get_node(NodeId);
  EDGE_T& get_edge(NodeId, NodeId);
  void add_node(NodeId, const NODE_T &);
  void add_edge(NodeId, NodeId, const EDGE_T &);
  void delete_edge(NodeId, NodeId);
  void delete_node(NodeId);
  bool node_exists(NodeId) const;
  bool edge_exists(NodeId, NodeId) const;
  /**
   * Iterator az elekhez
   */
  typename std::map<std::pair<NodeId, NodeId>, EDGE_T>::iterator edge_begin();
  typename std::map<std::pair<NodeId, NodeId>, EDGE_T>::iterator edge_end();
  /**
   * Iteratpor a csucsokhoz
   */
  typename std::map<NodeId, NODE_T>::iterator node_begin();
  typename std::map<NodeId, NODE_T>::iterator node_end();
  std::vector<NodeId> from(NodeId);
  std::vector<NodeId> to(NodeId);
  std::set<NodeId> get_nodes();
  void call(NodeId id, std::function<void(NODE_T&,NodeId&)> f) {
    if (node_exists(id)) {
      f(node_data_[id], id);
    }
  }
  size_t number_of_nodes() { return number_of_nodes_;}
  size_t number_of_edges() { return number_of_edges_;}
  
 private:
  size_t number_of_nodes_;
  size_t number_of_edges_;
  void expand(size_t max);
  std::vector<bool> used_node_;
  std::vector<std::vector<bool>> adj_matrix_;
  std::map<NodeId, NODE_T> node_data_;
  std::map<std::pair<NodeId, NodeId>, EDGE_T> edge_data_;
};

template<typename NODE_T, typename EDGE_T>
typename std::map<std::pair<NodeId, NodeId>,
                  EDGE_T>::iterator GraphAdj<NODE_T, EDGE_T>::edge_begin() {
    return edge_data_.begin();
}

template<typename NODE_T, typename EDGE_T>
typename std::map<std::pair<NodeId, NodeId>,
                  EDGE_T>::iterator GraphAdj<NODE_T, EDGE_T>::edge_end() {
    return edge_data_.end();
}

template<typename NODE_T, typename EDGE_T>
typename std::map<NodeId, NODE_T>::iterator GraphAdj<NODE_T, EDGE_T>::node_begin() {
  return node_data_.begin();
}

template<typename NODE_T, typename EDGE_T>
typename std::map<NodeId, NODE_T>::iterator GraphAdj<NODE_T, EDGE_T>::node_end() {
  return node_data_.end();
}

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
void GraphAdj<NODE_T, EDGE_T>::add_node(NodeId id, const NODE_T &t) {
  expand(id+1);
  if (!used_node_[id]) {
    used_node_[id] = true;
    node_data_[id] = t;
    number_of_nodes_+=1;
  }
  else {
    node_data_[id] = t;
  }
}

template<typename NODE_T, typename EDGE_T>
void GraphAdj<NODE_T, EDGE_T>::add_edge(NodeId from, NodeId to, const EDGE_T &t) {
  if ( edge_exists(from, to)) {
    edge_data_[std::make_pair(from, to)] = t;
    return;
  }
  if (!node_exists(from)) {
    add_node(from, NODE_T{});
  }
  if (!node_exists(to)) {
    add_node(to, NODE_T{});
  }
  expand(from > to ? from+1 : to+1);
  adj_matrix_[from][to] = true;
  edge_data_[std::make_pair(from, to)] = t;
  number_of_edges_+=1;
}

template<typename NODE_T, typename EDGE_T>
void GraphAdj<NODE_T, EDGE_T>::delete_edge(NodeId from, NodeId to) {
  if (edge_exists(from, to)) {
    adj_matrix_[from][to] = false;
    edge_data_.erase(std::make_pair(from, to));
    number_of_edges_-=1;
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
    number_of_nodes_-=1;
  }
}

template<typename NODE_T, typename EDGE_T>
NODE_T& GraphAdj<NODE_T, EDGE_T>::get_node(NodeId id) {
  if ( node_exists(id)) {
    return node_data_[id];
  }
  else {
    throw std::out_of_range("GraphAdj::get_node");
  }
}

template<typename NODE_T, typename EDGE_T>
EDGE_T& GraphAdj<NODE_T, EDGE_T>::get_edge(NodeId from, NodeId to) {
  if ( edge_exists(from, to)) {
    return edge_data_[std::make_pair(from, to)];
  }
  else {
    throw std::out_of_range("GraphAdj::get_edge");
  }
}

template<typename NODE_T, typename EDGE_T>
std::vector<NodeId> GraphAdj<NODE_T, EDGE_T>::from(NodeId id) {
  std::vector<NodeId> from_{};
  for (NodeId ii = 0; ii < adj_matrix_.size(); ++ii) {
    if (adj_matrix_[id][ii]) {
      from_.push_back(ii);
    }
  }
  return from_;
}

template<typename NODE_T, typename EDGE_T>
std::vector<NodeId> GraphAdj<NODE_T, EDGE_T>::to(NodeId id) {
  std::vector<NodeId> to{};
  for (NodeId ii = 0; ii < adj_matrix_.size(); ++ii) {
    if (adj_matrix_[ii][id]) {
      to.push_back(ii);
    }
  }
  return to;
}

template<typename NODE_T, typename EDGE_T>
std::set<NodeId> GraphAdj<NODE_T, EDGE_T>::get_nodes() {
  std::set<NodeId> nodes;
  for (NodeId ii = 0; ii < used_node_.size();++ii) {
    if (used_node_[ii]) {
      nodes.insert(ii);
    }
  }
  return nodes;
}

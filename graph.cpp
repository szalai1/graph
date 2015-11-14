#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <cstddef>
#include <map>
#include <set>
#include <functional>
#include <cstdlib>
typedef size_t NodeId;
/**
 * Szomszedossagi matrix-szal megvalositott graph osztaly.
 * Akkor hatekony, ha az id-kat 0-tol folytonsan hasznaljuk.
 */
template<typename NODE_T, typename EDGE_T>
class GraphAdj {
 public:
  GraphAdj(size_t max_node) {
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
 private:
  void expand(size_t max);
  std::vector<bool> used_node_;
  std::vector<std::vector<bool>> adj_matrix_;
  std::map<NodeId, NODE_T> node_data_;
  std::map<std::pair<NodeId, NodeId>, EDGE_T> edge_data_;
};

template<typename NODE_T, typename EDGE_T>
typename std::map<std::pair<NodeId, NodeId>, EDGE_T>::iterator GraphAdj<NODE_T, EDGE_T>::edge_begin() {
    return edge_data_.begin();
}

template<typename NODE_T, typename EDGE_T>
typename std::map<std::pair<NodeId, NodeId>, EDGE_T>::iterator GraphAdj<NODE_T, EDGE_T>::edge_end() {
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
void GraphAdj<NODE_T, EDGE_T>::add_edge(NodeId from, NodeId to, const EDGE_T &t) {
  if (!node_exists(from)) {
    add_node(from, NODE_T{});
  }
  if (!node_exists(to)) {
    add_node(to, NODE_T{});
  }
  expand(from > to ? from : to);
  used_node_[from] = true;
  used_node_[to] = true;
  adj_matrix_[from][to] = true;
  edge_data_[std::make_pair(from, to)] = t;
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

int main() {
  GraphAdj<int,int> G{10};
  for (int ii = 0; ii < 5; ++ii) {
    for ( int jj = 0; jj < 4; ++jj) {
        G.add_edge(ii*2, jj*2+1, ii*10 + jj);
    }
  }
  for (auto it = G.node_begin(); it != G.node_end(); ++it) {
    std::cout << "==== " << it->first <<  " ===="<< std::endl;
    auto to = G.from(it->first);
    for (auto ii : to) {
      std::cout << " " << ii << std::endl;
    }
  }

}


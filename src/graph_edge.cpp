#include <algorithm>
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
 * Ellistaval
 * Akkor hatekony, ha az id-kat 0-tol folytonsan hasznaljuk.
 * NODE_T es EDGE_T -nek legyen default construktora
 **/
template<typename NODE_T, typename EDGE_T>
class GraphEdgeList {
 public:
  GraphEdgeList(size_t max_node) : number_of_nodes_{0}, number_of_edges_{0} {  }
  NODE_T& get_node(NodeId);
  EDGE_T& get_edge(NodeId, NodeId);
  void add_node(NodeId, const NODE_T &);
  void add_edge(NodeId, NodeId, const EDGE_T &);
  void delete_edge(NodeId, NodeId);
  void delete_node(NodeId);
  bool node_exists(NodeId) const;
  bool edge_exists(NodeId, NodeId);
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
  void call(NodeId id, std::function<void(NODE_T&, NodeId&)> f) {
    if (node_exists(id)) {
      f(node_data_[id], id);
    }
  }
  size_t number_of_nodes() { return number_of_nodes_;}
  size_t number_of_edges() { return number_of_edges_;}
 private:
  size_t number_of_nodes_;
  size_t number_of_edges_;
  //void expand(size_t max);
  std::vector<bool> used_node_;
  // i -> {a,b,c}; i bol a-ba b-be c-be van el
  std::map<NodeId, std::vector<NodeId>> edge_list_from_;
  // i -> {a,b,c}; i -be a-bol b-bol c-bol lehet eljutni
  std::map<NodeId, std::vector<NodeId>> edge_list_to_;
  std::map<NodeId, NODE_T> node_data_;
  std::map<std::pair<NodeId, NodeId>, EDGE_T> edge_data_;
};

template<typename NODE_T, typename EDGE_T>
typename std::map<std::pair<NodeId, NodeId>,
                  EDGE_T>::iterator GraphEdgeList<NODE_T, EDGE_T>::edge_begin() {
    return edge_data_.begin();
}

template<typename NODE_T, typename EDGE_T>
typename std::map<std::pair<NodeId, NodeId>,
                  EDGE_T>::iterator GraphEdgeList<NODE_T, EDGE_T>::edge_end() {
    return edge_data_.end();
}

template<typename NODE_T, typename EDGE_T>
typename std::map<NodeId, NODE_T>::iterator GraphEdgeList<NODE_T, EDGE_T>::node_begin() {
  return node_data_.begin();
}

template<typename NODE_T, typename EDGE_T>
typename std::map<NodeId, NODE_T>::iterator GraphEdgeList<NODE_T, EDGE_T>::node_end() {
  return node_data_.end();
}

template<typename NODE_T, typename EDGE_T>
bool GraphEdgeList<NODE_T, EDGE_T>::node_exists(NodeId id) const {
  return edge_list_from_.find(id) != edge_list_from_.end();
}

template<typename NODE_T, typename EDGE_T>
bool GraphEdgeList<NODE_T, EDGE_T>::edge_exists(NodeId from, NodeId to) {
  return node_exists(from) &&
    node_exists(to) &&
    find(edge_list_from_[from].begin(), edge_list_from_[from].end(),to) != edge_list_from_[from].end();
}

template<typename NODE_T, typename EDGE_T>
void GraphEdgeList<NODE_T, EDGE_T>::add_node(NodeId id, const NODE_T &t) {
  if (!node_exists(id)) {
    edge_list_from_[id] = std::vector<NodeId>{};
    edge_list_to_[id] = std::vector<NodeId>{};
    number_of_nodes_+=1;
  }
  node_data_[id] = t;
}

template<typename NODE_T, typename EDGE_T>
void GraphEdgeList<NODE_T, EDGE_T>::add_edge(NodeId from, NodeId to, const EDGE_T &t) {
  if (!node_exists(from)) {
    add_node(from, NODE_T{});
  }
  if (!node_exists(to)) {
    add_node(to, NODE_T{});
  }
  if ( !edge_exists(from, to) ){
    edge_list_from_[from].push_back(to);
    edge_list_to_[to].push_back(from);
    number_of_edges_+=1;
  }
  edge_data_[std::make_pair(from, to)] = t;
}

template<typename NODE_T, typename EDGE_T>
void GraphEdgeList<NODE_T, EDGE_T>::delete_edge(NodeId from, NodeId to) {
  if (edge_exists(from, to)) {
    edge_list_from_.erase(from);
    edge_list_to_.erase(to);
    edge_data_.erase(std::make_pair(from, to));
    number_of_edges_-=1;
  }
}

template<typename NODE_T, typename EDGE_T>
void GraphEdgeList<NODE_T, EDGE_T>::delete_node(NodeId id) {
  if (node_exists(id)) {
    std::vector<NodeId> marked_to_delete{};
    for (auto i : edge_list_from_[id]) {
      marked_to_delete.push_back(i);
    }
    for (auto i : marked_to_delete) {
      delete_edge(id, i);
    }
    node_data_.erase(id);
    number_of_nodes_-=1;
    edge_list_from_.erase(edge_list_from_.find(id));
  }
}

template<typename NODE_T, typename EDGE_T>
NODE_T& GraphEdgeList<NODE_T, EDGE_T>::get_node(NodeId id) {
  if ( node_exists(id)) {
    return node_data_[id];
  }
  else {
    throw std::out_of_range("GraphEdgeList::get_node");
  }
}

template<typename NODE_T, typename EDGE_T>
EDGE_T& GraphEdgeList<NODE_T, EDGE_T>::get_edge(NodeId from, NodeId to) {
  if ( edge_exists(from, to)) {
    return edge_data_[std::make_pair(from, to)];
  }
  else {
    throw std::out_of_range("GraphEdgeList::get_edge");
  }
}

template<typename NODE_T, typename EDGE_T>
std::vector<NodeId> GraphEdgeList<NODE_T, EDGE_T>::from(NodeId id) {
  if ( node_exists(id)) {
    return edge_list_from_[id];
  }
  throw std::out_of_range("GraphEdgeList::from");
}

template<typename NODE_T, typename EDGE_T>
std::vector<NodeId> GraphEdgeList<NODE_T, EDGE_T>::to(NodeId id) {
  if ( node_exists(id)) {
    return edge_list_to_[id];
  }
  throw std::out_of_range("GraphEdgeList::from");
}

template<typename NODE_T, typename EDGE_T>
std::set<NodeId> GraphEdgeList<NODE_T, EDGE_T>::get_nodes() {
  std::set<NodeId> nodes;
  for ( auto it : edge_list_to_) {
    nodes.insert(it.first);
  }
  return nodes;
}

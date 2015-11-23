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
  void call(NodeId id, std::function<void(NODE_T&)> f) {
    if (node_exists(id)) {
      f(node_data_[id]);
    }
  }
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
  expand(id+1);
  if (!used_node_[id]) {
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
  expand(from > to ? from+1 : to+1);
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

template<typename NODE_T, typename EDGE_T>
void depth_first(GraphAdj<NODE_T,EDGE_T> &G,
                 std::stack<NodeId, std::vector<NodeId>> &fifo,
                 std::set<NodeId> &all_node,
                 std::function<void(NODE_T&)> f) {
  if( fifo.empty()) {
    if ( all_node.empty() ) {
      return;
    }
    else {
      fifo.push(*(all_node.begin()));
    }
  }
  auto next_id = fifo.top();
  fifo.pop();
  while(all_node.find(next_id) == all_node.end()) {
    if (fifo.empty()) {
      depth_first(G, fifo, all_node, f);
      return;
    }
    next_id = fifo.top();
    fifo.pop();
  }
  auto neigbors = G.from(next_id);
  //  all_node
  std::cout << "call" << next_id << std::endl;
  G.call(next_id, f);
  for (auto i : neigbors) {
    fifo.push(i);
  }
  all_node.erase(next_id);
  depth_first(G, fifo, all_node, f);
}

// pelda melysegi bejaras az osztaly hasznalatara 
template<typename NODE_T, typename EDGE_T>
void DepthFirst(GraphAdj<NODE_T, EDGE_T> &G, NodeId start, std::function<void(NODE_T&)> f) {
  auto n_vec = G.from(start);
  std::stack<NodeId, std::vector<NodeId>> neigbors(n_vec);
  neigbors.push(start);
  auto all_node = G.get_nodes();
  depth_first(G,
              neigbors,
              all_node, f);
}



int main() {
  GraphAdj<int,int> G{10};
  for (int ii = 0; ii < 5; ++ii) {
    for ( int jj = 5; jj < 10; ++jj) {
      G.add_node(ii, ii);
      G.add_node(jj, jj);
      G.add_edge(ii, jj, ii*100 + jj);
      G.add_edge(jj, ii, ii*100 + jj);
    }
  }
  // G.call(2,[](int &x){std::cout << "labda " << x << std::endl; } );
  DepthFirst<int,int>(G, 1, [](int &x){std::cout << "labda " << x << std::endl; });
  /* for (auto it = G.node_begin(); it != G.node_end(); ++it) {
    std::cout << "==== " << it->first <<  " ===="<< std::endl;
    auto to = G.from(it->first);
    G.call(it->first,[](int &x) {
        x+=42;
        std::cout << "node value : " << x << std::endl;});
        G.call(it->first,[](int &x) {
        x+=42;
        std::cout << "node value : " << x << std::endl;});
    for (auto ii : to) {
      std::cout << " " << ii << std::endl;
    }
    }*/

}


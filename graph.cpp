#include "graph.h"




void do_somethind(NodeId node) {
  std::cout << node << std::endl;
}

template<typename T>
void Graph<T>::depth_first(NodeId start) {
  search<std::queue<NodeId> >(start);
}

template<typename T>
void Graph<T>::breadth_first(NodeId start) {
  search<std::stack<NodeId> >(start);
}
template<typename T>
NodeId Graph<T>::find_other_part(std::set<NodeId> &set) {
  for ( NodeId i = 0;  i < number_of_nodes(); ++i) {
    if ( set.find(i) == set.end() ) {
      return i;
    }
  }
}

template<typename T>
template<typename STORAGE>
void Graph<T>::search(NodeId start) {
  STORAGE store;
  std::set<NodeId> already_seen;
  store.push(start);
  already_seen.insert(start);
  while ( not store.empty() and already_seen.size() == number_of_nodes() ) {
    NodeId next = store.pop();
    auto neigbours = get_neigbours(next);
    already_seen.insert(next);
    for ( auto i : neigbours) {
      store.push(i);
      do_something(i);
    }
    if ( store.empty() ) {
      store.push(find_other_part(already_seen));
    }
  }
  
}


#include "gtest/gtest.h"
#include "../src/graph_adj.cpp"
#include <functional>

//depth_first algoritmus, hogy teszteljem az osztalyokat
template<typename NODE_T, typename EDGE_T>
void depth_first(GraphAdj<NODE_T,EDGE_T> &G,
                 std::stack<NodeId, std::vector<NodeId>> &fifo,
                 std::set<NodeId> &all_node,
                 std::function<void(NODE_T&,NodeId&)> f) {
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
  G.call(next_id, f);
  for (auto i : neigbors) {
    fifo.push(i);
  }
  all_node.erase(next_id);
  depth_first(G, fifo, all_node, f);
}

// pelda melysegi bejaras 
template<typename NODE_T, typename EDGE_T>
void DepthFirst(GraphAdj<NODE_T, EDGE_T> &G,
                NodeId start,
                std::function<void(NODE_T&, NodeId&)> f) {
  auto n_vec = G.from(start);
  std::stack<NodeId, std::vector<NodeId>> neigbors(n_vec);
  neigbors.push(start);
  auto all_node = G.get_nodes();
  depth_first(G,
              neigbors,
              all_node, f);
}


//TEST 1
// 5-5 csucsu teljes paros grafon (oda-vissza el)
// lekerem a csucsokat aztan ellenorozm, hogy get_nodes
// ugyanazt adja e vissza
TEST(GraphAdj, DepthFist) {
  GraphAdj<int,int> G{10};
  for (int ii = 0; ii < 5; ++ii) {
    for ( int jj = 5; jj < 10; ++jj) {
      G.add_edge(ii, jj, ii*100 + jj);
      G.add_edge(jj, ii, ii*100 + jj);
    }
  }
  auto nodes = G.get_nodes();
  std::map<NodeId, int> nodes_test;
  DepthFirst<int,int>(G, 1,
                      [&nodes_test](int &x, NodeId& node_id)
                      { ++nodes_test[node_id]; });
  for ( auto ii : nodes_test) {
    bool in_set = (nodes.find(ii.first) == nodes.end());
    EXPECT_FALSE(in_set);
  }
  for ( auto ii : nodes) {
    auto it  = nodes_test.find(ii);
    bool in_map = (it == nodes_test.end());
    EXPECT_FALSE( in_map);
    EXPECT_EQ(1, it->second);
  }
}

//test2
//ellenorzi a csucsok hozzadasaval/ torleseel kapcsolatos dolgokat
TEST(GraphAdj, Nodes) {
  using Graph = GraphAdj<int, int>;
  Graph G{10};
  EXPECT_EQ(0, G.number_of_nodes());
  for ( int ii = 0; ii < 9; ++ii) {
    G.add_node(ii*10, ii);
    EXPECT_EQ(ii+1, G.number_of_nodes());
  }
  for ( int ii = 0; ii < 3; ++ii) {
    G.delete_node(ii*10);
  }
  EXPECT_EQ(6, G.number_of_nodes());
  size_t num = 0;
  for ( auto it = G.node_begin(); it != G.node_end(); ++it) { 
    ++num;
    EXPECT_EQ(0, (it->first)%10);
  }
  EXPECT_EQ(6, num);
}

//test3
// elekkel kapcsolatos testek
TEST(GraphAdj, Edges) {
  using Graph = GraphAdj<int, int>;
  Graph G{10};
  for ( NodeId id = 1; id < 8; ++id) {
    G.add_edge(0, id, id + 3);
    G.add_edge(id, 0, id + 2);
  }
  EXPECT_EQ(8, G.number_of_nodes());
  G.add_edge(8,2,0);
  EXPECT_EQ(15, G.number_of_edges());
  size_t from_0 = G.from(0).size();
  size_t from_8 = G.from(8).size();
  size_t to_8 = G.to(8).size();
  size_t to_2 = G.to(2).size();
  EXPECT_EQ(7, from_0);
  EXPECT_EQ(1, from_8);
  EXPECT_EQ(0, to_8);
  EXPECT_EQ(2, to_2);
}

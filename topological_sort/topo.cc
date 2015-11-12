#include <iostream>
#include <vector>
#include <unordered_set>
#include <cstdlib>

struct GraphNode {
  int val_;
  std::vector<GraphNode*> predecessors_;
  std::vector<GraphNode*> successors_;
  GraphNode(int val = 0) : val_(val) { }
};

std::vector<GraphNode*> createGraph(int idx = 0) {
  std::vector<GraphNode*> graph;
  GraphNode * gn0 = 0,
            * gn1 = 0,
            * gn2 = 0,
            * gn3 = 0,
            * gn4 = 0,
            * gn5 = 0,
            * gn6 = 0,
            * gn7 = 0,
            * gn8 = 0,
            * gn9 = 0;
  switch (idx) {
    case 0:
      /*          --> 2 -->
       *        /           \
       * 0 --> 1             4 --> 5.
       *        \           /
       *          --> 3 --> 
       */
      graph.push_back(gn0 = new GraphNode(0));
      graph.push_back(gn1 = new GraphNode(1));
      graph.push_back(gn2 = new GraphNode(2));
      graph.push_back(gn3 = new GraphNode(3));
      graph.push_back(gn4 = new GraphNode(4));
      graph.push_back(gn5 = new GraphNode(5));
      gn0->successors_.push_back(gn1);
      gn1->predecessors_.push_back(gn0);
      gn1->successors_.push_back(gn2);
      gn1->successors_.push_back(gn3);
      gn2->predecessors_.push_back(gn1);
      gn2->successors_.push_back(gn4);
      gn3->predecessors_.push_back(gn1);
      gn3->successors_.push_back(gn4);
      gn4->predecessors_.push_back(gn2);
      gn4->predecessors_.push_back(gn3);
      gn4->successors_.push_back(gn5);
      gn5->predecessors_.push_back(gn4);
      break;

    case 1:
      /*          --> 2 -->
       *        /           \
       * 0 --> 1             4 --> 5.
       *        \           /
       *          <-- 3 <-- 
       */
      graph.push_back(gn0 = new GraphNode(0));
      graph.push_back(gn1 = new GraphNode(1));
      graph.push_back(gn2 = new GraphNode(2));
      graph.push_back(gn3 = new GraphNode(3));
      graph.push_back(gn4 = new GraphNode(4));
      graph.push_back(gn5 = new GraphNode(5));
      gn0->successors_.push_back(gn1);
      gn1->predecessors_.push_back(gn0);
      gn1->predecessors_.push_back(gn3);
      gn1->successors_.push_back(gn2);
      gn2->predecessors_.push_back(gn1);
      gn2->successors_.push_back(gn4);
      gn3->predecessors_.push_back(gn4);
      gn3->successors_.push_back(gn1);
      gn4->predecessors_.push_back(gn2);
      gn4->successors_.push_back(gn3);
      gn4->successors_.push_back(gn5);
      gn5->predecessors_.push_back(gn4);
      break;

    default:
      break;

  }
  return graph;
}

void deleteGraph(std::vector<GraphNode*> graph) {
  for (size_t i=0; i<graph.size(); ++i)
    delete graph[i];
}

bool dfs0(std::vector<GraphNode*> & sorted_graph, GraphNode * node, std::unordered_set<GraphNode*> & visited, std::unordered_set<GraphNode*> & pending) {
  if (visited.find(node) == visited.end()) {
    visited.insert(node);
    pending.insert(node);
    for (size_t i=0; i<node->predecessors_.size(); ++i)
      if (! dfs0(sorted_graph, node->predecessors_[i], visited, pending))
        return false;
    sorted_graph.push_back(node);
    pending.erase(node);
    return true;
  } else if (pending.find(node) != pending.end())
    return false;
  else 
    return true;
}

// Return false if cyclic.
// Maintain a stack contains pending nodes, if the nodes' predecessor is visited and pending, then the graph is cyclic.
bool dfs(std::vector<GraphNode*> & sorted_graph, GraphNode * node, std::unordered_set<GraphNode*> & visited, std::unordered_set<GraphNode*> & pending) {
  visited.insert(node);
  pending.insert(node);
  for (size_t i=0; i<node->predecessors_.size(); ++i)
    if (visited.find(node->predecessors_[i]) == visited.end()) {    // Note: The braces are needed.
      if (! dfs(sorted_graph, node->predecessors_[i], visited, pending))
        return false;
    } else if (pending.find(node->predecessors_[i]) != pending.end())
      return false;
  sorted_graph.push_back(node);
  pending.erase(node);
  return true;
}

bool topoSort(std::vector<GraphNode*> & graph, std::vector<GraphNode*> & sorted_graph) {
  std::vector<GraphNode*> node_wo_outgoing;
  for (size_t i=0; i<graph.size(); ++i)
    if (graph[i]->successors_.size() == 0)
      node_wo_outgoing.push_back(graph[i]);

  std::unordered_set<GraphNode*> visited;
  std::unordered_set<GraphNode*> pending;
  for (size_t i=0; i<node_wo_outgoing.size(); ++i)
    if (! dfs0(sorted_graph, node_wo_outgoing[i], visited, pending))
      return false;
  return true;
}

int main(int argc, char * argv[]) {

  if (argc != 2) {
    std::cout << "Please specify the correct arguments." << std::endl;
    return -1;
  }

  int idx = std::atoi(argv[1]);

  std::vector<GraphNode*> g = createGraph(idx);
  std::vector<GraphNode*> sorted_graph;
  if (! topoSort(g, sorted_graph)) {
    std::cout << "Sorry, but the graph may contain a loop!" << std::endl;
    return 0;
  }
  std::cout << "The topological order of Graph " << idx << ':';
  for (size_t i=0; i<sorted_graph.size(); ++i)
    std::cout << ' ' << sorted_graph[i]->val_;
  std::cout << std::endl;

  deleteGraph(g);
  return 0;
}

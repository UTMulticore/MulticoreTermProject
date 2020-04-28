#include "fp_tree.h"

#include <iostream>
#include <string>
#include <queue>

FP_Tree::fp_node* FP_Tree::fp_node::containsItemAsChild(const std::string &item) {
  for (auto& child : children_) {
    if (child->item_ == item)
      return child;
  }
  return nullptr;
}




static std::size_t getTokenEndPosition(const std::string& str, std::size_t position) {
  std::size_t endPos = str.find('/', position);
  return endPos;
}

//  Flow: Get next item in path
//        if no matching item at node, create new node & move to that node
//        if matching item at node, add to count & and move to that node
//        repeat
//  Add in single-linked list
void FP_Tree::add(std::vector<std::string> item_path) {
  if (null_head_ == nullptr)
    null_head_ = new fp_node("NULL HEAD");
  fp_node* iter = null_head_;

  for (int i=0; i<item_path.size() && item_path[i].length() > 2; i++) {
    auto child = iter->containsItemAsChild(item_path[i]);
    if (child) {
      iter = child;
      iter->count_++;
    }
    else {
      fp_node* newChild = new fp_node(item_path[i]);
      iter->children_.push_back(newChild);
      iter = newChild;
    }
  }  
}

void FP_Tree::printNode(fp_node* n) {
  std::cout << "Node->" << n->item_ << " " << n->count_ << "\t";
}

void FP_Tree::printTree() {
  std::queue<fp_node*> q;
  q.push(null_head_);
  int level_count = 1;

  while (!q.empty()) {
    auto current = q.front();
    q.pop();
    printNode(current);
    for (auto& node : current->children_) {
      q.push(node);
    }

    level_count--;
    if (level_count == 0) {
      std::cout << "\n";
      level_count = q.size();
    }

  }
}
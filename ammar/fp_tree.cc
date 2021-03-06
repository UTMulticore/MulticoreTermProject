#include "fp_tree.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

#include "debug.h"



fp_node* fp_node::containsItemAsChild(const std::string &item) {
  for (auto& child : children_) {
    if (child->item_ == item)
      return child;
  }
  return nullptr;
}

bool FP_Tree::isEmpty() {
  return (null_head_ == nullptr || null_head_->children_.size() == 0);
}

bool FP_Tree::hasOnePath() {
  if (isEmpty())
    return true;
  // if every node has one child
  bool oneChild = true;
  fp_node* iter = null_head_;
  while (iter->children_.size() != 0) {
    if (iter->children_.size() > 1) {
      oneChild = false;
      break;
    }
    iter = iter->children_.front();
  }
  return oneChild;
}


void FP_Tree::sortMapByValue() {
    std::vector<std::pair<std::string, std::list<fp_node*>>> mapVector;
    // Insert entries
    for (auto iterator = header_table_.begin(); iterator != header_table_.end(); ++iterator) {
    mapVector.push_back(*iterator);
    }
    //std::sort(mapVector.begin(), mapVector.end(), []());
}

//  Flow: Get next item in path
//        if no matching item at node, create new node & move to that node
//        if matching item at node, add to count & and move to that node
//        repeat
//  Add in single-linked list
void FP_Tree::add(std::vector<std::string> item_path) {
  if (null_head_ == nullptr)
    null_head_ = new fp_node("NULL");
  fp_node* iter = null_head_;

  for (int i=0; i<item_path.size() && item_path[i].length() > 2; i++) {
    auto child = iter->containsItemAsChild(item_path[i]);
    if (child) {
      iter = child;
      iter->count_++;
    }
    else {
      fp_node* newChild = new fp_node(item_path[i]);
      newChild->parent_ = iter;
      iter->children_.push_back(newChild);
      iter = newChild;
      addItemLink(newChild); // add item link

      // SORTT ORDER SOMEHOW
    }
  }

}

void FP_Tree::addItemLink(fp_node* n) {
   header_table_[n->item_].push_back(n);
}

void FP_Tree::printNode(fp_node* n) {
  std::cout << "Node->" << n->item_ << " " << n->count_ << "\t";
}

void FP_Tree::printTree() {
  std::cout << "Tree:\n";
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
  std::cout << "header_table\n";
  for (const auto& p : header_table_) {
    std::cout << p.first << ":size:" << p.second.size() << "\n";
  }
}

std::vector<std::string> getPath(fp_node* iter, int min_supp) {
  std::vector<std::string> ret;
  while (iter->children_.size() != 0) {
    if (iter->count_ >= min_supp)
      ret.push_back(iter->item_);
    iter = iter->children_.front();
  }
  if (iter->count_ >= min_supp)
    ret.push_back(iter->item_);
  return ret;
}

std::vector<std::vector<std::string>> mineTree(FP_Tree& tree, std::uint32_t min_supp) {

  if (tree.isEmpty()){
    std::cout << "EMPTY\n";
    return {};
  }

  // Generate all combinations
  // and make sure support is greater than min support
  if (tree.hasOnePath()) {
    std::cout << "HAS ONE PATH\n";
   // For each combination of nodes in the path P
    // generate pattern with support that is min support
    // and 
    fp_node* iter = tree.null_head_;
    auto ret = getPath(iter->children_.front(), min_supp);
    return {ret};
  }
  else {
    std::cout << "MORE THAN ONE PATH\n";

    std::vector<std::vector<std::string>> ret;

    for (const auto& p : tree.header_table_) {
      std::unordered_map<std::string, int> cond_map;
      std::vector<std::string> conditional;
      conditional.push_back(">" + p.second.front()->item_ + "<");
      for (const auto node : p.second) {
        int count = node->count_;
        fp_node* iter = node;
        iter = iter->parent_;
        // add all parents to map with that count
        while (iter->item_ != "NULL") {
          cond_map[iter->item_] += count;
          iter = iter->parent_;
        }
      }

      // std::cout << "item" << conditional;
      // std::cout << "cond_map: " << cond_map << "\n";
      // So now we have all the conditional map of the pattern
      // lets check for support then just dump everything in the
      // return vec
      for (const auto& p : cond_map) {
        if (p.second >= min_supp){
          conditional.push_back(p.first);
        }
      }
      if (conditional.size() > 1)
        ret.push_back(conditional);
    }

    return ret;
  }
}


std::vector<std::vector<std::string>> FP_Tree::mine(std::uint32_t min_supp) {
  return mineTree(*this, min_supp);
}



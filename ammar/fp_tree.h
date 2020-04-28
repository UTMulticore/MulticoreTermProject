//
//
//
//
// Author: Ammar Sheikh
//

#pragma once

#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>



class FP_Tree {

 public: 
  struct fp_node {
    std::string item_;
    std::size_t count_;
    std::vector<fp_node*> children_;
    fp_node* next_similar_item_;

    fp_node(const std::string item) : item_(item), count_(1){};
    fp_node() = default;
    fp_node& operator=(const fp_node&) = delete;

    fp_node* containsItemAsChild(const std::string& item);
    void addChild(std::string item);
    
    //fp_node* childWithItem(const std::string& child item);
  };

  

  fp_node* null_head_;
  std::unordered_map<std::string, fp_node*> item_lists_;

  



  void add(std::vector<std::string> item_path);
  void printTree();
  void printNode(fp_node* n);
  


};
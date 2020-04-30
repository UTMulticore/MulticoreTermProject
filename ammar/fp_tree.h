//
//
// https://www.cs.sfu.ca/~jpei/publications/sigmod00.pdf
//
// Author: Ammar Sheikh
//

#pragma once

#include <list>
#include <string>
#include <vector>
#include <map>


  struct fp_node {
    std::string item_;
    std::size_t count_;
    std::vector<fp_node*> children_;
    fp_node* parent_;
    fp_node* next_similar_item_;

    fp_node(const std::string item) : item_(item), count_(1), 
                                  next_similar_item_(nullptr), 
                                  parent_(nullptr){};
    fp_node() = default;
    fp_node& operator=(const fp_node&) = delete;

    fp_node* containsItemAsChild(const std::string& item);
    void addChild(std::string item);
    friend class FPGrowth;
  };

class FP_Tree {


 public: 


  friend class FPGrowth;
  fp_node* null_head_;
  std::map<std::string, std::list<fp_node*>> header_table_;

  FP_Tree() : null_head_(nullptr) {}


  void sortMapByValue();
  void printNode(fp_node* n);


  void add(std::vector<std::string> item_path);
  void printTree();
  void addItemLink(fp_node* n);

  void buildTree();
  std::vector<std::vector<std::string>> mine(std::uint32_t min_supp, int num_threads);
  bool isEmpty();
  bool hasOnePath();
  
  void getParentsOfNode(fp_node* p);


};


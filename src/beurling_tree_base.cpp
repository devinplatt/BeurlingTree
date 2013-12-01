/*
 * beurling_tree_base.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: Devin
 *
 *  Defines the BeurlingTreeBase class.
 */

#include "beurling_tree_base.h"
#include <iostream>
#include <string>
using std::ios;
using std::endl;

namespace Platt {

// Initialize static members.
BeurlingTreeBase* BeurlingTreeBase::PcfBase::tree_ptr = nullptr;
vector< vector<unsigned int> > BeurlingTreeBase::PcfBase::triangle
    = vector< vector<unsigned int> >();
unsigned int BeurlingTreeBase::PcfBase::prime_count = 0;
int BeurlingTreeBase::PcfBase::height = -1;
ofstream BeurlingTreeBase::SerialBase::out_file;

void BeurlingTreeBase::InitDefault() {
  // Init root node
  tree.Init(Factorization(0));
  graphviz_node_counter = 0;
}

void BeurlingTreeBase::InitToHeight(unsigned int height) {
  // Init root node
  tree.Init(Factorization(0));
  RecursiveBuild(height, tree.GetRoot());
}

void BeurlingTreeBase::InitFromFile(string filename) {
  ifstream in(filename, ios::in);

  // Extract first factorization and set root to its value.
  string line;
  getline(in, line);
  // line.pop_back() avoided due to MinGW compatibility issues.
  line.erase(line.size()-1);  // line.pop_back();
  tree.Init(line);
  Node<Factorization>* root = tree.GetRoot();

  // Recurse on root
  RecursiveSerialBuild(&in, root);

  in.close();
}

void BeurlingTreeBase::RecursiveSerialBuild(ifstream* in, Node<Factorization>* n) {
  string line;
  getline(*in, line);

  // line.pop_back() and line.back() avoided due to MinGW compatibility issues.
  while(in->good() && /*line.back() == '['*/line[line.size()-1] == '[') {
    line.erase(line.size()-1);  // line.pop_back();
    Node<Factorization>* child = new Node<Factorization>(line);
    n->Add(child);
    RecursiveSerialBuild(in, child);

    getline(*in, line);
  }
  // else line.back was "]", or at end of file
}

// The Tree class handles destruction on its own
BeurlingTreeBase::~BeurlingTreeBase() {}

//void BeurlingTreeBase::NextLevel() {}
void BeurlingTreeBase::SerializeToFile(string filename) {
  SerialBase::SetFileStream(filename);
  SerialRec1 rec1;
  SerialRec2 rec2;
  SerialLeaf leaf;
  tree.DepthFirst(&rec1, &rec2, &leaf);
  SerialBase::CloseFileStream();
}

void BeurlingTreeBase::ExportAsDot(string filename) {

  graphviz_node_counter = 0;
  graph_file.open(filename.c_str());

  Node<Factorization>* root_node = tree.GetRoot();
  unsigned int root_graphviz_number = graphviz_node_counter;

  graph_file << "digraph G {" << endl;
  graph_file << '\t' <<  graphviz_node_counter
             << " [label=\"" << root_node->GetData().ToDotString() << "\"];"
             << endl;
  RecursiveExportAsDot(root_node,  root_graphviz_number);
  graph_file << "}" ;
  graph_file.close();
}

void BeurlingTreeBase::RecursiveExportAsDot(Node<Factorization>* current_node,
                                       unsigned int current_graphviz_number) {
  vector< Node<Factorization>* > children = current_node->GetChildren();
  for (Node<Factorization>* child : children){
    graphviz_node_counter++;
    // this is the child's id:
    unsigned int child_graphviz_number = graphviz_node_counter;
    // Add link of current_node to child_node
    AddToGraphvizFile(current_graphviz_number, child_graphviz_number, child);
    RecursiveExportAsDot(child, child_graphviz_number);
  }
}

void BeurlingTreeBase::AddToGraphvizFile(unsigned int parent_graphviz_number,
                                    unsigned int child_graphviz_number,
                                    Node<Factorization>* child){
  // Give a label to the child
  graph_file << '\t' <<  child_graphviz_number
             << " [label=\"" << child->GetData().ToDotString() << "\"];"
             << endl;

  // Link the child to the parent
  graph_file << '\t' <<  parent_graphviz_number
             << "->"
             << child_graphviz_number << ";" << endl;
}

// Returns a number triangle giving the frequencies of values of the prime
// counting function at different heights of the tree.
vector< vector<unsigned int> > BeurlingTreeBase::GetTriangle() {
  PcfBase::SetTree(this);
  PcfBase::ResetTriangle();
  PcfRec1 rec1;
  PcfRec2 rec2;
  PcfLeaf leaf;
  tree.DepthFirst(&rec1, &rec2, &leaf);
  return PcfBase::GetTriangle();
}

void BeurlingTreeBase::PcfRec1::operator() (Node<Factorization>* n) {
  height++;
  while (triangle.size() < (unsigned int)height+1)
    triangle.push_back(vector<unsigned int>());
  if (n->GetData().IsPrime())
    prime_count++;
  while (triangle[height].size() < prime_count)
    triangle[height].push_back(0);
  triangle[height][prime_count-1]++;
}

void BeurlingTreeBase::PcfRec2::operator() (Node<Factorization>* n) {
  height--;
  if (n->GetData().IsPrime())
    prime_count--;
}

void BeurlingTreeBase::PcfLeaf::operator() (Node<Factorization>* n) {
  height++;
  while (triangle.size() < (unsigned int)height+1)
    triangle.push_back(vector<unsigned int>());
  if (n->GetData().IsPrime())
    prime_count++;
  while (triangle[height].size() < prime_count)
    triangle[height].push_back(0);
  triangle[height][prime_count-1]++;

  height--;
  if (n->GetData().IsPrime())
    prime_count--;
}

void BeurlingTreeBase::SerialRec1::operator() (Node<Factorization>* n) {
  out_file << n->GetData().ToSerialString() << "[\n";
}

void BeurlingTreeBase::SerialRec2::operator() (Node<Factorization>* n) {
  out_file << "]\n";
}

void BeurlingTreeBase::SerialLeaf::operator() (Node<Factorization>* n) {
  out_file << n->GetData().ToSerialString() << "[\n" << "]\n";
}

}  // namespace Platt

/*
 * integer_tree.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Devin
 *
 *  Defines the IntegerTree class.
 */

#include "integer_tree.h"
#include <iostream>
#include <string>
using namespace std;

namespace Platt {

// Initialize static members.
IntegerTree* IntegerTree::PcfBase::tree_ptr = nullptr;
vector< vector<unsigned int> > IntegerTree::PcfBase::triangle
    = vector< vector<unsigned int> >();
unsigned int IntegerTree::PcfBase::prime_count = 0;
int IntegerTree::PcfBase::height = -1;
ofstream IntegerTree::SerialBase::out_file;

IntegerTree::IntegerTree() {
  // Init root node
  tree.Init(Factorization(0));
  graphviz_node_counter = 0;
}

IntegerTree::IntegerTree(unsigned int height) {
  // Init root node
  tree.Init(Factorization(0));
  RecursiveBuild(height, tree.GetRoot());
}

void IntegerTree::RecursiveBuild(unsigned int height, Node<Factorization>* n) {

  //cout << table.DebugString() << endl;

  if (height > 0) {
    // Add composites and recurse
    vector<Candidate> candidates = table.GetCandidates();
    for(Candidate c : candidates) {
      Node<Factorization>* child = new Node<Factorization>(c.GetFactors());
      n->Add(child);
      if (height > 1) {
        table.PushComposite(c);
        RecursiveBuild(height - 1, child);
        table.PopComposite(c);
      }
    }

    // Add prime and recurse
    Node<Factorization>* child = new Node<Factorization>(
                                         Factorization(table.GetPrimeCount()));
    n->Add(child);
    if (height > 1) {
      table.PushPrime();
      RecursiveBuild(height - 1, child);
      table.PopPrime();
    }
  }
}

IntegerTree::IntegerTree(string filename) {
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

void IntegerTree::RecursiveSerialBuild(ifstream* in, Node<Factorization>* n) {
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

void IntegerTree::NextLevel() {}
void IntegerTree::SerializeToFile(string filename) {
  SerialBase::SetFileStream(filename);
  SerialRec1 rec1;
  SerialRec2 rec2;
  SerialLeaf leaf;
  tree.DepthFirst(&rec1, &rec2, &leaf);
  SerialBase::CloseFileStream();
}

void IntegerTree::ExportAsDot(string filename) {

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

// assign number to node

void IntegerTree::RecursiveExportAsDot(Node<Factorization>* current_node,
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

void IntegerTree::AddToGraphvizFile(unsigned int parent_graphviz_number,
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
vector< vector<unsigned int> > IntegerTree::GetTriangle() {
  PcfBase::SetTree(this);
  PcfBase::ResetTriangle();
  PcfRec1 rec1;
  PcfRec2 rec2;
  PcfLeaf leaf;
  tree.DepthFirst(&rec1, &rec2, &leaf);
  return PcfBase::GetTriangle();
}

void IntegerTree::PcfRec1::operator() (Node<Factorization>* n) {
  height++;
  while (triangle.size() < (unsigned int)height+1)
    triangle.push_back(vector<unsigned int>());
  if (n->GetData().IsPrime())
    prime_count++;
  while (triangle[height].size() < prime_count)
    triangle[height].push_back(0);
  triangle[height][prime_count-1]++;
}

void IntegerTree::PcfRec2::operator() (Node<Factorization>* n) {
  height--;
  if (n->GetData().IsPrime())
    prime_count--;
}

void IntegerTree::PcfLeaf::operator() (Node<Factorization>* n) {
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

void IntegerTree::SerialRec1::operator() (Node<Factorization>* n) {
  out_file << n->GetData().ToSerialString() << "[\n";
}

void IntegerTree::SerialRec2::operator() (Node<Factorization>* n) {
  out_file << "]\n";
}

void IntegerTree::SerialLeaf::operator() (Node<Factorization>* n) {
  out_file << n->GetData().ToSerialString() << "[\n" << "]\n";
}

}  // namespace Platt





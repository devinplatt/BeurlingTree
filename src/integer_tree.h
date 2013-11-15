/*
 * IntegerTree.h
 *
 *  Created on: Nov 14, 2013
 *      Author: Devin
 *
 *  Declares the IntegerTree class. This class builds a tree giving the partial
 *  ordering of prime factorizations of Beurling generalized integers up to
 *  a finite height. It also outputs a triangle that stores the count of values
 *  of the prime counting function of different branches at a given height.
 */

#ifndef INTEGERTREE_H_
#define INTEGERTREE_H_

#include <vector>
#include <string>
#include <fstream>
#include "multiplication_table.h"
#include "tree.h"
using namespace std;

namespace Platt {

class IntegerTree {
 private:
  Tree<Factorization> tree;
  MultiplicationTable table;

  // Members used for creating a Graphviz file
  unsigned int graphviz_node_counter;   //
  ofstream graph_file;                  //

  void RecursiveBuild(unsigned int height, Node<Factorization>* n);
  void RecursiveSerialBuild(ifstream* in, Node<Factorization>* n);
  void RecursiveExportAsDot(Node<Factorization>* current_node,
                            unsigned int current_graphviz_number);
  void AddToGraphvizFile(unsigned int parent_graphviz_number,
                         unsigned int child_graphviz_number,
                         Node<Factorization>* child);
 public:
  /* We declare some functor classes. One set is used for creating a triangle
   * of prime counting function (pcf) values. The other is used for
   * serialization.
   */
  class PcfBase {
   protected:
    static IntegerTree* tree_ptr;
    static vector< vector<unsigned int> > triangle;
    static unsigned int prime_count;
    // Refers to height of triangle. Will correspond to height+1 of the tree.
    static int height;

   public:
    static void SetTree(IntegerTree* in) {tree_ptr = in;}
    static void ResetTriangle() {triangle.clear();}
    static vector< vector<unsigned int> > GetTriangle() {return triangle;}
  };

  class PcfRec1 : public PcfBase {
   public:
    void operator() (Node<Factorization>* n);
  };

  class PcfRec2 : public PcfBase {
   public:
    void operator() (Node<Factorization>* n);
  };

  class PcfLeaf : public PcfBase {
   public:
    void operator() (Node<Factorization>* n);
  };

  class SerialBase {
   protected:
    //static IntegerTree* tree_ptr;
    static ofstream out_file;

   public:
    //static void SetTree(IntegerTree* in) {tree_ptr = in;}
    static void SetFileStream(string s) {out_file.open(s.c_str());}
    static void CloseFileStream() {out_file.close();}
  };

  class SerialRec1 : public SerialBase {
   public:
    void operator() (Node<Factorization>* n);
  };

  class SerialRec2 : public SerialBase {
   public:
    void operator() (Node<Factorization>* n);
  };

  class SerialLeaf : public SerialBase {
   public:
    void operator() (Node<Factorization>* n);
  };

  // Default Constructor initializes with just a root node.
  IntegerTree();
  IntegerTree(unsigned int height);
  // Construct from deserialization of a file
  IntegerTree(string filename);
  // TODO: implement NextLevel().
  void NextLevel();
  /* Serialization is of plaintext form:
   *  #,#|#,#[
   *  #,#[
   *  ]
   *  #,#|#,#|#,#[
   *  ]
   *  ]
   */
  void SerializeToFile(string filename);
  void ExportAsDot(string filename);
  // Returns a number triangle giving the frequencies of values of the prime
  // counting function at different heights of the tree.
  vector< vector<unsigned int> > GetTriangle();
};

}  // namespace Platt

#endif /* INTEGERTREE_H_ */

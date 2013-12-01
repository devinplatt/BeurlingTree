/*
 * beurling_tree_base.h
 *
 *  Created on: Nov 17, 2013
 *      Author: Devin
 *
 *  Declares the BeurlingTreeBase class, the base class for IntegerTree and
 *  PrimePowerTree. This is an abstract base class since RecursiveBuild() is a
 *  pure virtual function.
 */

#ifndef BEURLING_TREE_BASE_H_
#define BEURLING_TREE_BASE_H_

#include <vector>
#include <string>
#include <fstream>
#include "multiplication_table.h"
#include "tree.h"
using std::ofstream;
using std::ifstream;

namespace Platt {

class BeurlingTreeBase {
 protected:
  Tree<Factorization> tree;
  MultiplicationTable table;

  // Members used for creating a Graphviz file
  unsigned int graphviz_node_counter;   //
  ofstream graph_file;                  //

  virtual void RecursiveBuild(unsigned int height, Node<Factorization>* n) = 0;
  void RecursiveSerialBuild(ifstream* in, Node<Factorization>* n);
  void RecursiveExportAsDot(Node<Factorization>* current_node,
                            unsigned int current_graphviz_number);
  void AddToGraphvizFile(unsigned int parent_graphviz_number,
                         unsigned int child_graphviz_number,
                         Node<Factorization>* child);
  // Initialization functions to be used in the constructors of the child
  // classes.
  void InitDefault();
  void InitToHeight(unsigned int height);
  void InitFromFile(string filename);
 public:
  /* We declare some functor classes. One set is used for creating a triangle
   * of prime counting function (pcf) values. The other is used for
   * serialization.
   */
  class PcfBase {
   protected:
    static BeurlingTreeBase* tree_ptr;
    static vector< vector<unsigned int> > triangle;
    static unsigned int prime_count;
    // Refers to height of triangle. Will correspond to height+1 of the tree.
    static int height;

   public:
    static void SetTree(BeurlingTreeBase* in) {tree_ptr = in;}
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
    //static BeurlingTreeBase* tree_ptr;
    static ofstream out_file;

   public:
    //static void SetTree(BeurlingTreeBase* in) {tree_ptr = in;}
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

  virtual ~BeurlingTreeBase();

  // virtual void NextLevel();

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

#endif /* BEURLING_TREE_BASE_H_ */

/*
 * integer_tree.h
 *
 *  Created on: Nov 14, 2013
 *      Author: Devin
 *
 *  Declares the IntegerTree class. This class builds a tree giving the partial
 *  ordering of prime factorizations of Beurling generalized integers up to
 *  a finite height. It also outputs a triangle that stores the count of values
 *  of the prime counting function of different branches at a given height.
 *
 *  Most of the implementation resides in the BeurlingTreeBase class. The key
 *  algorithmic details reside in the MultiplicationTable class.
 */

#ifndef INTEGER_TREE_H_
#define INTEGER_TREE_H_

#include "beurling_tree_base.h"

namespace Platt {

class IntegerTree : public BeurlingTreeBase {
 private:
  void RecursiveBuild(unsigned int height, Node<Factorization>* n);

 public:
  // Default Constructor initializes with just a root node.
  IntegerTree();
  IntegerTree(unsigned int height);
  // Construct from deserialization of a file
  IntegerTree(string filename);
  // TODO: implement NextLevel().
  void NextLevel();
};

}  // namespace Platt

#endif /* INTEGER_TREE_H_ */

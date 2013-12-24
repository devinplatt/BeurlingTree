/*
 * restricted_tree.h
 *
 *  Created on: Dec 4, 2013
 *      Author: Devin
 *
 *  Declares the RestrictedTree class, which builds a tree with a limited
 *  number of primes and/or composites in any branch. Used by the coefficient
 *  generator (DiagonalFormula).
 *  Eg.
 *    - RestrictedTree(-1,-1,h) builds a regular tree to height h.
 *    - RestrictedTree(3,4,8) builds a tree with at most 3 primes and at
 *    most 4 composites in any branch. Since 3+4=7, the limit of 8 is
 *    irrelevant in this case. The tree is built to a height of min(p+c,h).
 *
 */

#ifndef RESTRICTED_TREE_H_
#define RESTRICTED_TREE_H_

#include "beurling_tree_base.h"

namespace Platt {

class RestrictedTree : public BeurlingTreeBase {
 protected:
  int max_primes;
  int max_composites;
  int num_primes;
  int num_composites;
  void RecursiveBuild(unsigned int height, Node<Factorization>* n);

 public:
  // Default Constructor initializes with just a root node.
  RestrictedTree();
  RestrictedTree(int primes, int composites, int height);
  // Construct from deserialization of a file. At the moment there is no method
  // for determining the max_primes and max_composites values, so those are set
  // to -1.
  RestrictedTree(string filename);
  // TODO: implement NextLevel().
  void NextLevel();
};

}  // namespace Platt


#endif  /* RESTRICTED_TREE_H_ */

/*
 * prime_power_tree.h
 *
 *  Created on: Nov 17, 2013
 *      Author: Devin
 *
 *  Declaration of the PrimePowerTree class. This class differs from the
 *  IntegerTree class in that it builds a tree for the partial order on powers
 *  of primes, as opposed to the order on all factorizations.
 */

#ifndef PRIME_POWER_TREE_H_
#define PRIME_POWER_TREE_H_

#include "beurling_tree_base.h"

namespace Platt {

class PrimePowerTree : public BeurlingTreeBase {
 private:
  void RecursiveBuild(unsigned int height, Node<Factorization>* n);
  void RecursiveBuildContinue(unsigned int height, Node<Factorization>* n);

 public:
  // Default Constructor initializes with just a root node.
  PrimePowerTree();
  PrimePowerTree(unsigned int height);
  // Construct from deserialization of a file
  PrimePowerTree(string filename);
  // TODO: implement NextLevel().
  void NextLevel();
};

}  // namespace Platt

#endif /* PRIME_POWER_TREE_H_ */

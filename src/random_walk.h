/*
 * random_walk.h
 *
 *  Created on: Dev 14, 2015
 *      Author: Devin
 *
 *  Declares the RandomWalk class. This class builds a tree giving the partial
 *  ordering of prime factorizations of Beurling generalized integers up to
 *  a finite height. 
 *
 *  Most of the implementation resides in the BeurlingTreeBase class. The key
 *  algorithmic details reside in the MultiplicationTable class.
 */

#ifndef RANDOM_WALK_H_
#define RANDOM_WALK_H_

#include "beurling_tree_base.h"

namespace Platt {

class RandomWalk : public BeurlingTreeBase {
 private:
  vector<Factorization> path;
  vector<int> number_primes;
  vector<int> number_children;
  void RecursiveBuild(unsigned int height, Node<Factorization>* n);

 public:
  // Default Constructor initializes with just a root node.
  RandomWalk();
  RandomWalk(unsigned int height);
  // Construct from deserialization of a file
  RandomWalk(string filename);
  // TODO: implement NextLevel().
  void NextLevel();

  int length();
  int NumPrimes(int n);
  int NumPrimeFactors(int n);
  int NumDistinctPrimeFactors(int n);
  int NumChildren(int n);
};

}  // namespace Platt

#endif /* RANDOM_WALK_H_ */
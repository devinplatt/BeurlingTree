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
using std::ios;
using std::endl;

namespace Platt {

IntegerTree::IntegerTree() {
  InitDefault();
}

IntegerTree::IntegerTree(unsigned int height) {
  InitToHeight(height);
}

IntegerTree::IntegerTree(string filename) {
  InitFromFile(filename);
}

void IntegerTree::NextLevel() {}

void IntegerTree::RecursiveBuild(unsigned int height, Node<Factorization>* n) {
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

}  // namespace Platt

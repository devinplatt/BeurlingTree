/*
 * restricted_tree.cpp
 *
 *  Created on: Dec 4, 2013
 *      Author: Devin
 *
 *  Defines the RestrictedTree class.
 */

#include "restricted_tree.h"
#include <iostream>
#include <string>
using std::ios;
using std::endl;

namespace Platt {

RestrictedTree::RestrictedTree() {
  max_primes = -1;
  max_composites = -1;
  num_primes = 1;
  num_composites = 0;
  InitDefault();
}

RestrictedTree::RestrictedTree(int primes, int composites, int height) {
  max_primes = primes;
  max_composites = composites;
  num_primes = 1;
  num_composites = 0;
  InitToHeight(height);
}

RestrictedTree::RestrictedTree(string filename) {
  max_primes = -1;
  max_composites = -1;
  num_primes = 1;
  num_composites = 0;
  InitFromFile(filename);
}

void RestrictedTree::NextLevel() {}

void RestrictedTree::RecursiveBuild(unsigned int height,
                                    Node<Factorization>* n) {
  if (height > 0) {

    if (max_composites == -1 || num_composites < max_composites) {
      // Add composites and recurse
      vector<Candidate> candidates = table.GetCandidates();
      for(Candidate c : candidates) {
        Node<Factorization>* child = new Node<Factorization>(c.GetFactors());
        n->Add(child);
        if (height > 1) {
          table.PushComposite(c);
          num_composites++;
          RecursiveBuild(height - 1, child);
          num_composites--;
          table.PopComposite(c);
        }
      }
    }

    if(max_primes == -1 || num_primes < max_primes) {
      // Add prime and recurse
      Node<Factorization>* child = new Node<Factorization>(
                                         Factorization(table.GetPrimeCount()));
      n->Add(child);
      if (height > 1) {
        table.PushPrime();
        num_primes++;
        RecursiveBuild(height - 1, child);
        num_primes--;
        table.PopPrime();
      }
    }
  }
 }

}  // namespace Platt

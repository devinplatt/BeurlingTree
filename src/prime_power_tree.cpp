/*
 * prime_power_tree.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: Devin
 *
 *  Implementation of the PrimePowerTree class.
 */

#include "prime_power_tree.h"

namespace Platt {

PrimePowerTree::PrimePowerTree() {
  InitDefault();
}

PrimePowerTree::PrimePowerTree(unsigned int height) {
  InitToHeight(height);
}

PrimePowerTree::PrimePowerTree(string filename) {
  InitFromFile(filename);
}

void PrimePowerTree::NextLevel() {}

/* We build the multiplication table as usual for all integers, but we only add
 * integers to the tree if they are powers of primes. Thus we may need to do
 * some exploration akin to going down many more lengths of the integer tree
 * in order to get the PrimePowerTree to the desired height.
 */
void PrimePowerTree::RecursiveBuild(unsigned int height,
                                    Node<Factorization>* n) {
  if (height > 0) {
    // Add composites and recurse
    vector<Candidate> candidates = table.GetCandidates();
    for(Candidate c : candidates) {
      if (c.GetFactors().IsPrimePower()) {
        Node<Factorization>* child;
        // If factorization is not already a child of n, add it as a child.
        if (!n->HasChild(c.GetFactors())) {
          child = new Node<Factorization>(c.GetFactors());
          n->Add(child);
        }
        else {  // Otherwise, get the existing child.
          child = n->GetChild(c.GetFactors());
        }
        if (height > 1) {
          table.PushComposite(c);
          RecursiveBuild(height - 1, child);
          table.PopComposite(c);
        }
      }
      else {
        table.PushComposite(c);
        RecursiveBuildContinue(height, n);
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

// Same as RecursiveBuild, but without adding a prime
void PrimePowerTree::RecursiveBuildContinue(unsigned int height,
                                    Node<Factorization>* n) {
  if (height > 0) {
    // Add composites and recurse
    vector<Candidate> candidates = table.GetCandidates();
    for(Candidate c : candidates) {
      if (c.GetFactors().IsPrimePower()) {
        Node<Factorization>* child = new Node<Factorization>(c.GetFactors());
        n->Add(child);
        if (height > 1) {
          table.PushComposite(c);
          RecursiveBuild(height - 1, child);
          table.PopComposite(c);
        }
      }
      else {
        table.PushComposite(c);
        RecursiveBuildContinue(height, n);
        table.PopComposite(c);
      }
    }
  }
}

}  // namespace Platt

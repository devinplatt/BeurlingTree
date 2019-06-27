/*
 * random_walk.cpp
 *
 *  Created on: Dev 14, 2015
 *      Author: Devin
 *
 *  Defines the RandomWalk class.
 */

#include "random_walk.h"
#include <iostream>
#include <random>
#include <string>
using std::ios;
using std::endl;

namespace Platt {

// http://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister)
int MIN_RANDOMVAL = 0;
int MAX_RANDOMVAL = 20000;
// Guaranteed unbiased.
std::uniform_int_distribution<int> uni(MIN_RANDOMVAL, MAX_RANDOMVAL);

RandomWalk::RandomWalk() {
  InitDefault();
}

RandomWalk::RandomWalk(unsigned int height) {
  number_primes.push_back(1);
  number_children.push_back(2);
  path.push_back(Factorization(0));
  InitToHeight(height);
}

RandomWalk::RandomWalk(string filename) {
  InitFromFile(filename);
}

void RandomWalk::NextLevel() {}

void RandomWalk::RecursiveBuild(unsigned int height, Node<Factorization>* n) {
  if (height > 0) {
    // Get composites.
    vector<Candidate> candidates = table.GetCandidates();
    number_children.push_back(candidates.size() + 1);
    number_primes.push_back(number_primes[number_primes.size()-1]);
    
    // Choose either the prime or one of the composites.
    // std::cout << rng << std::endl;
    int index = uni(rng) % (candidates.size()+1);
    // index = 0;
    // std::cout << index << std::endl;
    
    // prime case
    if (index == candidates.size()) {
      // Add prime and recurse
      Node<Factorization>* child = new Node<Factorization>(
                                           Factorization(table.GetPrimeCount()));
      n->Add(child);
      path.push_back(Factorization(table.GetPrimeCount()));
      number_primes[number_primes.size()-1] += 1;
      if (height > 1) {
        table.PushPrime();
        RecursiveBuild(height - 1, child);
        table.PopPrime();
      }
    } else {  // one of the composites
      Candidate c = candidates[index];
      Node<Factorization>* child = new Node<Factorization>(c.GetFactors());
      n->Add(child);
      path.push_back(Factorization(c.GetFactors()));
      if (height > 1) {
        table.PushComposite(c);
        RecursiveBuild(height - 1, child);
        table.PopComposite(c);
      }
    }
  }
}

int RandomWalk::length() {
  return path.size();
}

int RandomWalk::NumPrimes(int n) {
  return number_primes[n];
};

int RandomWalk::NumPrimeFactors(int n) {
  return path[n].NumPrimeFactors();
}

int RandomWalk::NumDistinctPrimeFactors(int n) {
  return path[n].NumDistinctPrimeFactors();
}

int RandomWalk::NumChildren(int n) {
  return number_children[n];
}

Factorization RandomWalk::GetFactorization(int n) const {
  return path[n];
}

}  // namespace Platt


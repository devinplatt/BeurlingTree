/*
 * FactorizationSequence.h
 *
 *  Created on: Dec 4, 2013
 *      Author: Devin
 *
 *      Declares and defines the FactorizationSequence class, which is used by
 *      the coefficient generator (DiagonalFormula).
 */

#ifndef FACTORIZATIONSEQUENCE_H_
#define FACTORIZATIONSEQUENCE_H_

#include "factorization.h"
#include <vector>
#include <algorithm>
using std::vector;
using std::min;

namespace Platt {

class FactorizationSequence {
 private:
  vector<Factorization> factors;

 public:
  void Push(Factorization f) {factors.push_back(f);}
  void Pop() {factors.pop_back();}
  Factorization Back() {return factors.back();}

  string ToString() {
    string out = "[";
    for(Factorization f: factors) {
      out += f.ToDotString() + ",";
    }
    out[out.size()-1] = ']';
    return out;
  }

  // Used for std associative containers
  bool operator< (const FactorizationSequence& rhs) const {
    for (size_t x = 0; x < min(rhs.factors.size(), factors.size()); ++x) {
       if (factors[x] < rhs.factors[x])
         return true;
       else if (factors[x] != rhs.factors[x])
         return false;
    }

    // The sequences have the same first values
    if (factors.size() < rhs.factors.size())
      return true;
    else return false;
  }

  // Boilerplate that in an ideal world wouldn't as verbose (ie vector ==)
  bool operator == (const FactorizationSequence& rhs) const {
    if (factors.size() != rhs.factors.size())
      return false;
    for (size_t x = 0; x < factors.size(); ++x) {
      if (factors[x] != rhs.factors[x])
        return false;
    }
    return true;
  }
};

}  // namespace Platt

#endif /* FACTORIZATIONSEQUENCE_H_ */

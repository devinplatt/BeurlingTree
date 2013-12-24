/*
 * diagonal_formula.h
 *
 *  Created on: Dec 4, 2013
 *      Author: Devin
 *
 *  Declares the DiagonalFormula class, which implements an algorithm which
 *  determines the formula for a given diagonal by finding the binomial
 *  coefficients whose sum make this formula. The existence of this algorithm
 *  actually is proof enough to make interpolation a valid method for finding
 *  the polynomial formula for the diagonal.
 */

#ifndef DIAGONAL_FORMULA_H_
#define DIAGONAL_FORMULA_H_

#include "restricted_tree.h"
#include "factorization_sequence.h"

namespace Platt {

class DiagonalFormula : public RestrictedTree {

 private:
  vector<FactorizationSequence> FSvec;
  vector<int> n_offset;
  vector<int> k;

 public:
  // Default Constructor initializes with just a root node, useless...
  DiagonalFormula() : RestrictedTree() {}
  DiagonalFormula(int d);
  string GetFormulaString();
  vector<Tuple> GetFormula();
};

}  // namespace Platt

#endif /* DIAGONAL_FORMULA_H_ */

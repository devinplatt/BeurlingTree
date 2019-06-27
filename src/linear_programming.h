/*
 * linear_programming.h
 *
 *  Created on: May 20, 2019
 *      Author: Devin
 *
 *  Linear programming code, used to detect any invalid sequence of
 *  factorizations after the multiplication table's binary product check has
 *  been done.
 *
 */

#ifndef LINEAR_PROGRAMMING_H_
#define LINEAR_PROGRAMMING_H_

#include <algorithm>  // For std::max
#include <iostream>
#include <string>
#include <vector>
#include "factorization.h"
using std::cout;
using std::endl;
using std::max;
using std::string;
using std::vector;

namespace Platt {

// An exception class to handle an error if the linear programming backend fails
// to either find a feasible solution, or prove that a feasible solution does
// not exist.
// Borrowed from https://riptutorial.com/cplusplus/example/23640/custom-exception
class LinearProgrammingException: virtual public std::exception {
protected:
  std::string debug_str;
  std::string error_message;
public:
  explicit
  LinearProgrammingException(const string& msg, const string& debug_info):
    error_message(msg),
    debug_str(debug_info)
    {}
  ~LinearProgrammingException() throw () {}
  /** Returns a pointer to the (constant) error description.
   *  @return A pointer to a const char*. The underlying memory
   *  is in possession of the Except object. Callers must
   *  not attempt to free the memory.
   */
  virtual const char* what() const throw () {
    return error_message.c_str();
  }
  const std::string& get_debug_info() const {
    return debug_str;
  }
};

// Returns pairs of <1-indexed prime index, coefficient value> for the 
// constraint row defined by f1 < f2,  ie. f2 - f1 > 0.
vector<pair<int, double>> GetConstraintCoefficients(Factorization f1,
                                                    Factorization f2);
// For print debugging.
void PrintMatrixDebugString(int num_constraints, int num_primes,
                            int num_nonzero_elements, int* rowIndices,
                            int* colIndices, double* elements);
bool IsFeasibleSequence(
    const vector<Factorization>& current_sequence,
    Factorization candidate,
    const vector<Factorization>& other_candidates);

}  // namespace Platt

#endif /* LINEAR_PROGRAMMING_H_ */


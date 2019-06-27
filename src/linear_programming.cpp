/*
 * linear_programming.cpp
 *
 *  Created on: May 20, 2019
 *      Author: Devin
 *
 *  Linear programming code, used to detect any invalid sequence of
 *  factorizations after the multiplication table's binary product check has
 *  been done.
 *
 *  This implementation uses the Coin-Or CLP library.
 */

#include <algorithm>  // For std::max
#include <iostream>
#include <string>
#include <vector>
#include "ClpSimplex.hpp"
#include "CoinHelperFunctions.hpp"
#include "factorization.h"
#include "linear_programming.h"
using std::cout;
using std::endl;
using std::max;
using std::string;
using std::vector;

namespace Platt {

// Returns pairs of <prime index, coefficient value> for the 
// constraint row defined by f1 < f2,  ie. f2 - f1 > 0.
vector<pair<int, double>> GetConstraintCoefficients(Factorization f1,
                                                    Factorization f2) {
  int max_prime = max(f1.GetMaxPrime(), f2.GetMaxPrime());
  vector<double> coefficients;
  for (int i = 0; i <= max_prime; i++) {
    coefficients.push_back(0.0);
  }
  for (Tuple t : f2.GetFactors()) {
    coefficients[t.first] = t.second;
  }
  for (Tuple t : f1.GetFactors()) {
    coefficients[t.first] -= t.second;
  }
  vector<pair<int, double>> sparse_coefficients;
  for (int i = 0; i <= max_prime; i++) {
    if (coefficients[i] != 0) {
      sparse_coefficients.push_back(pair<int, double>(i, coefficients[i]));
    }
  }
  return sparse_coefficients;
}

// For print debugging.
void PrintMatrixDebugString(int num_constraints, int num_primes,
                            int num_nonzero_elements, int* rowIndices,
                            int* colIndices, double* elements) {
  vector<vector<double>> matrix(num_constraints, vector<double>(num_primes));
  for (int i = 0; i < num_nonzero_elements; i++) {
    int row = rowIndices[i];
    int col = colIndices[i];
    double value = elements[i];
    matrix[row][col] = value;
  }
  cout << "num_nonzero_elements: " << num_nonzero_elements << endl;
  cout << "num_constraints: " << num_constraints << endl;
  cout << "num_primes: " << num_primes << endl;
  cout << "Matrix values:" << endl;
  for (int i = 0; i < num_constraints; i++) {
    for (int j = 0; j < num_primes; j++) {
      cout << matrix[i][j] << "\t";
    }
    cout << endl;
  }
  cout << "End matrix" << endl;
}

bool IsFeasibleSequence(
  const vector<Factorization>& current_sequence,
  Factorization candidate,
  const vector<Factorization>& other_candidates) {
  // Determine: num_primes (col) and num_constraints (row).
  // The vector current_sequence includes the identity factorization.
  int num_constraints = current_sequence.size() - 1
                        + 1
                        + other_candidates.size();
  int num_primes = 0;
  for (Factorization f : current_sequence) {
    num_primes = max(num_primes, f.GetMaxPrime());
  }
  num_primes++;
  // Setup LP solver.
  // Variable declarations.
  ClpSimplex  model;
  model.setLogLevel(0);  // Supress console output.
  // The constraint matrix is defined using a sparse notation.
  // Arrays for row index i and column index j.
  vector<int> rowIndices;
  vector<int> colIndices;
  // Array for values "r" at indices i,j. (A_{i,j} = r).
  vector<double> elements;
  // Set the values of the constraint matrix.
  // This is the lefthand side of the inequality Ax ?= b. (In our case Ax > 0)
  // Anything not explicitly set is assumed to be 0.
  // We add constraints
  //   - for current sequence
  //   - for candidate factorization
  //   - for other candidates
  // Add constraints for the current sequence.
  int num_nonzero_elements = 0;
  int row_index = 0;
  while(row_index < current_sequence.size() - 1) {
    vector<pair<int, double>> constraint_coefficients = 
        GetConstraintCoefficients(current_sequence[row_index],
                                  current_sequence[row_index + 1]);
    for (auto p : constraint_coefficients) {
      rowIndices.push_back(row_index);
      colIndices.push_back(p.first);
      elements.push_back(p.second);
      num_nonzero_elements++;
    }
    row_index++;
  }
  // Add constraints for the candidate factorization.
  vector<pair<int, double>> constraint_coefficients = 
      GetConstraintCoefficients(current_sequence.back(), candidate);
  for (auto p : constraint_coefficients) {
    rowIndices.push_back(row_index);
    colIndices.push_back(p.first);
    elements.push_back(p.second);
    num_nonzero_elements++;
  }
  row_index++;
  // Add constraints for the other candidate factorizations.
  for(Factorization other_candidate : other_candidates) {
    vector<pair<int, double>> constraint_coefficients = 
        GetConstraintCoefficients(candidate, other_candidate);
    for (auto p : constraint_coefficients) {
      rowIndices.push_back(row_index);
      colIndices.push_back(p.first);
      elements.push_back(p.second);
      num_nonzero_elements++;
    }
    row_index++;
  }
  // Make sure the row_index and num_constraints variables are consistent.
  if (row_index != num_constraints) {
    cout << "Improper row_index" << endl;
    cout << "num_constraints == " << num_constraints << endl;
    cout << "row_index == " << row_index << endl;
  }
  // Load the matrix.
  // The "true" means that the matrix data are stored in column order.
  CoinPackedMatrix matrix(true, rowIndices.data(), colIndices.data(),
                          elements.data(), num_nonzero_elements);
  // Print Debugging for small examples.
  // PrintMatrixDebugString(num_constraints, num_primes, num_nonzero_elements,
  //                        rowIndices, colIndices, elements);
  // Create problem.
  // Set the coefficients of optimization.
  // These values are required for running the program, but
  // otherwise irrelevant to us since we only care about
  // feasibility of the constraints and not any particular values
  // of a solution.
  vector<double> objective;
  for (int j = 0; j < num_primes; j++) {
    objective.push_back(1);
  }
  // Set the "row bounds". This is the righthand side of the
  // inequality Ax ?= b. Ultimately, we wish to solve the
  // inequality Ax > 0, so we use small "epsilon" lower bound values for b.
  double epsilon = 0.01;
  vector<double> rowLower;
  for (int i = 0; i < num_constraints; i++) {
    rowLower.push_back(epsilon);
  }
  // Load the problem.
  // According to https://www.coin-or.org/Doxygen/Clp/classClpSimplex.html,
  // NULL pointers means default to largest possible row/column upper/lower
  // bounds.
  model.loadProblem(matrix, 0, 0, objective.data(), rowLower.data(), 0);
  // Run the solver so can check if the problem is infeasible.
  model.initialSolve();
  // Return true or false.
  if (model.isProvenOptimal()) {
    // Housekeeping: nothing to do.
    return true;
  } else if (model.isProvenPrimalInfeasible()) {
    // Housekeeping: nothing to do.
    return false;
  } else {
    string error_string = string("Linear Programming Exception: CLP has failed"
        " to either find a feasible solution or prove that a feasible solution"
        " does not exist.");
    string debug_string = "";
    if (model.isPrimalObjectiveLimitReached()) {
      debug_string += "Primal Objective Limit Reached\n";
    }
    if (model.isIterationLimitReached()) {
      debug_string += "Iteration Limit Reached\n";
    }
    if (model.isAbandoned()) {
      debug_string += "Problem \"Abandoned\" by CLP\n";
    }
    throw(LinearProgrammingException(error_string, debug_string));
  }
}

}  // namespace Platt


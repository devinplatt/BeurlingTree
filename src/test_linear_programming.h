/*
 * test_linear_programming.h
 *
 *  Created on: May 20, 2019
 *      Author: Devin
 *
 *  Test for the linear programming code.
 *  We test
 *    (1) That an invalid sequence of factorizations is detected (True Negative)
 *    (2) That an valid sequence of factorizations is accepted (True Positive)
 */

#ifndef TEST_LINEAR_PROGRAMMING_H_
#define TEST_LINEAR_PROGRAMMING_H_

#include <iostream>
#include <string>
#include <vector>
#include "factorization.h"
#include "linear_programming.h"
#include "test_utils.h"
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;

namespace Platt {

void TestValidConfiguration(bool* pass, string* error,
                            Factorization candidiate_factorization,
    const vector<Factorization>& other_candidate_factorizations,
    const vector<Factorization>& current_sequence) {
  //std::cout << "Running TestValidConfiguration" << std::endl;  // Print debugging.
  string error_message;
  error_message += "Function: TestValidConfiguration\n";
  error_message += "Candidate: " + candidiate_factorization.ToDotString()
                   + "\n";
  error_message += "\nCurrent Sequence: ";
  for (Factorization f : current_sequence)
    error_message += f.ToDotString() + "\t";
  error_message += "\nOther Candidate factorizations: ";
  for (Factorization f : other_candidate_factorizations)
    error_message += f.ToDotString() + "\t";
  error_message += "\n";

  EXPECT_TRUE(IsFeasibleSequence(current_sequence, candidiate_factorization,
                  other_candidate_factorizations),
               pass, error, error_message);
}

void TestInvalidConfiguration(bool* pass, string* error,
                              Factorization candidiate_factorization,
    const vector<Factorization>& other_candidate_factorizations,
    const vector<Factorization>& current_sequence) {
  //std::cout << "Running TestInvalidConfiguration" << std::endl;  // Print debugging.
  string error_message;
  error_message += "Function: TestInvalidConfiguration\n";
  error_message += "Candidate: " + candidiate_factorization.ToDotString()
                   + "\n";
  error_message += "\nCurrent Sequence: ";
  for (Factorization f : current_sequence)
    error_message += f.ToDotString() + "\t";
  error_message += "\nOther Candidate factorizations: ";
  for (Factorization f : other_candidate_factorizations)
    error_message += f.ToDotString() + "\t";
  error_message += "\n";

  EXPECT_FALSE(IsFeasibleSequence(current_sequence, candidiate_factorization,
                   other_candidate_factorizations),
               pass, error, error_message);
}

string ConstraintCoefficientsErrorMessage(vector<pair<int, double>> expected,
                                          vector<pair<int, double>> actual,
                                          Factorization f1, Factorization f2) {
  string error_message = "Failed TestGetConstraintCoefficients\n";
  error_message += "Failed for factorizations " + f1.ToDotString()
                   + " and " + f2.ToDotString() + "\n"; 
  error_message += "Expected: ";
  for (auto p : expected) {
    error_message += to_string(p.second) + "x" + to_string(p.first) + " + ";
  }
  error_message += "\nActual: ";
  for (auto p : actual) {
    error_message += to_string(p.second) + "x" + to_string(p.first) + " + ";
  }
  error_message += "\n";
  return error_message;
}

bool TestGetConstraintCoefficients(string* error) {
  bool pass = true;
  string error_message;
  // This test uses the natural numbers.
  Factorization f1;               // 1:         {(0,0)}
  Factorization f2(0);            // p_1:       {(0,1)}
  Factorization f3(1);            // p_2:       {(1,1)}
  Factorization f4 = f2+f2;       // p_1^2:     {(0,2)}
  Factorization f5(2);            // p_3:       {(2,1)}
  Factorization f6 = f2+f3;       // p_1 p_2:   {(0,1),(1,1)}
  Factorization f7(3);            // p_4:       {(3,1)}
  Factorization f8 = f2+f4;       // p_1^3:     {(0,3)}
  Factorization f9 = f3+f3;       // p_2^2:     {(1,2)}
  Factorization f10 = f3+f4;      // p_1 p_3:   {(0,1),(2,1)}

  vector<pair<int, double>> expected;
  expected.push_back(pair<int, double>(0, 1.0));
  vector<pair<int, double>> actual = GetConstraintCoefficients(f1, f2);
  error_message = ConstraintCoefficientsErrorMessage(expected, actual, f1, f2);
  EXPECT_TRUE(expected == actual, &pass, error, error_message);
  if (!pass) {
    return pass;
  }
  expected.clear();
  expected.push_back(pair<int, double>(0, -1.0));
  expected.push_back(pair<int, double>(1, 1.0));
  actual = GetConstraintCoefficients(f2, f3);
  error_message = ConstraintCoefficientsErrorMessage(expected, actual, f2, f3);
  EXPECT_TRUE(expected == actual, &pass, error, error_message);
  if (!pass) {
    return pass;
  }
  expected.clear();
  expected.push_back(pair<int, double>(0, 2.0));
  expected.push_back(pair<int, double>(1, -1.0));
  actual = GetConstraintCoefficients(f3, f4);
  error_message = ConstraintCoefficientsErrorMessage(expected, actual, f3, f4);
  EXPECT_TRUE(expected == actual, &pass, error, error_message);
  if (!pass) {
    return pass;
  }
  return pass;
}

bool TestSuccessCase(string* error) {
  bool pass = true;
  // This test uses the natural numbers.
  Factorization f1;               // 1:         {(0,0)}
  Factorization f2(0);            // p_1:       {(0,1)}
  Factorization f3(1);            // p_2:       {(1,1)}
  Factorization f4 = f2+f2;       // p_1^2:     {(0,2)}
  Factorization f5(2);            // p_3:       {(2,1)}
  Factorization f6 = f2+f3;       // p_1 p_2:   {(0,1),(1,1)}
  Factorization f7(3);            // p_4:       {(3,1)}
  Factorization f8 = f2+f4;       // p_1^3:     {(0,3)}
  Factorization f9 = f3+f3;       // p_2^2:     {(1,2)}

  vector<Factorization> current_sequence;
  current_sequence.push_back(f1);
  current_sequence.push_back(f2);
  current_sequence.push_back(f3);
  current_sequence.push_back(f4);
  current_sequence.push_back(f5);
  current_sequence.push_back(f6);
  current_sequence.push_back(f7);

  Factorization candidiate_factorization = f8;
  vector<Factorization> other_candidate_factorizations;
  other_candidate_factorizations.push_back(f9);
  TestValidConfiguration(&pass, error, candidiate_factorization,
                         other_candidate_factorizations,
                         current_sequence);
  return pass;
}

bool TestFailureCase(string* error) {
  //std::cout << "In function: TestFailureCase" << std::endl;  // Print debugging.
  bool pass = true;

  Factorization f1;               // 1:         {(0,0)}
  Factorization f2(0);            // p_1:       {(0,1)}
  Factorization f3(1);            // p_2:       {(1,1)}
  Factorization f4(2);            // p_3:       {(2,1)}
  Factorization f5 = f2+f2;       // p_1^2:     {(0,2)}
  Factorization f6 = f2+f3;       // p_1 p_2:   {(0,1),(1,1)}
  Factorization f7 = f3+f3;       // p_2^2:     {(1,2)}
  Factorization f8 = f2+f4;       // p_1 p_3:   {(0,1),(2,1)}
  Factorization f9 = f2+f5;       // p_1^3:     {(0,3)}
  Factorization f10 = f3+f4;      // p_2 p_3:   {(1,1),(2,1)}
  Factorization f11 = f4+f4;      // p_3^2:     {(2,2)}         Causes cycle!
  Factorization f12 = f2+f6;      // p_1^2 p_2: {(0,2),(1,1)}

  vector<Factorization> current_sequence;
  current_sequence.push_back(f1);
  current_sequence.push_back(f2);
  current_sequence.push_back(f3);
  current_sequence.push_back(f4);
  // Up to this point, no items with multiple binary products have been added.
  current_sequence.push_back(f5);
  // After p_1^2 is added, there are some items in the table (still beyond the
  // frontier) which have multiple binary products.
  current_sequence.push_back(f6);
  // At this step, p_1 p_2 is the last item in the sequence.
  // Our frontier candidates are p_1 p_3 and p_2^2.
  // Since each only require 1 binary product, either is acceptable.
  current_sequence.push_back(f7);
  current_sequence.push_back(f8);
  current_sequence.push_back(f9);
  // At this step, p_1^3 is the last item in the sequence.
  // Our frontier candidates are p_2 p_3 and p_1^2 p2.
  // Only p_2 p_3 has all of its binary products.
  current_sequence.push_back(f10);
  // At this step, p_2 p_3 is the last item in the sequence.
  // Our frontier candidates are p_1^2 p_2 and p_3^2.
  // p_1^2 p_2 has both of its binary products.
  // p_3^2 requires only one binary product.
  // Analysis of the full multiplication table (beyond frontier) shows that
  // adding p_3^2 before p_1^2 p2 creates a cycle.
  Factorization invalid_candidiate_factorization = f11;  // p_3^2
  vector<Factorization> other_candidate_factorizations;
  other_candidate_factorizations.push_back(f12);  // p_1^2 p_2
  TestInvalidConfiguration(&pass, error, invalid_candidiate_factorization,
                           other_candidate_factorizations,
                           current_sequence);
  // Return early if we have already failed.
  if (!pass) {
    return pass;
  }
  Factorization valid_candidiate_factorization = f12;  // p_1^2 p_2
  other_candidate_factorizations.pop_back();
  other_candidate_factorizations.push_back(f11);  // p_3^2
  TestValidConfiguration(&pass, error, valid_candidiate_factorization,
                         other_candidate_factorizations,
                         current_sequence);
  return pass;
}

bool TestLinearProgramming(string* error) {
  bool pass = TestGetConstraintCoefficients(error);
  if (!pass) {
    return pass;
  }
  pass = TestSuccessCase(error);
  if (!pass) {
    return pass;
  }
  pass = TestFailureCase(error);
  return pass;
}

}  // namespace Platt

#endif /* TEST_LINEAR_PROGRAMMING_H_ */


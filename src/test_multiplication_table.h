/*
 * test_multiplication_table.h
 *
 *  Created on: Nov 7, 2013
 *      Author: Devin
 *
 *  A test for the MultiplicationTable class.
 */

#ifndef TEST_MULTIPLICATION_TABLE_H_
#define TEST_MULTIPLICATION_TABLE_H_

#include <vector>
#include <algorithm>
#include "multiplication_table.h"
#include "test_utils.h"
using std::vector;
using std::sort;

namespace Platt {

bool LessByFactors(const Candidate& lhs, const Candidate& rhs) {
  return Factorization::LexicographicallyLess(
             lhs.GetFactors(), rhs.GetFactors());
}

bool CandidateVectorsAreEqual(/*const*/ vector<Candidate>& lhs,
                              /*const*/ vector<Candidate>& rhs) {
  sort(rhs.begin(), rhs.end(), LessByFactors);
  sort(lhs.begin(), lhs.end(), LessByFactors);
  if (rhs.size() != lhs.size())
    return false;
  for (size_t i = 0; i < lhs.size(); ++i) {
    if (rhs[i] != lhs[i])
      return false;
  }
  return true;
}

void TestConfiguration(bool* pass, string* error,
                       /*const*/ vector<Candidate>& expected_candidates,
                       /*const*/ vector<Candidate>& actual_candidates) {
    string error_message;
    error_message += "\nexpected candidates:\n";
    for (Candidate c: expected_candidates)
      error_message += c.DebugString();
    error_message += "actual candidates:\n";
    for (Candidate c: actual_candidates)
        error_message += c.DebugString();

    EXPECT_TRUE(CandidateVectorsAreEqual(
        actual_candidates, expected_candidates), pass, error,
               error_message);
}

bool TestMultiplicationTable(string* error) {
  *error = "";
  bool pass = true;
  MultiplicationTable table;
  Factorization f0;             // {(0,0)}
  Factorization f1(0);          // {(0,1)}
  Factorization f01(1);         // {(1,1)}
  Factorization f2 = f1+f1;     // {(0,2)}
  Factorization f001(2);        // {(2,1)}
  Factorization f11 = f1+f01;   // {(0,1),(1,1)}
  //
  Factorization f3 = f2+f1;     // {(0,3)}
  Factorization f02 = f01+f01;  // {(1,2)}
  Factorization f101 = f1+f001; // {(0,1),(2,1)}
  //
  Factorization f21 = f2+f01;   // {(0,2),(1,1)}

  Candidate four;
  four.AddEntry(Tuple(1,0));  // First item on second row
  four.SetFactors(f2);

  Candidate six;
  six.AddEntry(Tuple(1,1));   // Second item on second row
  six.SetFactors(f11);

  Candidate eight;
  eight.AddEntry(Tuple(1,2)); // Third item on second row
  eight.SetFactors(f3);

  Candidate nine;
  nine.AddEntry(Tuple(2,0));  // First item on third row
  nine.SetFactors(f02);

  Candidate ten;
  ten.AddEntry(Tuple(1,3));   // Fourth item second row
  ten.SetFactors(f101);

  Candidate twelve;
  twelve.AddEntry(Tuple(1,4));   // Fifth item on second row
  twelve.AddEntry(Tuple(2,1));   // Second item on third row
  twelve.SetFactors(f21);

  vector<Candidate> expected_candidates;
  vector<Candidate> actual_candidates;

  // Add the number 3 to the table
  table.PushPrime();
  expected_candidates.push_back(four);
  actual_candidates = table.GetCandidates();
  TestConfiguration(&pass, error, expected_candidates, actual_candidates);

  // Add the number 4 to the table
  table.PushComposite(four);
  expected_candidates.clear();
  expected_candidates.push_back(six);
  actual_candidates = table.GetCandidates();
  TestConfiguration(&pass, error, expected_candidates, actual_candidates);

  // Add the number 5 to the table
  table.PushPrime();
  actual_candidates = table.GetCandidates();
  TestConfiguration(&pass, error, expected_candidates, actual_candidates);

  // Add the number 6 to the table
  table.PushComposite(six);
  expected_candidates.clear();
  expected_candidates.push_back(eight);
  expected_candidates.push_back(nine);
  actual_candidates = table.GetCandidates();
  TestConfiguration(&pass, error, expected_candidates, actual_candidates);

  // Add the number 7 to the table
  table.PushPrime();
  actual_candidates = table.GetCandidates();
  TestConfiguration(&pass, error, expected_candidates, actual_candidates);

  // Add the number 8 to the table
  table.PushComposite(eight);
  expected_candidates.clear();
  expected_candidates.push_back(nine);
  expected_candidates.push_back(ten);
  actual_candidates = table.GetCandidates();
  TestConfiguration(&pass, error, expected_candidates, actual_candidates);

  // Add the number 9 to the table
  table.PushComposite(nine);
  expected_candidates.clear();
  expected_candidates.push_back(ten);
  actual_candidates = table.GetCandidates();
  TestConfiguration(&pass, error, expected_candidates, actual_candidates);

  // Add the number 10 to the table
  table.PushComposite(ten);
  expected_candidates.clear();
  expected_candidates.push_back(twelve);
  actual_candidates = table.GetCandidates();
  TestConfiguration(&pass, error, expected_candidates, actual_candidates);

  return pass;
}

}  // namespace Platt

#endif /* TEST_MULTIPLICATION_TABLE_H_ */

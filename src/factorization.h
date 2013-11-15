/*
 * factorization.h
 *
 *  Created on: Nov 5, 2013
 *      Author: Devin
 *
 *  This is the way the unique prime factorizations of an integer are stored
 *  in the program. The Factorization class is used as the data for nodes in
 *  the tree.
 *
 *  We have chosen to store factorizations as sets of (prime, exponent) pairs,
 *  which we call "Tuples" (this is a typedef of
 *  std::pair<unsigned int, unsigned int>). Another option would have been to
 *  store factorizations as exponent vectors (using std::vector<unsigned int>),
 *  but I have tried both implementations in the past and the Tuple method had
 *  slightly better performance in terms of time and space.
 *
 *  We present a table with examples corresponding to the natural numbers:
 *
 *  number  | ToDotString() | ToSerialString  |
 *  10        {(0,1),(2,1)}   0,1|2,1
 *  24        {(0,3),(1,1)}   0,3|1,1
 */

#ifndef FACTORIZATION_H_
#define FACTORIZATION_H_

#include <string>
#include <vector>
#include <utility>
using std::string;
using std::vector;
using std::pair;

namespace Platt {

// Note that "std::tuple" is a type!
// A Tuple is a (prime, exponent) pair.
typedef pair<unsigned int, unsigned int> Tuple;
bool LexicographicallyLess(const Tuple& lhs, const Tuple& rhs);

class Factorization {
 private:
  // The tuples are ordered by lower primes to higher primes.
  // This fact is depended on by the implementations of operator+, operator==
  // and LexicographicallyLess.
  vector<Tuple> factors;

  // Because C++ isn't Python
  vector<Tuple> SplitAndConvert(const string &s);

 public:
  // The default constructor sets factorization to the identity.
  Factorization();
  Factorization(unsigned int);
  Factorization(vector<Tuple>);
  Factorization(string serial_string);
  Factorization(const Factorization&);
  const Factorization& operator=(const Factorization&);
  Factorization operator+(const Factorization&) const;
  string ToDotString() const;
  string ToSerialString() const;
  bool IsPrime() const;
  bool IsIdentity() const;
  unsigned int RequiredCount() const;
  // Used by the std::less function templated for Factorization, so that
  // Factorization can be used in std associative containters. See the .cpp
  // file for more details.
  static bool LexicographicallyLess(const Factorization& lhs,
                                    const Factorization& rhs);
  // Do not confuse this operator for the actual ordering of factorizations in
  // a Beurling generalized prime system. This is a lexicographical ordering
  // used for std associative containers. I couldn't get a std::less template
  // overload to work, so I'm overloading the < operator instead.
  bool operator< (const Factorization& rhs) const;
  bool operator== (const Factorization& rhs) const;
  bool operator!= (const Factorization& rhs) const;
};

}  // namespace Platt

#endif  /* FACTORIZATION_H_ */

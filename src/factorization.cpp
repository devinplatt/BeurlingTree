/*
 * factorization.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: Devin
 *
 *  Implementation of the Factorization class.
 */

#include "factorization.h"
#include "compatibility.h"
#include <algorithm>
using std::min;

namespace Platt {

bool LexicographicallyLess(const Tuple& lhs, const Tuple& rhs) {
  if (lhs.first < rhs.first)
    return true;
  else if (lhs.first == rhs.first && lhs.second < rhs.second)
    return true;
  return false;
}

// Constructors should set local_type_is_vector.
Factorization::Factorization() {
  factors.push_back(Tuple(0,0));
}

Factorization::Factorization(unsigned int prime) {
  factors.assign(1,Tuple(prime,1));
}

Factorization::Factorization(vector<Tuple> tuples) {
  factors = tuples;
}

vector<Tuple> Factorization::SplitAndConvert(const string &s) {
  vector<string> first_split;
  stringstream ss1(s);
  string item;
  while (getline(ss1, item, '|')) {
    first_split.push_back(item);
  }

  vector<Tuple> second_split;
  for (size_t i = 0; i < first_split.size(); ++i) {
    stringstream ss2(first_split[i]);
    string item;

    getline(ss2, item, ',');
    unsigned int first = stoul(item);

    getline(ss2, item, ',');  // goes to end of item
    unsigned int second = stoul(item);

    second_split.push_back(Tuple(first,second));
  }

  return second_split;
}

Factorization::Factorization(string serial_string) {
  factors = SplitAndConvert(serial_string);
}

Factorization::Factorization(const Factorization& f) {
  factors = f.factors;
}

const Factorization& Factorization::operator=(const Factorization& f) {
  factors = f.factors;
  return f;
}

// Implementation depends on the sorting of factors.
Factorization Factorization::operator+(const Factorization& f) const{
  vector<Tuple> sum;
  size_t i = 0;
  size_t j = 0;
  // Go through both factorizations and add them to the sum, in order.
  while(i < factors.size() && j < f.factors.size()) {
    if(factors[i].first < f.factors[j].first) {
      sum.push_back(factors[i]);
      i++;
    } else if (factors[i].first == f.factors[j].first) {
      sum.push_back(Tuple(factors[i].first,
                          factors[i].second + f.factors[j].second));
      i++;
      j++;
    } else {  // factors[i].first > tf.factors[j].first
      sum.push_back(f.factors[j]);
      j++;
    }
  }
  // Once we go beyond the bounds of one of the vectors we finish with the
  // other vector. At least one of these loops doesn't commence.
  while (i < factors.size()) {
    sum.push_back(factors[i]);
    i++;
  }
  while (j < f.factors.size()) {
    sum.push_back(f.factors[j]);
    j++;
  }
  return sum;  // implicit conversion
}

string Factorization::ToDotString() const {
  string out;
  out += "{";
  for(size_t i = 0; i < factors.size(); ++i) {
    out += "(" + to_string(factors[i].first + 1) // Change to 1-based indexing
           + "," + to_string(factors[i].second)
           + ")";
    if(i+1 < factors.size())
      out += ",";
  }
  out += "}";
  return out;
}

string Factorization::ToSerialString() const {
  string out;
  for(size_t i = 0; i < factors.size(); ++i) {
    out += to_string(factors[i].first)
           + "," + to_string(factors[i].second);
    if(i + 1 < factors.size())
      out += "|";
  }
  return out;
}

bool Factorization::IsPrime() const {
  return (factors.size() == 1 && factors[0].second == 1);
}

bool Factorization::IsPrimePower() const {
  return (factors.size() == 1);
}

bool Factorization::IsIdentity() const {
  return (factors.size() == 1 && factors[0].first == 0
          && factors[0].second == 0);
}

/*
Required Count returns the number of "binary products" associated with a given
Factorization. We disregard products that have the identity as one of the
arguments.

Examples of output:

  Factorization as  Output of         Natural Number
  Exponent Vector   Required Count    Example
  <1,1,0>           =>  1             (6: 3*2)
  <1,2,0>           =>  2             (18: 2*9,3*6)
  <2,1,0>           =>  2             (12: 2*6,3*4) (symmetric to previous)
  <3,1,0>           =>  3             (24: 2*12,3*8,4*6)
  <2,2,0>           =>  4             (36: 2*18,3*12,4*8,6*6)
  <3,2,0>           =>  5             (72: 2*36,3*24,4*18,6*12,8*9)
  <4,1,0>           =>  4             (48: 2*24,3*16,4*12,6*8)

  <1,1,1>           =>  3             (30: 2*15,3*10,5*6)
  <2,1,1>           =>  5             (60: 2*30,3*20,4*15,5*12,6*10)
  <2,2,1>           =>  8      (180: 2*90,3*60,4*45,5*36,6*30,9*20,10*18,12*15)
  <3,1,1>           =>  7            (120: 2*60,3*40,4*30,5*24,6*20,8*15,10*12)

  <1,0,0>            => 0             (2: none)
  <2,0,0>            => 1             (4: 2*2)
  <3,0,0>            => 1             (8: 2*4)
  <4,0,0>            => 2             (16: 2*8,4*4)
  <5,0,0>            => 2             (32: 2*16,4*8)
  <6,0,0>            => 3             (64: 2*32,4*16,8*8)

Given a factorization f, each prime has a frequency,
ie.
  Given
          natural number  | exponent vector | tuples
    1.    12                <2,1>             {(1,2),(2,1)}
    2.    24                <3,1>             {(1,3),(2,1)}
    3.    30                <1,1,1>           {(1,1),(2,1),(3,1)}

  For the first example:
    The first prime has a frequency of 2
    The second prime has a frequency of 1

  For the second example:
    The first prime has a frequency of 3
    The second prime has a frequency of 1

  For the third example:
    The first, second, and third primes all have a frequency of 1

We use these frequencies to determine the required number of binary products
-- combinations of two numbers that yield the given factorization.

The Algorithm:

  let count = 1
  for each prime frequency pf:
    count = count * (pf+1)

At this point count equals the number of all the "subsets" of factorization f.
For example, for the factorization <2,2> (in exponent vector form)
count = 9, since the "subsets" of <2,2> are (in lexicographical order):
<0,0>, <0,1>, <0,2>, <1,0>, <1,1>, <1,2>, <2,0>, <2,1>, <2,2>

Note that we can pair each "subset" with another to get the factorization f.
In some cases (like with f = <2,2>) there will be one subset we pair with
itself (e.g <1,1> and <1,1> makes <2,2>). To get the required count we
effectively wish to count these pairs.

We also wish to disregard the pair containing the identity (eg. <0,0>).

Thus if count is even at this point we want the value (count-2)/2,
and if count is odd we want the value (count-1)/2.

  count = count - 1
  count = count / 2, rounded down

  return count
*/
unsigned int Factorization::RequiredCount() const {
  // Algorithm:
  //   let count = 1
  //   for each prime frequency pf:
  //     count = count * (pf+1)
  //   count = count - 1
  //   count = count / 2, rounded down
  //   return count

  unsigned int count = 1;
  for(Tuple t : factors) {
    count *= t.second+1;
  }

  count--;
  count /= 2;
  return count;
}

// Our lexicographical ordering is defined so that:
// 1. The identity is the first element in the ordering.
// 2. Otherwise The factorization with the lowest power of the lowest prime
// comes first in the ordering, including the zero power.
// 3. If the two factorizations are the same except that one has more Tuples
//    the factorization with less Tuples comes first.
// This implementation depends on the sorting of factors.
bool Factorization::LexicographicallyLess(const Factorization& lhs,
                                          const Factorization& rhs) {
  vector<Tuple> lfactors = lhs.factors;
  vector<Tuple> rfactors = rhs.factors;
  if(rhs.IsIdentity())
    return false;
  else if(lhs.IsIdentity())
    return true;
  size_t i = 0;
  while(i < min(lfactors.size(), rfactors.size())) {
    if(lfactors[i].first < rfactors[i].first) {
      return false;
    } else if (lfactors[i].first == rfactors[i].first) {
      if (lfactors[i].second < rfactors[i].second)
        return true;
      else if (lfactors[i].second > rfactors[i].second)
        return false;
      // otherwise lfactors[i] == rfactors[i]
      i++;
    } else {  // factors[i].first > tf.factors[j].first
      return true;
    }
  }
  // If we didn't return out of the while loop, the lhs and rhs must have the
  // same values up to the minimum size of the two.
  if (lfactors.size() < rfactors.size())
    return true;
  return false;
}

bool Factorization::operator< (const Factorization& rhs) const {
  return LexicographicallyLess(*this, rhs);
}

// Depends on sorting of factors
bool Factorization::operator== (const Factorization& rhs) const {
  if (factors.size() != rhs.factors.size())
    return false;
  for (size_t i = 0; i < factors.size(); ++i) {
    if (factors[i].first != rhs.factors[i].first
        || factors[i].second != rhs.factors[i].second)
      return false;
  }
  return true;
}

bool Factorization::operator!= (const Factorization& rhs) const {
  return !(*this == rhs);
}

}  // namespace Platt

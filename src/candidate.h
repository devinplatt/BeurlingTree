/*
 * candidate.h
 *
 *  Created on: Nov 7, 2013
 *      Author: Devin
 *
 *  Declares the Candidate class. This class facilitates an interface between
 *  the table and the tree. The Factorizaion is used by both the table and the
 *  table and the tree. The entries are used by just the table.
 *
 *  The implementation is inlined because it is short.
 */

#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <vector>
#include <algorithm>
#include "factorization.h"
#include "compatibility.h"
using std::vector;
using std::sort;

namespace Platt {

class Candidate {
 private:
  Factorization factors;
  vector< Tuple > entries;

 public:
  Candidate() {}
  Candidate(const Factorization& f, const Tuple& t) {
    factors = f; entries.push_back(t);
  }
  Candidate(const Candidate& c) {entries = c.entries; factors = c.factors;}
  void SetFactors(const Factorization& f) {factors = f;}
  // TODO: Decide on which of the following three to actually use. Discard the
  // others.
  void SetEntries(const vector<Tuple>& in) {entries = in;}
  void AddEntry(unsigned int i, unsigned int j) {
    entries.push_back(Tuple(i,j));
  }
  void AddEntry(Tuple t) {
    entries.push_back(t);
  }

  Factorization GetFactors() const {return factors;}
  vector<Tuple> GetEntries() const {return entries;}

  bool operator== (/*const*/ Candidate& rhs) /*const*/ {
    if (factors != rhs.factors)
      return false;
    if (entries.size() != rhs.entries.size())
      return false;
    sort(entries.begin(), entries.end(), LexicographicallyLess);
    sort(rhs.entries.begin(), rhs.entries.end(), LexicographicallyLess);
    for (size_t i = 0; i < entries.size(); ++i) {
      if (entries[i].first != rhs.entries[i].first
          || entries[i].second != rhs.entries[i].second)
        return false;
    }
    return true;
  }
  bool operator!= (/*const*/ Candidate& rhs) /*const*/ {
      return !(*this == rhs);
  }
  string DebugString() const {
    string out;
    out += "Factorization: " + factors.ToDotString() + "\n";
    out += "Cells:\n";
    for (Tuple t : entries) {
      out += "(" + to_string(t.first) + "," + to_string(t.second) + ") ";
    }
    out += "\n";
    return out;
  }
};

}  // namespace Platt

#endif /* CANDIDATE_H_ */

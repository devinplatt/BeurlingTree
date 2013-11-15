/*
 * multiplication_table.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Devin
 *
 *  Defines the MultiplicationTable class.
 */

#include <map>
#include <set>
#include "multiplication_table.h"
#include "candidate.h"
using std::multimap;
using std::set;

namespace Platt {

/*
Algorithm for determining "frontier" cells of the multiplication table:

  let rIndices be the list of rightmost row indices of the table

  for(each row r in rIndices besides the first row r=0)

    let c = rIndices[r]+r, ie. it is the column index in the table with the
    offset added
    let cAbove = rIndices[r-1]+r-1, the column index for the row above

    if (c < cAbove)
      add (r,c+1) to the frontier

  // Determine if a new row can be added to rIndices, that is, if the first
  // cell of the next row (this is on the diagonal) is in the frontier.
  let r = max index of rIndices
  let cAbove = rIndices[r]+r
  if( r+1 <= cAbove )
    add (r+1,r+1) to the frontier

  return the frontier
*/
vector<Tuple> MultiplicationTable::GetFrontier() const {
  vector<Tuple> frontier;
  for (size_t i = 1; i < table.size(); ++i) {
    if (table[i].size() < table[i-1].size() -1)
      frontier.push_back(Tuple(i, table[i].size()));
  }
  if (table.back().size() >= 2)
    frontier.push_back(Tuple(table.size(),0));
  return frontier;
}

// Sets prime_count to 0. Initializes table.
MultiplicationTable::MultiplicationTable() {
  table.push_back(vector<Cell>());
  table[0].push_back(Cell(0,Factorization()));  // Identity
  table[0].push_back(Cell(1,Factorization(0))); // First Prime
  prime_count = 1;
}

/*
Algorithm for determining the children of a given node:

1.

Determine the "frontier" cells of the multiplication table. The frontier is
the set of cells that touch the boundary of the existing cells in the table and
follow a certain rule:

  - There must exist a cell in the same column in the row above it.

This rule enforces the rule of multiplication that larger numbers lead to
greater products, ie. translation-invariance.

The frontier is a starting list of candidate cells which can be filled in to
the table in this particular invocation of the algorithm. Note that the cell to
the right of the rightmost cell in the first row is not included in the
frontier; it is always added to the table since the first row represents
multiplication by the identity.

2.

We have a multimap of candidates, where the key is a factorization and the value
is a row, column pair (i,j) indicating a cell where this factorization would be
present.

We also have the set of keys (factorizations).

In this step we take the frontier cells and add them to the multimap of
candidates. This collapses the set of cells so that all cells with the same
factorization are collected together.

for each cell in frontier
  add cell to candidates multimap
  (add cell's key to the set of keys, which does not store duplicates)

3.

We now must remove candidates that do not have "enough" cells given their
factorization. For example, the factorization (2,1) should have two cells
associated with it:
  the cell corresponding to (2,0) * (0,1)
  the cell corresponding to (1,0) * (1,1)
If we have just one cell in the frontier for this factorization we are not
allowed to add this factorization to the table yet. Implicitly the missing cell
means that there is some other factorization f' which is considered "less" than
this factorization f, and f' is not yet in the table. Essentially, by counting
cells and making sure that they meet this requirement we are checking the
recursive rule of our partial ordering. The table acts as a memoization
technique for "remembering" the recursive rules of this partial ordering.

let keys_to_erase be the list of keys of candidates to remove from the multimap of candidates

for each factorization f in keys
  let required_count = RequiredCount(f)
  let current_count = count of elements in candidates with key f

  if current_count < required_count
    add f to keys_to_erase

for each factorization f in keys_to_erase
  remove elements of candidates with key f
  (remove key f from keys)

4.

Return candidates.
*/
vector<Candidate> MultiplicationTable::GetCandidates() const {
  vector<Candidate> final_candidates;
  multimap<Factorization, Tuple> candidates;
  set<Factorization> keys;

  // 1. Determine frontier cells in table
  vector<Tuple> frontier = GetFrontier();

  // 2. Add frontier items to candidates
  for (Tuple t : frontier) {
    // Add item with the factorization that is the product of the two numbers
    // referenced by its row and column indices
    Factorization sum = table[0][t.first].factors
                        + table[0][t.first + t.second].factors;
    candidates.insert(pair<Factorization, Tuple>(sum, t));
    keys.insert(sum);
  }

  // 3. Remove candidates
  set<Factorization> keys_to_erase;

  for (Factorization f : keys) {
    unsigned int required_count = f.RequiredCount();
    unsigned int current_count = candidates.count(f);
    if (current_count < required_count)
      keys_to_erase.insert(f);
  }

  for (Factorization f : keys_to_erase) {
    candidates.erase(f);
    keys.erase(f);
  }

  // Convert Multimap to vector
  for (Factorization f : keys) {
    Candidate c;
    c.SetFactors(f);
    auto range = candidates.equal_range(f);
    for (auto it = range.first; it != range.second; it++)
      c.AddEntry(it->second);
    final_candidates.push_back(c);
  }

  return final_candidates;
}

unsigned int MultiplicationTable::GetPrimeCount() const {
  return prime_count;
}

// Note that the case  where a new row needs to be added to the table needs to
// be watched for. We can't assume that the row exists. If one of the Candidates
// has 0 for its accessor column index then a new row may need to be added.
void MultiplicationTable::PushComposite(const Candidate& c) {
  unsigned int next_number = table[0].size();
  Factorization f = c.GetFactors();
  // Add item to first row.
  table[0].push_back(Cell(next_number, f));
  // Add the rest of the entries.
  vector<Tuple> entries = c.GetEntries();
  for (Tuple e : entries) {
    while (e.first >= table.size())
      table.push_back(vector<Cell>());
    table[e.first].push_back(Cell(next_number, f));
  }
}

void MultiplicationTable::PopComposite(const Candidate& c) {
  table[0].pop_back();
  vector<Tuple> entries = c.GetEntries();
  for (Tuple e : entries) {
    table[e.first].pop_back();
  }
  // We use the size of table (the number of rows) at other points in the
  // algorithm. If there are empty rows we must delete them. Due to properties
  // of the table, we should not have to worry about deleting a row in between
  // rows; this condition should in theory only occur with the last row.
  if (table.back().size() == 0)
    table.pop_back();
}

void MultiplicationTable::PushPrime() {
  table[0].push_back(Cell(table[0].size(),Factorization(prime_count)));
  prime_count++;
}

void MultiplicationTable::PopPrime() {
  prime_count--;
  table[0].pop_back();
}

string MultiplicationTable::DebugString() const {
  string out;
  out += "Printing table debug string\n";
  for (size_t i = 0; i < table.size(); ++i) {
    for (size_t j = 0; j < table[i].size(); ++j) {
      out += "|" + table[i][j].factors.ToDotString() + "|";
    }
    out += "\n";
  }
  out += "\n";
  return out;
}

}  // namespace Platt

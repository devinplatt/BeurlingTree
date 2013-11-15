/*
 * multiplication_table.h
 *
 *  Created on: Nov 7, 2013
 *      Author: devin
 */

#ifndef MULTIPLICATION_TABLE_H_
#define MULTIPLICATION_TABLE_H_

#include <vector>
#include "candidate.h"
using std::vector;

namespace Platt {

  // Since our program is in C++, we used 0-based indexing.

  // As we have a multiplication table, each cell is a number. Each cell (i,j)
  // can be considered as the "product" of the numbers in cells (0,i) and (0,j).

  // The first row of the table corresponds to multiplication by the identity
  // element, so any number that is in a cell in the table must necessarily
  // appear in the first row.

  // Cells in the table contains two members:
  // - A "position" indicating the index of the number in the first row.
  // - The unique prime factorization of the number in the Beurling generalized
  // integer system.
  struct Cell {
    unsigned int position;
    Factorization factors;
    Cell(): position(), factors() {}
    Cell(unsigned int p, Factorization f): position(p), factors(f) {}
  };

class MultiplicationTable {
 private:

  // The multiplication table is dynamically allocated; it grows in size over
  // time. It is a subset of an upper-right triangular matrix (including the
  // diagonal). There is no need to store the lower triangular part since
  // multiplication is symmetric. We consider cells (i,j) to be invalid if
  // i < j.
  //
  // It is important to note the indexing of the table:
  //  table[x][y] corresponds to cell (x,y+x) in the table.
  //
  // An important property of the table is that the length of any row is less
  // than or equal to the length of the row above it, including the offset. That
  // is to say:
  //  (i,j) can only be in the table if (i-1,j) exists (for i > 0)
  //  table[x][y] can only be in the table if table[x-1][y+x] exists (for x > 0)
  // This property stems as a result of how the algorithm in the GetCandidates
  // function chooses candidates for additions to the table.
  vector< vector<Cell> > table;

  // Example:
  // The following is an example using the natural numbers as our integer
  // system. We use the exponent vector form of factorization (with arrays of
  // length two for formatting convenience only). Please note that this is just
  // a "cut-away" view of the table, not an actual state during the flow of the
  // algorithm, since there are values filled in which aren't yet occuring in
  // the top row.
  //
  //       0       1       2       3        ...
  //   --------------------------------------
  // 0 |0, [0,0]|1, [1,0]|2, [0,1]|3, [2,0]|
  //   --------------------------------------
  // 1 |        |3, [2,0]|5, [1,1]|7, [3,0]|
  //   --------------------------------------
  // 2 |        |        |8, [0,2]|11,[2,1]|
  //   --------------------------------------
  // 3 |        |        |        |15,[4,0]|
  //   --------------------------------------
  //
  // Examples of the state of the table during steps of the algorithm, if
  // choices of the candidate cell are consistent with the natural numbers:
  //
  // 1. Initialization
  //       0       1
  //   -------------------
  // 0 |0, [0,0]|1, [1,0]|
  //   -------------------
  //
  // 2.
  //       0       1       2
  //   ----------------------------
  // 0 |0, [0,0]|1, [1,0]|2, [0,1]|
  //   ----------------------------
  //
  // 3.
  //       0       1       2       3
  //   -------------------------------------
  // 0 |0, [0,0]|1, [1,0]|2, [0,1]|3, [2,0]|
  //   -------------------------------------
  // 1          |3, [2,0]|
  //            ----------
  //
  // 4.
  //        0          1          2          3          4
  //   --------------------------------------------------------
  // 0 |0, [0,0,0]|1, [1,0,0]|2, [0,1,0]|3, [2,0,0]|4, [0,0,1]|
  //   --------------------------------------------------------
  // 1            |3, [2,0,0]|
  //              ------------
  //
  // 5.
  //        0          1          2          3          4          5
  //   -------------------------------------------------------------------
  // 0 |0, [0,0,0]|1, [1,0,0]|2, [0,1,0]|3, [2,0,0]|4, [0,0,1]|5, [1,1,0]|
  //   -------------------------------------------------------------------
  // 1            |3, [2,0,0]|5, [1,1,0]|
  //              -----------------------

  // A count of the number of primes in the table. Used for pushing and popping
  // primes.
  unsigned int prime_count;

  // Recall that table[x][y] corresponds to cell (x,y+x) = (i,j) in the table,
  // where x,y are accessor indices and i,j are table indices. In this program
  // we will always use accessor indices.

  vector<Tuple> GetFrontier() const;

 public:

  // Sets prime_count to 0. Initializes table.
  MultiplicationTable();
  vector<Candidate> GetCandidates() const;
  unsigned int GetPrimeCount() const;
  void PushComposite(const Candidate&);
  void PopComposite(const Candidate&);
  void PushPrime();
  void PopPrime();

  // For debugging:
  string DebugString() const;
};

}  // namespace Platt

#endif /* MULTIPLICATION_TABLE_H_ */

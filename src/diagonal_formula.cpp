/*
 * diagonal_formula.cpp
 *
 *  Created on: Dec 4, 2013
 *      Author: Devin
 *
 *  Defines the DiagonalFormula class.
 */

#include "diagonal_formula.h"
#include <set>
#include <algorithm>
using std::set;
using std::sort;

namespace Platt {

  DiagonalFormula::DiagonalFormula(int d) :
      RestrictedTree(d-1, d-1, 2*d-2) {
    // Build a set of all distinct subsequences of d-1 composites.
    set <FactorizationSequence> FSs;
    FactorizationSequence FS;

    // Add composites to the sequence on the way down
    auto PrechildFunctor = [&] (Node<Factorization>* N) {
      if (!(N->GetData().IsPrime()))
        FS.Push(N->GetData());
    };

    // Remove composites on the way back up.
    auto PostchildFunctor = [&] (Node<Factorization>* N) {
      if (!(N->GetData().IsPrime()))
        FS.Pop();
    };

    // Add the sequence to the set
    auto LeafFunctor = [&] (Node<Factorization>* N) {
      if (!(N->GetData().IsPrime()))
              FS.Push(N->GetData());
      FSs.insert(FS);
      if (!(N->GetData().IsPrime()))
              FS.Pop();
    };

    tree.DepthFirst(&PrechildFunctor, &PostchildFunctor, &LeafFunctor);

    // Now pass over the tree for each FactorizationSequence and determine
    // its corresponding coefficient.
    for(FactorizationSequence FS : FSs) {
      vector<Node<Factorization>*> common;
      vector<Node<Factorization>*> current;
      FactorizationSequence currentFS;

      //
      auto PrechildFunctor = [&] (Node<Factorization>* N) {
        current.push_back(N);
        if (!(N->GetData()).IsPrime())
          currentFS.Push(N->GetData());
      };


      auto PostchildFunctor = [&] (Node<Factorization>* N) {
        current.pop_back();
        if (!(N->GetData()).IsPrime())
                  currentFS.Pop();
      };

      // Add the sequence to the set
      auto LeafFunctor = [&] (Node<Factorization>* N) {
        current.push_back(N);
        if (!(N->GetData()).IsPrime())
                  currentFS.Push(N->GetData());
        // Case of first branch with the FS subsequence
        if(common.size() == 0) {
          if (currentFS == FS)
            common = current;
        } else {  // common is populated
          if (currentFS == FS) {
            // Trim common down to the common lead sequence
            size_t x = common.size()-1;
            while(current[x] != common[x]) { // equality as pointers!
              common.pop_back();
              x--;
            }
          }
        }
        current.pop_back();
        if (!(N->GetData()).IsPrime())
          currentFS.Pop();
      };

      tree.DepthFirst(&PrechildFunctor, &PostchildFunctor, &LeafFunctor);

      FSvec.push_back(FS);
      // TODO: fix this comment.
      // The binomial coefficient corresponding to FS is:
      // (?????) choose
      // (?????)
      // Our values are offset by a value of d-1 because our sequence starts
      // from row d
      // If there is just one branch for the FS, we choose all d-1 composites
      // (which occur after a string of d-1 primes). Thus we have d-1 spots to
      // fill, but the offset cancels this to 0.
      // Otherwise, we have the length of the whole branch (2*d-2) minus the
      // length of the common sequence, and this again is offset by subtracting
      // 2*d-2 - (d-1) = d-1.
      if ((int)common.size() == 2*d-2)
        n_offset.push_back(0);  // num_comp == d-1
      else
        n_offset.push_back(d-1-common.size());
      // Determine the number of composites in common.
      int num_comp = 0;
      for (Node<Factorization>* N : common) {
        if (!(N->GetData()).IsPrime())
          num_comp++;
      }
      // If there is just one branch for the FS, we choose all composites
      // (which occur after a string of d-1 primes)
      if ((int)common.size() == 2*d-2)
        k.push_back(num_comp);  // num_comp == d-1
      else
        k.push_back(d-1-num_comp);
    }

  }

  string DiagonalFormula::GetFormulaString() {
    string out;
    for(size_t x = 0; x < FSvec.size(); ++x) {
      out += FSvec[x].ToString() + ": (n + " + to_string(n_offset[x])
             + ") choose " + to_string(k[x]) + "\n";
    }
    return out;
  }

  vector <Tuple> DiagonalFormula::GetFormula() {
    vector <Tuple> out;
    for(size_t x = 0; x < k.size(); ++x)
      out.push_back(Tuple(n_offset[x], k[x]));
    sort(out.begin(), out.end(), LexicographicallyLess);
    return out;
  }

}  // namespace Platt



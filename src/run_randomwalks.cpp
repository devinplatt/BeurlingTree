//============================================================================
// Name        : run_randomwalks.cpp
// Author      : Devin Platt
// Version     :
// Copyright   : MIT License
// Description : A demonstration of the program's randomwalks.
//============================================================================

#include <time.h>
#include <iostream>
#include <limits>
using namespace std;
/*#include "test_tree.h"
#include "test_factorization.h"
#include "test_multiplication_table.h"
#include "random_walk.h"
#include "prime_power_tree.h"
#include "restricted_tree.h"
#include "diagonal_formula.h"
using namespace std;
using namespace Platt;

const unsigned int HEIGHT = 4;
const unsigned int RUNS = 1;

void RunRandomWalk(RandomWalk** tree, unsigned int height) {
  time_t begin = time(NULL);
  cout << "Building path of length " << height << endl;
  *tree = new RandomWalk(height);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Built path in " << seconds << " seconds" << endl;
}*/

//int main() {
int foo() {

  /*RandomWalk* path = nullptr;
  int number_primes[RUNS][HEIGHT+1];
  int number_prime_factors[RUNS][HEIGHT+1];
  int number_distinct_prime_factors[RUNS][HEIGHT+1];
  int number_children[RUNS][HEIGHT+1];

  for (int i = 0; i < RUNS; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      RunRandomWalk(&path, j);
      number_primes[i][j] = path->NumPrimes(j);
      number_prime_factors[i][j] = path->NumPrimeFactors(j);
      number_distinct_prime_factors[i][j] = path->NumDistinctPrimeFactors(j);
      number_children[i][j] = path->NumChildren(j);
    }
  }

  cout << "number primes: " << number_primes[RUNS-1][HEIGHT-1];
  cout << "number prime factors: " << number_prime_factors[RUNS-1][HEIGHT-1];
  cout << "number distinct prime factors: " <<
          number_distinct_prime_factors[RUNS-1][HEIGHT-1];
  cout << "number children: " << number_children[RUNS-1][HEIGHT-1];*/

    cin.ignore(numeric_limits<std::streamsize>::max(),'\n');
  return 0;
}
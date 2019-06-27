/*
 * test_random_walk.h
 *
 *  Created on: April 26, 2019
 *      Author: Devin Platt
 *
 *  A test for the RandomWalk class.
 */

#ifndef TEST_RANDOM_WALK_H_
#define TEST_RANDOM_WALK_H_

#include "random_walk.h"
#include "multiplication_table.h"
#include<string>
using std::string;

#include<iostream>
using namespace std;

namespace Platt {

bool TestRandomWalk(string* error) {

  const int HEIGHT = 50;
  const int RUNS = 10000;
  RandomWalk* path = nullptr;
  bool pass = true;
  *error = "";

  for (int run = 0; run < RUNS; run++) {
    if (run < 5 or run % 1000 == 0) { 
      cout << "run: " << run << endl;
    }
    try {
      path = new RandomWalk(HEIGHT);
    } catch (const CandidatesException& e) {
      *error +=  string("No composite candidates during TestRandomWalk\n")
                 + e.what() + "\n"
                 + "On run number: " + std::to_string(run + 1) + "\n"
                 + "Table DebugString length: " + std::to_string(e.get_table_str().length()) + "\n";
      if (e.get_table_str().length() < 1000) {
        *error += e.get_table_str();
      }
      pass = false;
      break;
    }
    delete path;
  }
  return pass;
}

}  // namespace Platt

#endif  /* TEST_RANDOM_WALK_H_ */


//============================================================================
// Name        : main.cpp
// Author      : Devin Platt
// Version     :
// Copyright   : MIT License
// Description : A demonstration of the program's functionality.
//============================================================================

#include <iostream>
#include <limits>
#include "test_tree.h"
#include "test_factorization.h"
using namespace std;
using namespace Platt;

void VerifyTest (bool pass, string test, string* error);

int main() {
  string error;

  VerifyTest(TestFactorization(&error), "Factorization test", &error);
	VerifyTest(TestTree(&error), "Tree test", &error);

	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	return 0;
}

void VerifyTest (bool pass, string test, string* error) {
  if(pass) {
    cout << test << " passes." << endl;
  } else {
    cout << test << " fails!" << endl;
    cout << "Error: " << *error << endl;
  }
}

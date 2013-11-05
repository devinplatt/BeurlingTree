//============================================================================
// Name        : beurling_tree.cpp
// Author      : Devin Platt
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <limits>
#include "test_tree.h"
using namespace std;
using namespace Platt;

void VerifyTest (bool pass, string test);

int main() {
	VerifyTest(TestTree(), "Tree test");

	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	return 0;
}

void VerifyTest (bool pass, string test) {
  if(pass)
    cout << test << " passes." << endl;
  else
    cout << test << " fails!" << endl;
}

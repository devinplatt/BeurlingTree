//============================================================================
// Name        : main.cpp
// Author      : Devin Platt
// Version     :
// Copyright   : MIT License
// Description : A demonstration of the program's functionality.
//============================================================================

#include <time.h>
#include <iostream>
#include <limits>
#include "test_tree.h"
#include "test_factorization.h"
#include "test_multiplication_table.h"
#include "integer_tree.h"
using namespace std;
using namespace Platt;

const unsigned int HEIGHT = 4;

void VerifyTest (bool pass, string test, string* error);
void RunTests();
void BuildTree(IntegerTree** tree, unsigned int height);
void ExportAsDot(IntegerTree* tree, string filename);
void SerializeTree(IntegerTree* tree, string filename);
void OpenTree(IntegerTree** serial_copy, string filename);
void PrintTriangle(IntegerTree* tree);
void ExportTriangle(IntegerTree* tree, string filename);

int main() {
  RunTests();
  IntegerTree* tree = nullptr;
  IntegerTree* serial_copy = nullptr;

  BuildTree(&tree, HEIGHT);
  ExportAsDot(tree, "tree.dot");
  PrintTriangle(tree);
  ExportTriangle(tree, "triangle.txt");
  SerializeTree(tree, "serial.txt");
  OpenTree(&serial_copy, "serial.txt");
  ExportAsDot(serial_copy, "serial.dot");
  PrintTriangle(serial_copy);

  if (tree) delete tree;
  if (serial_copy) delete serial_copy;

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

void RunTests() {
  string error;
  VerifyTest(TestFactorization(&error), "Factorization test", &error);
  VerifyTest(TestTree(&error), "Tree test", &error);
  VerifyTest(TestMultiplicationTable(&error), "MultiplicationTable test", &error);
}

void BuildTree(IntegerTree** tree, unsigned int height) {
  time_t begin = time(NULL);
  cout << "Building Tree to height " << height << endl;
  *tree = new IntegerTree(height);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Built Tree in " << seconds << " seconds" << endl;
}

void ExportAsDot(IntegerTree* tree, string filename) {
  time_t begin = time(NULL);
  cout << "Exporting " << filename << endl;
  tree->ExportAsDot(filename);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Exported to " << filename << " in " << seconds << " seconds" << endl;
}

void SerializeTree(IntegerTree* tree, string filename){
  time_t begin = time(NULL);
  cout << "Serializing..." << endl;
  tree->SerializeToFile(filename);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Serialized Tree in " << seconds << " seconds" << endl;
}

void OpenTree(IntegerTree** serial_copy, string filename) {
  time_t begin = time(NULL);
  cout << "Opening..." << endl;
  *serial_copy = new IntegerTree(filename);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Opened " << filename << " in " << seconds << " seconds" << endl;
}

void PrintTriangle(IntegerTree* tree) {
  cout << "Printing triangle" << endl;
  vector< vector<unsigned int> > triangle = tree->GetTriangle();
  for (size_t i = 0; i < triangle.size(); ++i) {
    for (size_t j = 0; j < triangle[i].size(); ++j) {
      cout << triangle[i][j] << "\t";
    }
    cout << endl;
  }
  cout << endl;
}

void ExportTriangle(IntegerTree* tree, string filename) {
  cout << "Exporting triangle" << endl;
  ofstream out(filename);
  vector< vector<unsigned int> > triangle = tree->GetTriangle();
  for (size_t i = 0; i < triangle.size(); ++i) {
    for (size_t j = 0; j < triangle[i].size(); ++j) {
      out << triangle[i][j] << "\t";
    }
    out << endl;
  }
  out.close();
}

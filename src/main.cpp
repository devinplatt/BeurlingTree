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
#include "prime_power_tree.h"
using namespace std;
using namespace Platt;

const unsigned int HEIGHT = 10;

void VerifyTest (bool pass, string test, string* error);
void RunTests();
void BuildTree(IntegerTree** tree, unsigned int height);
void BuildTree(PrimePowerTree** tree, unsigned int height);
void ExportAsDot(BeurlingTreeBase* tree, string filename);
void SerializeTree(BeurlingTreeBase* tree, string filename);
void OpenTree(IntegerTree** serial_copy, string filename);
void OpenTree(PrimePowerTree** serial_copy, string filename);
void PrintTriangle(BeurlingTreeBase* tree);
void ExportTriangle(BeurlingTreeBase* tree, string filename);
void DemoIntegerTree();
void DemoPrimePowerTree();

int main() {
  RunTests();
  //DemoIntegerTree();
  //DemoPrimePowerTree();
  PrimePowerTree* tree = nullptr;
  BuildTree(&tree, HEIGHT);
  ExportTriangle(tree, "primepower_triangle.txt");
  if (tree) delete tree;

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

void BuildTree(PrimePowerTree** tree, unsigned int height) {
  time_t begin = time(NULL);
  cout << "Building Tree to height " << height << endl;
  *tree = new PrimePowerTree(height);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Built Tree in " << seconds << " seconds" << endl;
}

void ExportAsDot(BeurlingTreeBase* tree, string filename) {
  time_t begin = time(NULL);
  cout << "Exporting " << filename << endl;
  tree->ExportAsDot(filename);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Exported to " << filename << " in " << seconds << " seconds" << endl;
}

void SerializeTree(BeurlingTreeBase* tree, string filename){
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

void OpenTree(PrimePowerTree** serial_copy, string filename) {
  time_t begin = time(NULL);
  cout << "Opening..." << endl;
  *serial_copy = new PrimePowerTree(filename);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Opened " << filename << " in " << seconds << " seconds" << endl;
}

void PrintTriangle(BeurlingTreeBase* tree) {
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

void ExportTriangle(BeurlingTreeBase* tree, string filename) {
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

void DemoIntegerTree() {
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
}

void DemoPrimePowerTree() {
  PrimePowerTree* tree = nullptr;
  PrimePowerTree* serial_copy = nullptr;

  BuildTree(&tree, HEIGHT);
  ExportAsDot(tree, "primepower.dot");
  PrintTriangle(tree);
  ExportTriangle(tree, "primepower_triangle.txt");
  SerializeTree(tree, "primepower_serial.txt");
  OpenTree(&serial_copy, "primepower_serial.txt");
  ExportAsDot(serial_copy, "primepower_serial.dot");
  PrintTriangle(serial_copy);

  if (tree) delete tree;
  if (serial_copy) delete serial_copy;
}

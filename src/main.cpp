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
#include <string>
#include "test_tree.h"
#include "test_factorization.h"
#include "test_linear_programming.h"
#include "test_multiplication_table.h"
#include "test_random_walk.h"
#include "integer_tree.h"
#include "prime_power_tree.h"
#include "restricted_tree.h"
#include "diagonal_formula.h"
#include "random_walk.h"
using namespace std;
using namespace Platt;

const unsigned int RUNS = 1;
const unsigned int HEIGHT = 250;
// The following two constants are used for RestrictedTree.
const int MAX_PRIMES = 3;
const int MAX_COMPOSITES = 3;

void VerifyTest (bool pass, string test, string* error);
void RunTests();
void BuildTree(IntegerTree** tree, unsigned int height);
void BuildTree(PrimePowerTree** tree, unsigned int height);
void BuildTree(RestrictedTree** tree, unsigned int height);
void ExportAsDot(BeurlingTreeBase* tree, string filename);
void SerializeTree(BeurlingTreeBase* tree, string filename);
void OpenTree(IntegerTree** serial_copy, string filename);
void OpenTree(PrimePowerTree** serial_copy, string filename);
void PrintTriangle(BeurlingTreeBase* tree);
void ExportTriangle(BeurlingTreeBase* tree, string filename);
void DemoIntegerTree();
void DemoPrimePowerTree();
void DemoRestrictedTree();
void DemoDiagonalFormula();
void RunRandomWalks(/*int runs, int height*/);
void DemoRandomWalk();
void DebugRandomWalk();

int main() {
  RunTests();
  //DemoIntegerTree();

  //DemoPrimePowerTree();
  //DemoRestrictedTree();
  //DemoDiagonalFormula();
  //RunRandomWalks();

  //IntegerTree* tree = nullptr;
  //BuildTree(&tree, 10);
  //PrintTriangle(tree);
  //if (tree) delete tree;

  /*for(size_t x = 2; x < 4; ++x) {
    DiagonalFormula df(x);
    vector <Tuple> coefficients = df.GetFormula();
    cout << coefficients.size() << " ";
    for (size_t y = 0; y < coefficients.size(); ++y) {
      cout << "(" << to_string(coefficients[y].first) << ", "
           << to_string(coefficients[y].second) << "), ";
    }
    cout << endl;
  }*/

  //DebugRandomWalk();

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
  VerifyTest(TestLinearProgramming(&error), "Linear programming test", &error);
  VerifyTest(TestMultiplicationTable(&error), "MultiplicationTable test", &error);
  VerifyTest(TestRandomWalk(&error), "RandomWalk test", &error);
}

void BuildTree(IntegerTree** tree, unsigned int height) {
  time_t begin = time(NULL);
  cout << "Building Integer Tree to height " << height << endl;
  *tree = new IntegerTree(height);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Built Tree in " << seconds << " seconds" << endl;
}

void BuildTree(PrimePowerTree** tree, unsigned int height) {
  time_t begin = time(NULL);
  cout << "Building PrimePower Tree to height " << height << endl;
  *tree = new PrimePowerTree(height);
  time_t end = time(NULL);
  double seconds = difftime(end, begin);
  cout << "Built Tree in " << seconds << " seconds" << endl;
}

void BuildTree(RestrictedTree** tree, unsigned int height) {
  time_t begin = time(NULL);
  cout << "Building Restricted Tree to height " << height << endl;
  *tree = new RestrictedTree(MAX_PRIMES, MAX_COMPOSITES, height);
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

void OpenTree(RestrictedTree** serial_copy, string filename) {
  time_t begin = time(NULL);
  cout << "Opening..." << endl;
  *serial_copy = new RestrictedTree(filename);
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

void DemoRestrictedTree() {
  RestrictedTree* tree = nullptr;
  RestrictedTree* serial_copy = nullptr;

  BuildTree(&tree, HEIGHT);
  ExportAsDot(tree, "restricted.dot");
  //SerializeTree(tree, "restricted_serial.txt");
  //OpenTree(&serial_copy, "restricted_serial.txt");
  //ExportAsDot(serial_copy, "restricted_serial.dot");

  if (tree) delete tree;
  if (serial_copy) delete serial_copy;
}

void DemoDiagonalFormula() {
  DiagonalFormula f(5);
  cout << f.GetFormulaString() << endl;
  for(size_t x = 2; x < 5; ++x) {
    DiagonalFormula df(x);
    vector <Tuple> coefficients = df.GetFormula();
    cout << coefficients.size() << " ";
    for (size_t y = 0; y < coefficients.size(); ++y) {
      cout << "(" << to_string(coefficients[y].first) << ", "
           << to_string(coefficients[y].second) << "), ";
    }
    cout << endl;
  }
}

void RunRandomWalk(RandomWalk** tree, unsigned int height) {
    //time_t begin = time(NULL);
    //cout << "Building path of length " << height << endl;
    *tree = new RandomWalk(height);
    //time_t end = time(NULL);
    //double seconds = difftime(end, begin);
    //cout << "Built path in " << seconds << " seconds" << endl;
}

void exportCSV(int matrix[RUNS][HEIGHT+1], string filename) {
    std::ofstream out(filename);
    
    for (int i = 0; i < RUNS; i++) {
        for (int j = 0; j < HEIGHT+1; j++)
            out << std::to_string(matrix[i][j]) << ',';
        out << '\n';
    }
}

void RunRandomWalks(/*int runs, int height*/) {
    RandomWalk* path = nullptr;
    int number_primes[RUNS][HEIGHT+1];
    int number_prime_factors[RUNS][HEIGHT+1];
    int number_distinct_prime_factors[RUNS][HEIGHT+1];
    int number_children[RUNS][HEIGHT+1];
    
    for (int i = 0; i < RUNS; i++) {
        //RunRandomWalk(&path, j);
        if (path != nullptr)
            delete path;
        path = new RandomWalk(HEIGHT);
        for (int j = 0; j < HEIGHT+1; j++) {
            number_primes[i][j] = path->NumPrimes(j);
            number_prime_factors[i][j] = path->NumPrimeFactors(j);
            number_distinct_prime_factors[i][j] = path->NumDistinctPrimeFactors(j);
            number_children[i][j] = path->NumChildren(j);
        }
    }
    
    cout << "number primes: " << number_primes[RUNS-1][HEIGHT] << endl;
    cout << "number prime factors: " << number_prime_factors[RUNS-1][HEIGHT] << endl;
    cout << "number distinct prime factors: " <<
    number_distinct_prime_factors[RUNS-1][HEIGHT] << endl;
    cout << "number children: " << number_children[RUNS-1][HEIGHT] << endl;
    
    exportCSV(number_primes, "number_primes.csv");
    exportCSV(number_prime_factors, "number_prime_factors.csv");
    exportCSV(number_distinct_prime_factors, "number_distinct_prime_factors.csv");
    exportCSV(number_children, "number_children.csv");
}

void DemoRandomWalk(/*int runs, int height*/) {
    RandomWalk* path = nullptr;
    int number_primes[HEIGHT+1];
    int number_prime_factors[HEIGHT+1];
    int number_distinct_prime_factors[HEIGHT+1];
    int number_children[HEIGHT+1];
    vector<Factorization> factorizations;

    try {
        path = new RandomWalk(HEIGHT);
    } catch (const CandidatesException& e) {
        std::cout << "Catch is working!" << endl
                  << e.what() << endl
                  << "Table DebugString length: " << e.get_table_str().length() << endl;
      if (e.get_table_str().length() < 2000) {
        cout << e.get_table_str() << endl;
      }
      return;
    }

    for (int j = 0; j < HEIGHT+1; j++) {
        number_primes[j] = path->NumPrimes(j);
        number_prime_factors[j] = path->NumPrimeFactors(j);
        number_distinct_prime_factors[j] = path->NumDistinctPrimeFactors(j);
        number_children[j] = path->NumChildren(j);
        factorizations.push_back(path->GetFactorization(j));
    }
    delete path;

    cout << "number primes: " << number_primes[HEIGHT] << endl;
    cout << "number prime factors: " << number_prime_factors[HEIGHT] << endl;
    cout << "number distinct prime factors: " <<
    number_distinct_prime_factors[HEIGHT] << endl;
    cout << "number children: " << number_children[HEIGHT] << endl;

    for (int i = 0; i < HEIGHT+1; i++) {
        cout << i << ": " << factorizations[i].ToDotString() << endl;
    }

    //exportCSV(number_primes, "number_primes.csv");
    //exportCSV(number_prime_factors, "number_prime_factors.csv");
    //exportCSV(number_distinct_prime_factors, "number_distinct_prime_factors.csv");
    //exportCSV(number_children, "number_children.csv");
}

void DebugRandomWalk(/*int runs, int height*/) {
    RandomWalk* path = nullptr;
    int number_primes[HEIGHT+1];
    int number_prime_factors[HEIGHT+1];
    int number_distinct_prime_factors[HEIGHT+1];
    int number_children[HEIGHT+1];
    vector<Factorization> factorizations;
 
    while (true) {   
        try {
            path = new RandomWalk(HEIGHT);
        } catch (const CandidatesException& e) {
            std::cout << "Catch is working!" << endl
                      << e.what() << endl
                      << "Table DebugString length: " << e.get_table_str().length() << endl;
            if (e.get_table_str().length() < 500) {
                cout << e.get_table_str() << endl;
                delete path;
                return;
            }
        } 
        delete path;
    } 
}


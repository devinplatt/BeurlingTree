/*
 * test_tree.h
 *
 *  Created on: Nov 4, 2013
 *      Author: Devin
 *
 *  Test for the Tree and Node classes.
 */

#ifndef TEST_TREE_H_
#define TEST_TREE_H_

#include <iostream>
#include <string>
#include "tree.h"
#include "compatibility.h"
using std::string;

namespace Platt {

bool TestTree(string* error){
  Tree<int> T;
  T.Init(1);
  string output;
  int count = 1;

  auto InsertItems = [&] (Node<int>* N) {
    Node<int>* n1 = new Node<int>(count+1);
    Node<int>* n2 = new Node<int>(count+2);
    count+=2;
    N->Add(n1);
    N->Add(n2);
  };

  auto ReadItems = [&] (Node<int>* N) {
    output += Platt::to_string(N->GetData());
  };

  T.DepthFirst(T.NO_ACTION(), T.NO_ACTION(), &InsertItems);
  T.DepthFirst(T.NO_ACTION(), T.NO_ACTION(), &InsertItems);
  T.BreadthFirst(&ReadItems, T.NO_ACTION());

  string expected = "1234567";
  bool pass = (output == expected);
  if (!pass) {
    *error = "output of tree is \"" + output + "\" when it should be " + expected + "\n";
  }
  return pass;
}

}  // namespace Platt


#endif /* TEST_TREE_H_ */

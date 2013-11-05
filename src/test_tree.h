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
#include <sstream>
#include "Tree.h"
using std::string;
// Because to_string is broken with MinGW
using std::stringstream;

namespace Platt {

bool TestTree(){
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
    // This should be a one-liner, but to_string broken with MinGW
    //*out += to_string(N->GetData());
    stringstream ss;
    ss << N->GetData();
    output += ss.str();
  };

  T.DepthFirst(T.NO_ACTION(), T.NO_ACTION(), &InsertItems);
  T.DepthFirst(T.NO_ACTION(), T.NO_ACTION(), &InsertItems);
  T.BreadthFirst(&ReadItems, T.NO_ACTION());

  return output == "1234567";
}

}  // namespace Platt


#endif /* TEST_TREE_H_ */

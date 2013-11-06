/*
 * test_utils.h
 *
 *  Created on: Nov 5, 2013
 *      Author: Devin
 *
 *  Some basic functions for writing tests. I eschew using a framework for
 *  simplicity and compatibility.
 *
 *  Test functions are written in CAPS even though they are not macros. That's
 *  just what I am used to.
 */

#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <string>
#include "compatibility.h"
using std::string;

namespace Platt {

template<class T>
void EXPECT_EQ(T actual, T expected, bool* pass, string* error, string name) {
  if (!(actual == expected)) {
    *pass = false;
    *error += name + " = " + to_string(actual) + ", but it should equal "
             + to_string(expected) + "\n";
  }
}

void EXPECT_TRUE(bool condition, bool* pass, string* error, string message) {
  if (!condition) {
    *pass = false;
    *error += message + "\n";
  }
}

void EXPECT_FALSE(bool condition, bool* pass, string* error, string message) {
  if (condition) {
    *pass = false;
    *error += message + "\n";
  }
}

}  // namespace Platt

#endif /* TEST_UTILS_H_ */

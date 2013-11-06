/*
 * compatibility.h
 *
 *  Created on: Nov 5, 2013
 *      Author: Devin
 *
 *  Provides resources for when compatibility is an issue. At the moment the
 *  issue is that my version of MinGW does not support std::to_string() or
 *  std::stoul().
 *
 *  Implementation is inlined for sake of brevity (there is no need to make a
 *  separate .cpp file just yet, so long as this file remains short.)
 */

#ifndef COMPATIBILITY_H_
#define COMPATIBILITY_H_

#include <string>
#include <sstream>
using std::string;
using std::stringstream;

namespace Platt {

template <class T>
string to_string(T number) {
  stringstream ss;
  ss << number;
  return ss.str();
}

inline unsigned long stoul(string s) {
  stringstream ss(s);
  unsigned long u;
  ss >> u;
  return u;
}

}  // namespace Platt

#endif  /* COMPATIBILITY_H_ */

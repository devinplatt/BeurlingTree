/*
 * node.h
 *
 *  Created on: Nov 4, 2013
 *      Author: Devin
 *
 *  This file declares and defines inline the Node class, which is used in the
 *  Tree class.
 *
 *  For our purposes, a functor is a functor that takes Node* as its parameter
 *  and must have an operator which takes the argument:  “Node<T>* N”.
 *
 *  Implementation is inlined in the header since it is short.
 */

#ifndef NODE_H_
#define NODE_H_

#include<vector>
using std::vector;

namespace Platt {

template <class T> class FunctorBase;

template <class T>
class Node{
 private:
  T data;
  vector< Node<T>* > nodes;

 public:
  Node() {}
  Node(T Data){data=Data;}
  void SetData(T Data) {data = Data;}
  void Add(Node* N) {nodes.push_back(N);}
  T GetData() const {return data;}
  T* GetDataPtr() {return &data;}
  vector< Node<T>* > GetChildren() {return nodes;}
  bool Childless(){ return nodes.size()==0; }

  template <class functor>
  void Iterate(functor& f) {
    for (auto &child : nodes) {
      f(child);
    }
  }
};

}  // namespace Platt

#endif /* NODE_H_ */

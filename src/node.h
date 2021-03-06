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
 *  and must have an operator which takes the argument:  �Node<T>* N�.
 *
 *  Implementation is inlined in the header since it is short.
 */

#ifndef NODE_H_
#define NODE_H_

#include<vector>
using std::vector;
#include <map>
using std::map;

namespace Platt {

template <class T>
class Node{
 private:
  T data;
  map< T, Node<T>* > nodes;

 public:
  Node() {}
  Node(T Data){data=Data;}
  void SetData(T Data) {data = Data;}
  void Add(Node* N) {nodes.insert(std::pair<T,Node<T>*>(N->data,N));}
  T GetData() const {return data;}
  T* GetDataPtr() {return &data;}
  vector< Node<T>* > GetChildren() {
    vector< Node<T>* > children_vector;
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
      children_vector.push_back(it->second);
    return children_vector;
  }
  Node<T>* GetChild (const T& in) {return nodes[in];}
  bool Childless(){return nodes.size()==0;}
  bool HasChild (const T& in) {return nodes.count(in) == 1;}

  template <class functor>
  void Iterate(functor& f) {
    for (auto &child : nodes) {
      f(child.second);  // child.second is the child node
    }
  }
};

}  // namespace Platt

#endif /* NODE_H_ */

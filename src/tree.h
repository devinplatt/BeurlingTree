/*
 * tree.h
 *
 *  Created on: Nov 4, 2013
 *      Author: Devin
 *
 *  A tree designed specifically for the Beurling tree program. It has no node
 *  insertion/deletion or search functions. For our purposes, a functor is a
 *  functor that takes Node* as its parameter and must have an operator which
 *  takes the argument: Node<T>* N".
 *
 *  Unfortunately due to templating we must include implementation in the
 *  header file.
 */

#ifndef TREE_H_
#define TREE_H_

#include <queue>
#include "Node.h"
using std::queue;

namespace Platt {

template <class T>
class Tree {
 private:
  Node<T>* root;

  // We define some functors for use with the Iterate method of Node.

  class CopyFunctor {
   private:
    Node<T>* current;

   public:
    CopyFunctor() {current = nullptr;}
    CopyFunctor(Node<T>* in) {
      current = in;
    }

    CopyFunctor(const CopyFunctor& c){current = c.current;}

    void operator() (Node<T>* N) {
      Node<T>* temp = new Node<T>( N->GetData() );
      current->Add(temp);
      CopyFunctor c(temp);
      N->Iterate(c);
    }
  };

  template <class Rec1, class Rec2, class Leaf>
  class DfsFunctor {
   private:
    Rec1* r1;
    Rec2* r2;
    Leaf* l;

   public:
    DfsFunctor() {r1 = nullptr; r2 = nullptr; l = nullptr;}
    DfsFunctor(Rec1* R1, Rec2* R2, Leaf* L) {
      r1=R1;
      r2=R2;
      l=L;
    }

    DfsFunctor(const DfsFunctor& R) {
      r1=R.r1;
      r2=R.r2;
      l=R.l;
    }

    void operator() (Node<T>* current) {
      if( !current->Childless() ) {  // if not at leaf
        if (r1) {(*r1)( current );}
        current->Iterate ( *this );
        if (r2) {(*r2)( current );}
      }
      else  // else at leaf
        (*l)( current );
    }
  };

  class BlankFunctor {
   public:
    BlankFunctor(){}
    void operator() (Node<T>* N){}
  };

 public:
  Tree(){root = 0;}
  Tree(const Tree<T>& in) {
    Tree();
    *this = in;
  }

  const Tree<T>& operator = ( const Tree& in ) {
    if (in.root != 0) {
      root = new Node<T>(in.root->GetData());
      CopyFunctor c(root);
      in.root->Iterate(c);
    }
    return in;
  }

  // Initialize with root value
  void Init(T data) {
    Node<T>* N = new Node<T>(data);
    root = N;
  }

  // The behavior of this function is unexpected if the functors modify the
  // "nodes" field of any nodes.
  template <class parent_functor, class child_functor>
  void BreadthFirst(parent_functor* p, child_functor* c) {
    queue< Node<T>* > q;
    Node<T>* temp;
    auto EnqueueFunc = [&] (Node<T>* N) {q.push(N);};

    q.push(root);

    // Note: the functors should not alter the "nodes" fields of the nodes.
    while (!q.empty()) {
      temp = q.front();
      q.pop();
      if (p) {(*p)(temp);}
      if (c) {temp->Iterate(*c);}
      temp->Iterate( EnqueueFunc ); // enqueue all of the children
    }
  }

  // Follows a depth first path and takes actions at different steps.
  template <class Rec1, class Rec2, class leaf>
  void DepthFirst(Rec1* r1, Rec2* r2, leaf* l) {
    DfsFunctor<Rec1,Rec2,leaf> d(r1,r2,l);
    d(root);
  };

  // This is useful when the generic algorithms (DFS, BFS) still don't cut it.
  Node<T>* GetRoot() {return root;}

  // Use this when you want to specify NO_ACTION at a certain step of a generic
  // algorithm.
  BlankFunctor* NO_ACTION() {return nullptr;}

  ~Tree(){
    auto DeleteAfterChildren = [] (Node<T>* N) {delete N;};
    auto DeleteLeaf = [] (Node<T>* N) {delete N;};

    if(root != 0)
      DepthFirst(NO_ACTION() , &DeleteAfterChildren, &DeleteLeaf);
  }
};

}  // namespace Platt


#endif /* TREE_H_ */

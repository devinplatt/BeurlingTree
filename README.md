BeurlingTree
============

Code for generating a tree for the partial order of factorizations of Beurling generalized integers.

I've decided to implement my own basic tree data structure rather than using a library like
Boost::Graph for simplicity and for the sake of not having to learn the API. For that reason I had
to provide my own method of serialization, which may not be the most efficient.

At the moment I've uploaded the Tree, Factorization, and MultiplicationTable classes. Documentation
on the algorithm for deciding children of a node in the tree is given only in the code at the
moment. I plan to provide separate documentation for the algorithm once all of the code is up. More
on the way soon!
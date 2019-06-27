# BeurlingTree

Code for generating a tree for the partial order of factorizations of Beurling generalized integers. For more information on the topic, see my articles: http://www.devinplatt.com/math.html
The implementation is in C++. In the future I might write versions in other languages, but for now this is all you get.

## Installing Dependencies

This program makes use of the COIN-OR Linear Programming Solver (or CLP, see https://github.com/coin-or/Clp). To install on Ubuntu, run the command
```bash
sudo apt-get install coinor-libclp-dev
```

## Building

Using g++ in Ubuntu, one can link to the CLP libraries using the command
```bash
g++ *.cpp -std=c++11 -I/usr/include/coin/ -lClp -lCoinUtils -lbz2 -lz -llapack -lblas -lm -o output
```

This is a work in progress. Additional documentation is provided in the separate Documentation.txt file.


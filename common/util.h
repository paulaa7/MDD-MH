#pragma once

#include <iostream>
#include <vector>

using namespace std;

// Overload << for vector
template <typename S>
ostream &operator<<(ostream &os, const vector<S> &vector) {

  // Printing all the elements using <<
  for (auto i : vector)
    os << i << " ";
  return os;
}

template <class T> void print_vector(string name, const vector<T> &sol) {
    cout << name << ": ";
  
    for (auto elem : sol) {
      cout << elem << ", ";
    }
    cout << endl;
}

template <class T> void print_vector2(string name, const vector<T> &sol) {
    cout << name << ": ";
  
    for (auto elem : sol) {
      cout << "(" << elem.first << ", " << elem.second << "), ";
    }
    cout << endl;
}

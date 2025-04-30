#include <cassert>
#include <greedy.h>
#include <iostream>
#include <problem.h>
#include <random.hpp>
#include "MDD.h"

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH GreedySearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);

  size_t m = problem->getSolutionSize();
  size_t n = problem->getSolutionDomainRange().second + 1;
  tSolution solucion(0);

  vector<size_t> options;
  for (size_t i=0; i<n; i++) options.push_back(i);

  //el primer elemento es aleatorio
  solucion.push_back(Random::get<tDomain>(0,n-1));
  options.erase(find(options.begin(), options.end(), solucion[0]));

  //evaluacion de las posibles fitness
  tFitness aux_fit = 0.0, best_fit = 0.0;
  tSolution best_sol(0);
  size_t index = 0;

  for (int r = 1; r < m; r++) {
    if (options.empty()) break;

    tSolution sol1(0);
    tSolution sol2(0);
    
    sol1 = sol2 = solucion;
    
    sol1.push_back(options[0]);
    best_fit = aux_fit = problem->fitness(sol1);
    best_sol = sol1;
    index = 0;
    
    for (size_t i = 1; i < options.size(); i++){
      sol2.push_back(options[i]);
      aux_fit = problem->fitness(sol2);

      if (aux_fit < best_fit){
        best_fit = aux_fit;
        best_sol = sol2;
        index = i;
      }
      sol2.pop_back();
    }

    solucion.push_back(options[index]);

    options[index] = options.back();

    options.pop_back();
  }

  return ResultMH(solucion, best_fit, 1);
}



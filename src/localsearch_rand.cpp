#include <cassert>
#include <localsearch_rand.h>
#include <problem.h>
#include <random.hpp>
#include <mh.h>
#include <unordered_set>
#include <iostream>

using namespace std;

/**
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */

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

ResultMH LocalSearchRandom::optimize(Problem *problem, int maxevals){
  assert(maxevals > 0);

  //n y m
  size_t m = problem->getSolutionSize();
  size_t n = problem->getSolutionDomainRange().second + 1;

  //solution
  tSolution solution = problem->createSolution();
  SolutionFactoringInfo *info = problem->generateFactoringInfo(solution);
  tFitness fitness = problem->fitness(solution);

  //opciones
  unordered_set<tDomain> usados(solution.begin(), solution.end());
  vector<tDomain> options;

  for (tDomain i = 0; i < n; i++) {
    if (usados.find(i) == usados.end()) {
        options.push_back(i);
    }
  }

  vector<pair<size_t, size_t>> combinaciones_posibles;
  combinaciones_posibles.clear();
    for (size_t i=0; i<m; i++){
      for (const tDomain& opt : options) {
          combinaciones_posibles.emplace_back(i, opt);
  }}
  Random::shuffle(combinaciones_posibles);
  size_t index_comb = 0;

  for (int z=0; z<maxevals; z++){

      if (combinaciones_posibles.empty()) continue;
      if (index_comb >= combinaciones_posibles.size()) break;
      
      auto& [pos, new_value] = combinaciones_posibles[index_comb];

      tFitness fit2 = problem->fitness(solution, info, pos, new_value);

      if (fit2 < fitness) {
          auto it = std::find(options.begin(), options.end(), new_value);

          if (it != options.end()) {
              *it = solution[pos];
          }

          problem->updateSolutionFactoringInfo(info, solution, pos, new_value);
          solution[pos] = new_value;
          fitness = fit2;

          //actualizar combinaciones_posibles
          combinaciones_posibles.clear();
            for (size_t i=0; i<m; i++){
              for (const tDomain& opt : options) {
                  combinaciones_posibles.emplace_back(i, opt);
          }}
          index_comb = 0;
          
      } else index_comb ++;
  }

  return ResultMH(solution, fitness, maxevals);
}


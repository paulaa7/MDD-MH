#include <cassert>
#include <localsearch_heur.h>
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

ResultMH LocalSearchHeur::optimize(Problem *problem, int maxevals){
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
  sort(combinaciones_posibles.begin(), combinaciones_posibles.end(), [problem, solution, info](const pair<size_t, tOption>& a, const pair<size_t, tOption>& b) {
    return (problem->fitness(solution, info, a.first, a.second) < problem->fitness(solution, info, b.first, b.second));
   });

  for (int z=0; z<maxevals; z++){

      if (combinaciones_posibles.empty()) continue;

      auto& [pos, new_value] = combinaciones_posibles[0];

      tFitness fit2 = problem->fitness(solution, info, pos, new_value);

      if (fit2 < fitness) {
          auto it = find(options.begin(), options.end(), new_value);

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
          sort(combinaciones_posibles.begin(), combinaciones_posibles.end(), [problem, solution, info](const pair<size_t, tOption>& a, const pair<size_t, tOption>& b) {
            return (problem->fitness(solution, info, a.first, a.second) < problem->fitness(solution, info, b.first, b.second));
          });
      }
      else break;
  }

  return ResultMH(solution, fitness, maxevals);
}


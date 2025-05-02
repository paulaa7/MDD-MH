#include <cassert>
#include <AM_comun.h>
#include <mh.h>
#include <unordered_set>
#include <set>
#include <iostream>

using namespace std;

/**
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */

void AM_comun::reparar(const int num_nodos, vector<bool> &hijo) {
    if (num_nodos == 0) return;

    else if (num_nodos > 0) {
      size_t pos = 0;

      for (int i = num_nodos; i > 0; i --) {
        while (!hijo[pos])
          pos = Random::get<size_t>(0, hijo.size()-1);

        hijo[pos] = false;
      }
    }

    else {
      size_t pos = 0;

      for (int i = num_nodos; i < 0; i ++) {
        while (hijo[pos])
          pos = Random::get<size_t>(0, hijo.size()-1);
        
        hijo[pos] = true;
      }
    }
  }

void AM_comun::cruce_uniforme(const tSolution& padre1, const tSolution& padre2, 
                                    vector<tSolution> &poblacion_nueva) {
    //almacenaré los padres y los hijos como vectores de booleanos durante el cruce
    vector<bool> p1 (n, false);
    vector<bool> p2 (n, false);
    vector<bool> h1 (n, false);
    vector<bool> h2 (n, false);
    tSolution hijo1, hijo2;

    bool hijo_a_elegir;
    int tam = padre1.size();

    //inicialización de padres
    for (size_t i = 0; i < tam; i ++) {
      p1[padre1[i]] = true;
      p2[padre2[i]] = true;
    }

    //asignación de valores a los hijos
    for (size_t i = 0; i < n; i ++) {
      //los valores comunes se mantienen en ambos
      if (p2[i] == p1[i]) h1[i] = h2[i] = p2[i];
      
      else { //el resto se dividen de forma aleatoria
        hijo_a_elegir = Random::get<bool>();

        if (hijo_a_elegir) { h1[i] = p1[i]; h2[i] = p2[i];}
        else               { h2[i] = p1[i]; h1[i] = p2[i];}
      }
    }

    //reparación
    int num_nodos_hijo1=0, num_nodos_hijo2=0;
    for (size_t i=0; i<n; i++) {
      num_nodos_hijo1 += (h1[i] - p1[i]);
      num_nodos_hijo2 += (h2[i] - p1[i]);
    }

    reparar(num_nodos_hijo1, h1);
    reparar(num_nodos_hijo2, h2);

    for (size_t i = 0; i < n; i ++) {
      if (h1[i])  hijo1.push_back(i);
      if (h2[i])  hijo2.push_back(i);
    }

    poblacion_nueva.push_back(hijo1);
    poblacion_nueva.push_back(hijo2);
  }

void AM_comun::mutar(tSolution &cromosoma) {
    set<size_t> posibles;
    for (size_t i = 0; i < n; i ++) posibles.insert(i);
    for (size_t i = 0; i < cromosoma.size(); i ++) posibles.erase(cromosoma[i]);

    size_t a_cambiar = Random::get<size_t>(0,cromosoma.size()-1);
    size_t pos_nuevo_valor = Random::get<size_t>(0,posibles.size()-1);

    auto it = posibles.begin();
    advance(it, pos_nuevo_valor);

    cromosoma[a_cambiar] = *it;
  }
  
tSolution AM_comun::BL_rand(tSolution &sol, int maxevals, Problem * problem){
  assert(maxevals > 0);

  //n y m
  size_t m = problem->getSolutionSize();
  size_t n = problem->getSolutionDomainRange().second + 1;

  //solution
  tSolution solution = sol;
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

  return solution;
}
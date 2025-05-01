#include <cassert>
#include <AGE_posicion.h>
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

  void AGE_posicion::cruce_intercambio(const tSolution& padre1, const tSolution& padre2, 
                                       vector<tSolution> &poblacion_nueva) {
    //almacenaré los padres y los hijos como vectores de booleanos durante el cruce
    vector<bool> p1 (n, false);
    vector<bool> p2 (n, false);
    vector<bool> h1 (n, false);
    vector<bool> h2 (n, false);
    tSolution hijo1, hijo2;

    vector<bool> nodes_shuffle;
    vector<bool> shuffle1, shuffle2;

    //inicialización de padres
    for (size_t i = 0; i < padre1.size(); i ++) p1[padre1[i]] = p2[padre2[i]] = true;

    for (size_t i = 0; i < n; i ++) if (p2[i] != p1[i]) nodes_shuffle.push_back(p2[i]);

    shuffle1 = shuffle2 = nodes_shuffle;
    Random::shuffle(shuffle1);
    Random::shuffle(shuffle2);

    //asignación de valores a los hijos
    for (size_t i = 0; i < n; i ++) {
      //los valores comunes se mantienen en ambos
      if (p2[i] == p1[i]) h1[i] = h2[i] = p2[i];

      else { //el resto se reparte aleatoriamente
        h1[i] = shuffle1.back();
        shuffle1.pop_back();

        h2[i] = shuffle2.back();
        shuffle2.pop_back();
      }
    }       

    for (size_t i = 0; i < n; i ++) {
      if (h1[i])  hijo1.push_back(i);
      if (h2[i])  hijo2.push_back(i);
    }

    poblacion_nueva.push_back(hijo1);
    poblacion_nueva.push_back(hijo2);
  }

  void AGE_posicion::mutar(tSolution &gen) {
    set<size_t> posibles;
    for (size_t i = 0; i < n; i ++) posibles.insert(i);
    for (size_t i = 0; i < gen.size(); i ++) posibles.erase(gen[i]);

    size_t a_cambiar = Random::get<size_t>(0,gen.size()-1);
    size_t pos_nuevo_valor = Random::get<size_t>(0,posibles.size()-1);

    auto it = posibles.begin();
    std::advance(it, pos_nuevo_valor);

    gen[a_cambiar] = *it;
  }
  
  ResultMH AGE_posicion::optimize(Problem *problem, int maxevals){
    assert(maxevals > 0);
    size_t evals = 0;
  
    //n y m
    m = problem->getSolutionSize();
    n = problem->getSolutionDomainRange().second + 1;
  
    //solution
    vector<tSolution> poblacion;
    vector<tSolution> poblacion_sel;
    vector<tSolution> poblacion_nueva;

    tSolution peor_sol, mejor_sol;
    size_t peor_sol_index;

    tFitness fitness;

    tSolution solution, mejor_hijo_reemplazo;
    solution = problem->createSolution();
    poblacion.push_back(solution);

    peor_sol = mejor_sol = solution;

    for (size_t i=1; i<50; i++){
        solution = problem->createSolution();
        poblacion.push_back(solution);

        if (problem->fitness(solution) > problem->fitness(peor_sol)) {
            peor_sol = solution; 
            peor_sol_index = i;
        }
        if (problem->fitness(solution) < problem->fitness(mejor_sol))
            mejor_sol = solution;
    }

    for (; evals < maxevals; evals += 50) {
        poblacion_sel.clear();
        poblacion_nueva.clear();
        poblacion_sel.reserve(2);
        //poblacion_nueva.reserve(50);

        //selección
        tSolution sol1, sol2;
        size_t pos_aux1, pos_aux2;
        for (size_t i=0; i<poblacion_sel.size(); i++) {
          pos_aux1 = Random::get<size_t>(0,poblacion.size()-1);
          pos_aux2 = Random::get<size_t>(0,poblacion.size()-1);

          if (problem->fitness(poblacion[pos_aux2]) < problem->fitness(poblacion[pos_aux1]))
              pos_aux1 = pos_aux2;

          pos_aux2 = Random::get<size_t>(0,poblacion.size()-1);

          if (problem->fitness(poblacion[pos_aux2]) < problem->fitness(poblacion[pos_aux1]))
              poblacion_sel.push_back(poblacion[pos_aux2]);
          else
              poblacion_sel.push_back(poblacion[pos_aux1]);
        }
      
        //cruce
        cruce_intercambio(poblacion_sel[0], poblacion_sel[1], poblacion_nueva);

        //mutación  (poblacion_sel, no?)
        size_t n_mutaciones = ceil(0.1 * poblacion_sel.size());
        size_t mutaciones = 0;

        while (mutaciones < n_mutaciones) {
          for (size_t i=0; i<poblacion_sel.size() && mutaciones < n_mutaciones; i++) {
              mutar(poblacion_nueva[i]);
              mutaciones ++;
          }
        }

        //averiguar qué hijo tiene mejor fitness
        mejor_hijo_reemplazo = poblacion_nueva[1];
        if (problem->fitness(poblacion_nueva[0]) < problem->fitness(poblacion_nueva[1]))
            mejor_hijo_reemplazo = poblacion_nueva[0];

        //comprobar si el mejor hijo es la mejor solución
        if (problem->fitness(mejor_hijo_reemplazo) < problem->fitness(mejor_sol))
            mejor_sol = mejor_hijo_reemplazo;

        //reemplazo
        if (problem->fitness(peor_sol) > problem->fitness(mejor_hijo_reemplazo));
            poblacion[peor_sol_index] = mejor_hijo_reemplazo;

        //recalcular peor solución
        peor_sol = poblacion_nueva[0];
        peor_sol_index = 0;
        for (size_t i=1; i<poblacion_nueva.size(); i++){
          if (problem->fitness(poblacion_nueva[i]) > problem->fitness(peor_sol)) {
            peor_sol = poblacion_nueva[i];
            peor_sol_index = i;
          }
        }
    }

    fitness = problem->fitness(mejor_sol);
    return ResultMH(mejor_sol, fitness, evals);
  }


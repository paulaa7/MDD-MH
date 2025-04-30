#include <algorithm>
#include <MDD.h>
#include <random.hpp>
#include <fstream>
#include <iostream>
using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

tFitness MDD::fitness(const tSolution &solution) {
  vector<double> distancias(solution.size(), 0.0);

  for (size_t i = 0; i < solution.size(); i ++){
    for (size_t j = 0; j < solution.size(); j ++){
      (solution[i] <= solution[j]) ? distancias[i] += dispersiones[solution[i]][solution[j]] 
                                   : distancias[i] += dispersiones[solution[j]][solution[i]];
    }
  }

  auto [minimo_it, maximo_it] = std::minmax_element(distancias.begin(), distancias.end());
  double minimo = *minimo_it;
  double maximo = *maximo_it;

  return maximo - minimo;
}

tSolution MDD::createSolution() {
  tSolution solution(m);
  int aux;

  for (size_t i = 0; i < solution.size();) {
    aux = Random::get<int>(0,n-1);

    if (find(solution.begin(), solution.begin() + i, aux) == solution.begin() + i) {
      solution[i] = aux;
      i++;
    }
  }

  return solution;
}

void MDD::leerFichero(const char* ruta){
  ifstream file(ruta);
    if (!file) {
        cerr << "Error al abrir el archivo" << endl;
        exit(1);
    }
    
    file >> n >> m;
    
    dispersiones = vector<vector<double>>(n, vector<double>(n, 0.0));
    
    int nodo1, nodo2;
    double dispersion;
    
    while (file >> nodo1 >> nodo2 >> dispersion) {
            dispersiones[nodo1][nodo2] = dispersion;
    }
    
    file.close();
}


 tFitness MDD::fitness(const tSolution &solution,
                           SolutionFactoringInfo *solution_info,
                           unsigned pos_change, tDomain new_value) {

  auto* info = new LS(*dynamic_cast<LS*>(solution_info));

  info->distancias[pos_change] = 0.0;

  for (size_t i = 0; i < info->distancias.size(); i ++){
    if (i != pos_change){
      //restar distancias con el nodo a quitar
      info->distancias[i] -= (solution[i] <= solution[pos_change])
                             ? dispersiones[solution[i]][solution[pos_change]]
                             : dispersiones[solution[pos_change]][solution[i]];

      //sumar distancias con el nodo a añadir
      info->distancias[i] += (solution[i] <= new_value)
                             ? dispersiones[solution[i]][new_value]
                             : dispersiones[new_value][solution[i]];

      //calcular distancias para el nuevo nodo
      info->distancias[pos_change] += (new_value <= solution[i])
                                      ? dispersiones[new_value][solution[i]]
                                      : dispersiones[solution[i]][new_value];
    }
  }

  auto [minimo_it, maximo_it] = std::minmax_element(info->distancias.begin(), info->distancias.end());
  double minimo = *minimo_it;
  double maximo = *maximo_it;

  return maximo - minimo;
}



SolutionFactoringInfo* MDD::generateFactoringInfo(const tSolution &solution) {
  LS *info = new LS(solution);

  vector<double> distancias(solution.size(), 0.0);
  info->distancias = distancias;

  for (size_t i = 0; i < solution.size(); i ++){
    for (size_t j = 0; j < solution.size(); j ++)
      (solution[i] <= solution[j]) ? info->distancias[i] += dispersiones[solution[i]][solution[j]] 
                                   : info->distancias[i] += dispersiones[solution[j]][solution[i]];
  }
  return info;
}


void MDD::updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info, 
        const tSolution &solution, unsigned pos_change, tDomain new_value){
          
  auto *info = dynamic_cast<LS*>(solution_info);

  info->distancias[pos_change] = 0.0;

  
  for (size_t i = 0; i < solution.size(); i ++){

    //restar distancias con el nodo a quitar
    info->distancias[i] -= (solution[i] <= solution[pos_change])
                            ? dispersiones[solution[i]][solution[pos_change]]
                            : dispersiones[solution[pos_change]][solution[i]];

    //sumar distancias con el nodo a añadir
    info->distancias[i] += (solution[i] <= new_value)
                            ? dispersiones[solution[i]][new_value]
                            : dispersiones[new_value][solution[i]];

    //calcular distancias para el nuevo nodo
    info->distancias[pos_change] += (new_value <= solution[i])
                                    ? dispersiones[new_value][solution[i]]
                                    : dispersiones[solution[i]][new_value];
  }

}


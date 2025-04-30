#ifndef __MDD_H
#define __MDD_H
#include "../common/problem.h"
#include <vector>

class LS : public SolutionFactoringInfo {
    public:
        std::vector<double> distancias;
        LS(const tSolution &s) : distancias(s.size(), 0.0f) {}
        void updateDistance(size_t index, double value) {
            distancias[index] = value;
        }

        double getDistance(size_t index) const {
            return distancias[index];
        }
};

class MDD : public Problem {
    size_t n = 0;
    size_t m = 0;

    std::vector<std::vector<double>> dispersiones;

    public:
        MDD(const char* ruta) : Problem() {leerFichero(ruta);}


        tFitness fitness(const tSolution &solution) override;

        tFitness fitness(const tSolution &solution, SolutionFactoringInfo *solution_info,
                    unsigned pos_change, tDomain new_value) override;


        SolutionFactoringInfo* generateFactoringInfo(const tSolution &solution) override;

        void updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info, 
                    const tSolution &solution, unsigned pos_change, tDomain new_value) override;


        tSolution createSolution() override;
        size_t getSolutionSize() override { return m; }
        std::pair<tDomain, tDomain> getSolutionDomainRange() override {
            return std::make_pair(0, n-1);
        }
        void leerFichero(const char* ruta);
};

#endif
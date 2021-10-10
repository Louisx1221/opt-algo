//@file       : test_func.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include <iostream>
#include <ctime>

#include "../src/PSO.h"
#include "../src/GA.h"
#include "../src/IA.h"
#include "../src/SA.h"

using namespace std;

double func(double x[])
{
    double x1 = x[0];
    double x2 = x[1];
    double x3 = x[2];
    return x1 * x2 + (x2 - 0.05) * (x2 - 0.05) + x3 * x3;
}

int main()
{
    clock_t t_start, t_end;

    int dim = 3;
    double lb[3] = { 0, -1, 0.5 };
    double ub[3] = { 1, 1, 1 };

    // Immune Algorithm
    t_start = clock();
    IA ia = IA(func, dim, lb, ub);
    ia.Optimize();
    t_end = clock();
    cout << "\nImmune Algorithm" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "x = " << ia.x_best[0] << " " << ia.x_best[1] << " " << ia.x_best[2] << endl;
    cout << "y = " << ia.y_best << endl;

    // Genetic Algorithm
    t_start = clock();
    GA ga = GA(func, dim, lb, ub);
    ga.Optimize();
    t_end = clock();
    cout << "\nGenetic Algorithm" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "x = " << ga.x_best[0] << " " << ga.x_best[1] << " " << ga.x_best[2] << endl;
    cout << "y = " << ga.y_best << endl;

    // Particle Swarm Optimization
    t_start = clock();
    PSO pso = PSO(func, dim, lb, ub);
    pso.Optimize();
    t_end = clock();
    cout << "\nParticle Swarm Optimization" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "x = " << pso.x_best[0] << " " << pso.x_best[1] << " " << pso.x_best[2] << endl;
    cout << "y = " << pso.y_best << endl;

    // Simulated Annealing
    t_start = clock();
    SA sa = SA(func, dim, lb, ub);
    sa.Optimize();
    t_end = clock();
    cout << "\nSimulated Annealing" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s" << endl;
    cout << "x = " << sa.x_best[0] << " " << sa.x_best[1] << " " << sa.x_best[2] << endl;
    cout << "y = " << sa.y_best << endl;

}

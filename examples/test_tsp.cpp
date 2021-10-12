//@file       : test_tsp.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include <iostream>
#include <ctime>

#include "../src/ACO.h"
#include "../src/TS.h"
#include "../src/MCTS.h"
#include "../src/GA.h"
#include "../src/SA.h"

using namespace std;

double HeuristicValue(double* x, double* y)
{
    return sqrt((x[0] - y[0]) * (x[0] - y[0]) + (x[1] - y[1]) * (x[1] - y[1]));
}

int main()
{
    clock_t t_start = 0.0, t_end = 0.0;

    srand((int)time(0));

    int n_cities = 100, n_dim = 3, n_pop = 50, iter_max = 100, opc = 0, opm = 0;
    double cp = 2.0, alpha = 1.0, beta = 2.0, rho = 0.2, q = 1.0, tau_max = 1.0, pc = 0.8, pm = 0.2;
    double x[3] = {};

    MCTS mcts = MCTS(HeuristicValue, n_cities, n_dim, iter_max, cp);
    TS ts = TS(HeuristicValue, n_cities, n_dim, n_pop, iter_max);
    ACO aco = ACO(HeuristicValue, n_cities, n_dim, n_pop, iter_max, alpha, beta, q, rho, tau_max);
    GA_TSP ga = GA_TSP(HeuristicValue, n_cities, n_dim, n_pop, iter_max, pc, pm, opc, opm);
    SA_TSP sa = SA_TSP(HeuristicValue, n_cities, n_dim);

    for (int i = 0; i < n_cities; i++)
    {
        x[0] = rand() / double(RAND_MAX);
        x[1] = rand() / double(RAND_MAX);
        x[2] = rand() / double(RAND_MAX);
        mcts.CreateCities(i, x);
        ts.CreateCities(i, x);
        aco.CreateCities(i, x);
        ga.SetCities(i, x);
        sa.SetCities(i, x);
    }

    // Monte Carlo Tree Search (MCTS)
    t_start = clock();
    mcts.Optimize();
    t_end = clock();
    cout << "\nMonte Carlo Tree Search (MCTS)" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s";
    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << mcts.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << mcts.reward_best << endl;

    // Tabu Search (TS)
    t_start = clock();
    ts.Optimize();
    t_end = clock();
    cout << "\nTabu Search (TS)" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s";
    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << ts.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << ts.reward_best << endl;

    // Ant Colony Optimization (ACO)
    t_start = clock();
    aco.Optimize();
    t_end = clock();
    cout << "\nAnt Colony Optimization (ACO)" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s";
    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << aco.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << aco.reward_best << endl;

    // Genetic Algorithm (GA)
    t_start = clock();
    ga.Optimize();
    t_end = clock();
    cout << "\nGenetic Algorithm (GA)" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s";
    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << ga.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << ga.reward_best << endl;

    // Simulated Annealing (SA)
    t_start = clock();
    sa.Optimize();
    t_end = clock();
    cout << "\nSimulated Annealing (SA)" << endl;
    cout << "time = " << double(t_end - t_start) / CLOCKS_PER_SEC << "s";
    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << sa.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << sa.reward_best << endl;
}

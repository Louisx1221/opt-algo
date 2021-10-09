//@file       : test_tsp.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include <iostream>
#include <ctime>

#include "../src/ACO.h"
#include "../src/TS.h"
#include "../src/MCTS.h"
#include "../src/GA.h"

using namespace std;

double HeuristicValue(double* x, double* y)
{
    return sqrt((x[0] - y[0]) * (x[0] - y[0]) + (x[1] - y[1]) * (x[1] - y[1]));
}

int main()
{
    srand((int)time(0));

    int n_cities = 10, n_dim = 2, n_pop = 50, iter_max = 100;
    double cp = 2.0, alpha = 1.0, beta = 2.0, rho = 0.2, q = 1.0, tau_max = 1.0;
    double x[2] = {};

    MCTS mcts = MCTS(HeuristicValue, n_cities, n_dim, n_pop * iter_max, cp);
    TS ts = TS(HeuristicValue, n_cities, n_dim, n_pop, iter_max);
    ACO aco = ACO(HeuristicValue, n_cities, n_dim, n_pop, iter_max, alpha, beta, q, rho, tau_max);
    GA_TSP ga = GA_TSP(HeuristicValue, n_cities, n_dim, n_pop, iter_max);
    //ACO aco = ACO(HeuristicValue, n_cities);
    for (int i = 0; i < n_cities; i++)
    {
        x[0] = rand() / double(RAND_MAX);
        x[1] = rand() / double(RAND_MAX);
        mcts.CreateCities(i, x);
        ts.CreateCities(i, x);
        aco.CreateCities(i, x);
        ga.SetCities(i, x);
    }

    mcts.Optimize();

    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << mcts.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << mcts.reward_best << endl;

    ts.Optimize();

    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << ts.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << ts.reward_best << endl;

    aco.Optimize();

    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << aco.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << aco.reward_best << endl;

    ga.Optimize();

    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << ga.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << ga.reward_best << endl;
}

//@file       : test_tsp.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include <iostream>
#include <ctime>

#include "../src/ACO.h"
#include "../src/TS.h"

using namespace std;

double HeuristicValue(double* x, double* y)
{
    return sqrt((x[0] - y[0]) * (x[0] - y[0]) + (x[1] - y[1]) * (x[1] - y[1]));
}

int main()
{
    srand((int)time(0));

    int n_ants = 40, n_cities = 10, iter_max = 100;
    double alpha = 0.5, beta = 0.8, rho = 0.2, q = 8, tau_max = 1;
    double x, y;

    TS ts = TS(HeuristicValue, n_ants, n_cities, iter_max);
    ACO aco = ACO(HeuristicValue, n_ants, n_cities, iter_max, alpha, beta, q, rho, tau_max);
    //ACO aco = ACO(n_ants, n_cities);
    for (int i = 0; i < n_cities; i++)
    {
        x = rand() / double(RAND_MAX);
        y = rand() / double(RAND_MAX);
        ts.CreateCities(i, x, y);
        aco.CreateCities(i, x, y);
    }

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
}

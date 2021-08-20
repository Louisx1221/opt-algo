//@file       : test_tsp.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include <iostream>
#include <ctime>

#include "../src/ACO.h"

using namespace std;

double HeuristicValue(double* x, double* y)
{
    return 1 / ((x[0] - y[0]) * (x[0] - y[0]) + (x[1] - y[1]) * (x[1] - y[1]));
}

int main()
{
    srand((int)time(0));

    int n_ants = 10, n_cities = 10, iter_max = 10;
    double alpha = 0.5, beta = 0.8, rho = 0.2, q = 8, tau_max = 1;

    ACO aco = ACO(HeuristicValue, n_ants, n_cities, iter_max, alpha, beta, q, rho, tau_max);
    //ACO aco = ACO(n_ants, n_cities);
    for (int i = 0; i < n_cities; i++)
    {
        aco.CreateCities(i, rand() / double(RAND_MAX), rand() / double(RAND_MAX));
    }

    aco.Optimize();

    cout << endl << "Best route:" << endl;
    for (int i = 0; i < n_cities; i++)
        cout << aco.route_best[i] << " ";
    cout << endl << "Best reward:" << endl << aco.reward_best << endl;
}

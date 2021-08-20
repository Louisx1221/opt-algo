//@file       : test_func.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include <iostream>

#include "../src/PSO.h"
#include "../src/GA.h"
#include "../src/IA.h"

using namespace std;

double func(double x[])
{
    double x1 = x[0];
    double x2 = x[1];
    double x3 = x[2];
    return -x1 * x2 - (x2 - 0.05) * (x2 - 0.05) - x3 * x3;
}

double func1(double x)
{
    return -(x - 1) * (x - 2);
}

int main()
{

    double lb[3] = { 0, -1, 0.5 };
    double ub[3] = { 1, 1, 1 };

    IA ia = IA(func, 3, lb, ub);
    ia.Optimize();
    cout << "x = " << ia.best_x[0] << " " << ia.best_x[1] << " " << ia.best_x[2] << endl;
    cout << "y = " << ia.best_y << endl;

    GA ga = GA(func, 3, lb, ub);
    ga.Optimize();
    cout << "x = " << ga.best_x[0] << " " << ga.best_x[1] << " " << ga.best_x[2] << endl;
    cout << "y = " << ga.best_y << endl;

    PSO pso = PSO(func, 3, lb, ub);
    pso.Optimize();

    cout << "x = " << pso.gbest_x[0] << " " << pso.gbest_x[1] << " " << pso.gbest_x[2] << endl;
    cout << "y = " << pso.gbest_y << endl;
}

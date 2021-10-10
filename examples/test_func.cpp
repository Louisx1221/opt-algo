//@file       : test_func.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include <iostream>

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

double func1(double x)
{
    return (x - 1) * (x - 2);
}

int main()
{
    int dim = 3;
    double lb[3] = { 0, -1, 0.5 };
    double ub[3] = { 1, 1, 1 };

    IA ia = IA(func, dim, lb, ub);
    ia.Optimize();
    cout << "x = " << ia.x_best[0] << " " << ia.x_best[1] << " " << ia.x_best[2] << endl;
    cout << "y = " << ia.y_best << endl;

    GA ga = GA(func, dim, lb, ub);
    ga.Optimize();
    cout << "x = " << ga.x_best[0] << " " << ga.x_best[1] << " " << ga.x_best[2] << endl;
    cout << "y = " << ga.y_best << endl;

    PSO pso = PSO(func, dim, lb, ub);
    pso.Optimize();

    cout << "x = " << pso.x_best[0] << " " << pso.x_best[1] << " " << pso.x_best[2] << endl;
    cout << "y = " << pso.y_best << endl;

    SA sa = SA(func, dim, lb, ub);
    sa.Optimize();

    cout << "x = " << sa.x_best[0] << " " << sa.x_best[1] << " " << sa.x_best[2] << endl;
    cout << "y = " << sa.y_best << endl;

}

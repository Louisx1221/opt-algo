//@file       : PSO.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include "PSO.h"

#include <iostream>
#include <cmath>
#include <ctime>

PSO::PSO(double (*func_)(double*), int n_dim_, double lb_[], double ub_[],
    int pop_, int iter_max_, double w_, double cp_, double cg_)
{
    func = func_;
    n_dim = n_dim_;  // dimension of particles, which is the number of variables of func
    pop = pop_;  // number of particles
    iter_max = iter_max_;  // max iter
    w = w_;  // inertia
    cp = cp_;
    cg = cg_;  // parameters to control personal best, global best respectively
    
    lb = new double[n_dim];
    ub = new double[n_dim];
    v_high = new double[n_dim];
    for (int i = 0; i < n_dim; i++)
    {
        lb[i] = lb_[i];
        ub[i] = ub_[i];
        v_high[i] = ub[i] - lb[i];
    }

    srand((int)time(0));

    Init();
}

PSO::~PSO()
{
    for (int i = 0; i < pop; i++)
    {
        delete[] x[i];
        delete[] v[i];
        delete[] pbest_x[i];
    }
    delete[] y;
    delete[] pbest_y;
}

void PSO::Init()
{
    x = new double* [pop];
    v = new double* [pop];
    y = new double [pop];
    pbest_x = new double* [pop];
    pbest_y = new double[pop];
    gbest = 0;
    gbest_x = new double[n_dim];
    gbest_y = 0.0;

    for (int i = 0; i < pop; i++)
    {
        x[i] = new double[n_dim];
        v[i] = new double[n_dim];
        pbest_x[i] = new double[n_dim];
        for (int j = 0; j < n_dim; j++)
        {
            x[i][j] = rand() / double(RAND_MAX) * (ub[j] - lb[j]) + lb[j];
            v[i][j] = rand() / double(RAND_MAX) * (2 * v_high[j]) - v_high[j];
            pbest_x[i][j] = 0.0;
        }
        y[i] = 0.0;
        pbest_y[i] = -INFINITY;
    }

    for (int i = 0; i < n_dim; i++)
        gbest_x[i] = 0.0;
}

void PSO::Optimize()
{
    for (int i = 0; i < iter_max; i++)
    {
        UpdateVecPos();
        Fitness();
        UpdateBest();
    }

    for (int i = 0; i < n_dim; i++)
        gbest_x[i] = x[gbest][i];
    gbest_y = y[gbest];
}

void PSO::UpdateVecPos()
{
    double r1, r2;
    for (int i = 0; i < pop; i++)
    {
        for (int j = 0; j < n_dim; j++)
        {
            r1 = rand() / double(RAND_MAX);
            r2 = rand() / double(RAND_MAX);
            v[i][j] *= w;
            v[i][j] += cp * r1 * (pbest_x[i][j] - x[i][j]) + cg * r2 * (pbest_x[gbest][j] - x[i][j]);
            x[i][j] += v[i][j];
            if (x[i][j] < lb[j])
                x[i][j] = lb[j];
            else if (x[i][j] > ub[j])
                x[i][j] = ub[j];
        }
    }
}

void PSO::Fitness()
{
    for (int i = 0; i < pop; i++)
    {
        y[i] = func(x[i]);
    }
}

void PSO::UpdateBest()
{
    for (int i = 0; i < pop; i++)
    {
        if (pbest_y[i] < y[i])
        {
            for (int j = 0; j < n_dim; j++)
                pbest_x[i][j] = x[i][j];
            pbest_y[i] = y[i];
        }
    }

    for (int i = 0; i < pop; i++)
    {
        if (pbest_y[gbest] < pbest_y[i])
            gbest = i;
    }
}
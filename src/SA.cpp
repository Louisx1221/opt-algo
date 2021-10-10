//@file       : SA.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/10/19

#include "SA.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

SA::SA(double (*func_)(double*), int n_dim_, double* lb_, double* ub_,
	double T_max_, double T_min_, double c_cool_, int L_, int stay_max_)
{
	func = func_;
	n_dim = n_dim_;
	T_max = T_max_;
	T_min = T_min_;
	T = T_max;
	// coefficient of cooldown
	c_cool = c_cool_;
	// num of iteration under every temperature (also called Long of Chain)
	L = L_;
	// stop if y_best stay unchanged over max stay counter times(also called cooldown time)
	stay_max = stay_max_;

	lb = new double[n_dim];
	ub = new double[n_dim];
	for (int i = 0; i < n_dim; i++)
	{
		lb[i] = lb_[i];
		ub[i] = ub_[i];
	}

	srand((int)time(0));

	Init();
}

SA::~SA()
{
	delete[] lb;
	delete[] ub;
	delete[] x_best;
}

void SA::Init()
{
	x_best = new double[n_dim];
	for (int i = 0;i<n_dim;i++)
		x_best[i] = (lb[i] + ub[i]) / 2;
	y_best = func(x_best);
}

void SA::Optimize()
{
	int stay_num = 0;
	double* x_current = new double[n_dim];
	double* x_new = new double[n_dim];
	double y_current = 0.0, y_new = 0.0;
	double p = 0.0;

	while ((T > T_min) && (stay_num < stay_max))
	{
		for (int i = 0; i < L; i++)
		{
			NewSol(x_current, x_new);
			y_new = func(x_new);

			// Metropolis
			if (y_new < y_current)
				p = 1.0;
			else
				p = exp(-(y_new - y_current) / T);
			if (p > rand() / double(RAND_MAX))
			{
				for (int j = 0; j < n_dim; j++)
					x_current[j] = x_new[j];
				y_current = y_new;

				if (y_new < y_best)
				{
					for (int j = 0; j < n_dim; j++)
						x_best[j] = x_new[j];
					y_best = y_new;
				}
			}
		}

		if (y_best >= y_current)
			stay_num++;
		else
			stay_num = 0;

		CoolDown();
	}

	delete[] x_current;
	delete[] x_new;
}

void SA::NewSol(double x0[], double x[])
{
	double r = 0.0, u = 0.0;
	for (int i = 0; i < n_dim; i++)
	{
		r = rand() / double(RAND_MAX);
		u = T * (pow(1 + 1 / T, r) - 1);
		r = rand() / double(RAND_MAX);
		u = (r > 0.5) ? u : (-u);
		x[i] = x0[i] + u * (ub[i] - lb[i]);
		if (x[i] < lb[i])
			x[i] = lb[i];
		else if (x[i] > ub[i])
			x[i] = ub[i];
	}
}

void SA::CoolDown()
{
	T = T * c_cool;
}
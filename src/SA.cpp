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

SA_TSP::SA_TSP(double (*func_)(double*, double*), int n_cities_, int n_dim_,
	double T_max_, double T_min_, double c_cool_, int L_, int stay_max_)
{
	func = func_;
	n_cities = n_cities_;
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

	srand((int)time(0));

	Init();
}

SA_TSP::~SA_TSP()
{
	for (int i = 0; i < n_cities; i++)
	{
		delete[] cities[i];
	}
	delete[] cities;
	delete[] route_best;
}

void SA_TSP::Init()
{
	cities = new double* [n_cities];
	route_best = new int[n_cities];
	for (int i = 0; i < n_cities; i++)
	{
		cities[i] = new double[n_dim];
		for (int j = 0; j < n_dim; j++)
		{
			cities[i][j] = 0.0;
		}
		route_best[i] = 0;
	}
	reward_best = INFINITY;
}

void SA_TSP::SetCities(int city, double x[])
{
	//设置城市状态(如：位置)
	for (int i = 0; i < n_dim; i++)
		cities[city][i] = x[i];
}

void SA_TSP::Optimize()
{
	int stay_num = 0, iter = 0;
	int* x_current = new int[n_cities];
	int* x_new = new int[n_cities];
	double y_current = 0.0, y_new = 0.0;
	double p = 0.0;

	InitSol();
	for (int i = 0; i < n_cities; i++)
		x_current[i] = route_best[i];
	y_current = reward_best;

	while ((T > T_min) && (stay_num < stay_max))
	{
		for (int i = 0; i < L; i++)
		{
			NewSol(x_current, x_new);
			y_new = Fitness(x_new);

			// Metropolis
			if (y_new < y_current)
				p = 1.0;
			else
				p = exp(-(y_new - y_current) / T);
			if (p > rand() / double(RAND_MAX))
			{
				for (int j = 0; j < n_cities; j++)
					x_current[j] = x_new[j];
				y_current = y_new;

				if (y_new < reward_best)
				{
					for (int j = 0; j < n_cities; j++)
						route_best[j] = x_new[j];
					reward_best = y_new;
				}
			}
		}

		if (reward_best >= y_current)
			stay_num++;
		else
			stay_num = 0;

		iter++;
		CoolDown();
	}

	delete[] x_current;
	delete[] x_new;
}

void SA_TSP::InitSol()
{
	vector<int> temp_cities(n_cities);
	for (int i = 0; i < n_cities; i++)
		temp_cities[i] = i;

	random_shuffle(temp_cities.begin(), temp_cities.end());
	for (int i = 0; i < n_cities; i++)
	{
		route_best[i] = temp_cities[i];
	}
	reward_best = Fitness(route_best);
}

void SA_TSP::NewSol(int x0[], int x[])
{
	int op = rand() / 3;
	Mutation(x0, x, op);
}

void SA_TSP::Mutation(int x0[], int x[], int op)
{
	//变异
	switch (op)
	{
	case 1:
	{
		//逆转(Reverse)
		/*example:
		chrom_[10] = { 0,1,2,3,4,5,6,7,8,9 };
		p1 = 3, p2 = 6;
		chrom[10]  = { 0,1,2,6,5,4,3,7,8,9 }*/

		int p1 = rand() % (n_cities - 1);
		int p2 = rand() % (n_cities - 1);
		if (p1 > p2)
		{
			int temp = p1;
			p1 = p2;
			p2 = temp;
		}

		for (int i = 0; i < p1; i++)
			x[i] = x0[i];
		for (int i = p1; i <= p2; i++)
			x[i] = x0[p1 + p2 - i];
		for (int i = p2 + 1; i < n_cities; i++)
			x[i] = x0[i];
		break;
	}
	case 2:
	{
		//插入(Insert)
		/*example:
		chrom_[10] = { 0,1,2,3,4,5,6,7,8,9 };
		p1 = 3, p2 = 6;
		chrom[10]  = { 0,1,2,6,3,4,5,7,8,9 }*/

		int p1 = rand() % (n_cities - 1);
		int p2 = rand() % (n_cities - 1);
		if (p1 > p2)
		{
			int temp = p1;
			p1 = p2;
			p2 = temp;
		}

		for (int i = 0; i < p1; i++)
			x[i] = x0[i];
		x[p1] = x0[p2];
		for (int i = p1 + 1; i <= p2; i++)
			x[i] = x0[i - 1];
		for (int i = p2 + 1; i < n_cities; i++)
			x[i] = x0[i];
		break;
	}
	default:
	{
		//交换(Swap)
		/*example:
		chrom_[10] = { 0,1,2,3,4,5,6,7,8,9 };
		p1 = 3, p2 = 6;
		chrom[10]  = { 0,1,2,6,4,5,3,7,8,9 }*/

		for (int i = 0; i < n_cities; i++)
			x[i] = x0[i];

		int p1 = rand() % (n_cities - 1);
		int p2 = rand() % (n_cities - 1);

		x[p1] = x0[p2];
		x[p2] = x0[p1];
	}
	}
}

double SA_TSP::Distance(int city_i, int city_j)
{
	//计算距离
	double dis = sqrt(pow(cities[city_i][0] - cities[city_j][0], 2) +
		pow(cities[city_i][1] - cities[city_j][1], 2));
	return dis;
}

double SA_TSP::Fitness(int route[])
{
	//计算解的适应度值(如：距离)
	double reward = 0.0;
	//for (int i = 0; i < n_cities - 1; i++)
	//	reward += Distance(route[i], route[i + 1]);
	//reward += Distance(route[n_cities - 1], route[0]);
	for (int i = 0; i < n_cities - 1; i++)
		reward += func(cities[route[i]], cities[route[i + 1]]);
	reward += func(cities[route[n_cities - 1]], cities[route[0]]);
	return reward;
}

void SA_TSP::CoolDown()
{
	T = T * c_cool;
	//T = T_max / (1 + log(1 + double(iter)));
}
//@file       : MCTS.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/24

#include "MCTS.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

MCTS::MCTS(double (*func_)(double*, double*), int n_cities_, int iter_max_, double cp_)
{
	func = func_;
	n_cities = n_cities_;
	iter_max = iter_max_;
	cp = cp_;

	srand((int)time(0));

	Init();
}

MCTS::~MCTS()
{
	delete[] prob;
	delete[] route_best;

	for (int i = 0; i < n_cities; i++)
	{
		delete[] cities[i];
		delete[] q_tab[i];
		delete[] n_tab[i];
	}
}

void MCTS::Init()
{
	cities = new double* [n_cities];
	q_tab = new double* [n_cities];
	n_tab = new int* [n_cities];
	prob = new double[n_cities];
	route_best = new int[n_cities];
	
	q_max = 1.0;
	q_min = 0.0;

	for (int i = 0; i < n_cities; i++)
	{
		cities[i] = new double[2];
		for (int j = 0; j < 2; j++)
			cities[i][j] = 0.0;

		q_tab[i] = new double[n_cities];
		n_tab[i] = new int[n_cities];
		for (int j = 0; j < n_cities; j++)
		{
			q_tab[i][j] = 0.0;
			n_tab[i][j] = 0;
		}

		prob[i] = 0.0;
		route_best[i] = -1;
	}
}

void MCTS::CreateCities(int city_id, double pos_x, double pos_y)
{
	cities[city_id][0] = pos_x;
	cities[city_id][1] = pos_y;
}

void MCTS::Optimize()
{
	int* route = new int[n_cities];
	double reward;
	for (int i = 0; i < iter_max; i++)
	{
		Selection(route);
		reward = Reward(route);
		BackPropagation(1 / reward, route);

		if ((reward < reward_best) || (i == 0))
		{
			for (int j = 0; j < n_cities; j++)
				route_best[j] = route[j];
			reward_best = reward;
		}
		
		//cout << reward_best << " ";
	}
}

void MCTS::Selection(int* route)
{
	route[0] = 0;
	for (int i = 1; i < n_cities; i++)
	{
		route[i] = -1;
		Probabilities(route[i - 1], route);
		double r = rand() / double(RAND_MAX);
		for (int j = 0; j < n_cities; j++)
		{
			if (j != 0)
				prob[j] += prob[j - 1];
			if ((prob[j] >= r) || (j == n_cities - 1))
			{
				route[i] = j;
				break;
			}
		}
	}
}

void MCTS::Probabilities(int city, int* route)
{
	int allow;
	double sum = 0.0;

	int index;
	for (index = 0; index < n_cities; index++)
	{
		if (route[index] == -1)
			break;
	}

	for (int i = 0; i < n_cities; i++)
	{
		if (i == city)
		{
			prob[i] = 0.0;
			continue;
		}

		allow = 1;
		for (int j = 0; j < n_cities; j++)
		{
			if (route[j] == -1)
				break;

			if (i == route[j])
			{
				allow = 0;
				break;
			}
		}

		if (allow == 1)
		{
			if (n_tab[index][i] == 0)
				prob[i] = 1;
			else
				prob[i] = (q_tab[index][i] - q_min) / (q_max - q_min) + cp * sqrt(log(n_tab[index - 1][city]) / n_tab[index][i]);
		}
		else
			prob[i] = 0.0;
	}

	for (int i = 0; i < n_cities; i++)
		sum += prob[i];

	for (int i = 0; i < n_cities; i++)
		prob[i] /= sum;
}

double MCTS::Distance(int city_i, int city_j)
{
	double dis = sqrt(pow(cities[city_i][0] - cities[city_j][0], 2) +
		pow(cities[city_i][1] - cities[city_j][1], 2));
	return dis;
}

double MCTS::Reward(int* route)
{
	double reward = 0.0;
	//for (int i = 0; i < n_cities - 1; i++)
	//	reward += Distance(route[i], route[i + 1]);
	//reward += Distance(route[n_cities - 1], route[0]);
	for (int i = 0; i < n_cities - 1; i++)
		reward += func(cities[route[i]], cities[route[i + 1]]);
	reward += func(cities[route[n_cities - 1]], cities[route[0]]);
	return reward;
}

void MCTS::BackPropagation(double q, int * route)
{
	if (q > q_max)
		q_max = q;
	else if(q < q_min)
		q_min = q;

	for (int i = 0; i < n_cities; i++)
	{
		if (q > q_tab[i][route[i]])
			q_tab[i][route[i]] = q;
	}
}
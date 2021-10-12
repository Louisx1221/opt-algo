//@file       : ACO.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#include "ACO.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

ACO::ACO(double (*func_)(double*, double*), int n_cities_, int n_dim_, int n_ants_, int iter_max_,
	double alpha_, double beta_, double q_, double rho_, double tau_max_)
{
	func = func_;
	n_cities = n_cities_;
	n_dim = n_dim_;
	n_ants = n_ants_;
	iter_max = iter_max_;
	alpha = alpha_;
	beta = beta_;
	q = q_;
	rho = rho_;
	tau_max = tau_max_;

	srand((int)time(0));

	Init();
}

ACO::~ACO()
{
	delete[] prob;
	delete[] reward;
	delete[] route_best;

	for (int i = 0; i < n_cities; i++)
	{
		delete[] cities[i];
		delete[] tau[i];
		delete[] delta_tau[i];
	}
	delete[] cities;
	delete[] tau;
	delete[] delta_tau;

	for (int i = 0; i < n_ants; i++)
	{
		delete[] tabu[i];
	}
	delete[] tabu;
}

void ACO::Init()
{
	cities = new double* [n_cities];
	tau = new double* [n_cities];
	delta_tau = new double* [n_cities];
	tabu = new int* [n_ants];
	prob = new double[n_cities];
	reward = new double[n_ants];
	route_best = new int[n_cities];

	for (int i = 0; i < n_cities; i++)
	{
		cities[i] = new double[n_dim];
		for (int j = 0; j < n_dim; j++)
		{
			cities[i][j] = 0.0;
		}

		tau[i] = new double[n_cities];
		delta_tau[i] = new double[n_cities];
		for (int j = 0; j < n_cities; j++) {
			tau[i][j] = tau_max;
			delta_tau[i][j] = 0.0;
		}

		prob[i] = 0.0;
	}

	for (int i = 0; i < n_ants; i++)
	{
		tabu[i] = new int[n_cities];
		for (int j = 0; j < n_cities; j++) {
			tabu[i][j] = -1;
		}
	}
}

void ACO::CreateCities(int city, double x[])
{
	for (int i = 0; i < n_dim; i++)
		cities[city][i] = x[i];
}

void ACO::Optimize()
{
	for (int i = 0; i < iter_max; i++)
	{
		for (int j = 0; j < n_ants; j++)
		{
			ChooseCities(j);
			reward[j] = Reward(tabu[j]);
		}

		int index_best = min_element(reward, reward + n_ants) - reward;
		if ((reward[index_best] < reward_best) || (i == 0))
		{
			for (int k = 0; k < n_cities; k++)
				route_best[k] = tabu[index_best][k];
			reward_best = reward[index_best];
		}
		
		//cout << reward_best << " ";

		UpdatePheromone();
	}
}

void ACO::ChooseCities(int ant)
{
	for (int i = 0; i < n_cities; i++)
		tabu[ant][i] = -1;

	for (int i = 0; i < n_cities; i++)
	{
		if (i == 0)
			tabu[ant][i] = rand() % n_cities;
		else
		{
			Probabilities(tabu[ant][i - 1], tabu[ant]);
			double r = rand() / double(RAND_MAX);
			for (int j = 0; j < n_cities; j++)
			{
				if (j != 0)
					prob[j] += prob[j - 1];
				if ((prob[j] >= r) || (j == n_cities - 1))
				{
					tabu[ant][i] = j;
					break;
				}
			}
		}
	}
}

void ACO::Probabilities(int city, int route[])
{
	int allow;
	double value;
	double sum = 0.0;

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
			value = HeuristicValue(city, i);
			prob[i] = pow(tau[city][i], alpha) * pow(value, beta);
		}
		else
			prob[i] = 0.0;
	}

	for (int i = 0; i < n_cities; i++)
		sum += prob[i];

	for (int i = 0; i < n_cities; i++)
		prob[i] /= sum;
}

double ACO::HeuristicValue(int city_i, int city_j)
{
	//double value = 1 / Distance(city_i, city_j);
	double value = 1 / func(cities[city_i], cities[city_j]);
	return value;
}

double ACO::Distance(int city_i, int city_j)
{
	double dis = sqrt(pow(cities[city_i][0] - cities[city_j][0], 2) +
		pow(cities[city_i][1] - cities[city_j][1], 2));
	return dis;
}

double ACO::Reward(int route[])
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

void ACO::UpdatePheromone()
{
	for (int i = 0; i < n_cities; i++)
	{
		for (int j = 0;j < n_cities; j++)
			delta_tau[i][j] = 0.0;
	}

	for (int i = 0; i < n_ants; i++)
	{
		for (int j = 0; j < n_cities - 1; j++)
		{
			delta_tau[j][j + 1] += q / reward[i];
		}
		delta_tau[n_cities - 1][0] += q / reward[i];
	}

	for (int i = 0; i < n_cities; i++)
	{
		for (int j = 0; j < n_cities; j++)
		{
			tau[i][j] *= (1 - rho);
			tau[i][j] += delta_tau[i][j];
		}
	}
}
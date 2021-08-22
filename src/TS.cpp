//@file       : TS.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/21

#include "TS.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

TS::TS(double (*func_)(double*, double*), int n_cands_, int n_cities_, int iter_max_)
{
	func = func_;
	n_cands = n_cands_;
	n_cities = n_cities_;
	iter_max = iter_max_;

	tabu_len = (int)sqrt((double)n_cities * (n_cities - 1.0) / 2.0);

	srand((int)time(0));

	Init();
}

TS::~TS()
{
	delete[] reward;
	delete[] route_best;

	for (int i = 0; i < n_cities; i++)
	{
		delete[] cities[i];
		delete[] tabu[i];
	}

	for (int i = 0; i < n_cands; i++)
	{
		delete[] cands[i];
	}
}

//public

void TS::Init()
{
	cities = new double* [n_cities];
	tabu = new int* [n_cities];
	cands = new int* [n_cands];
	reward = new double[n_cands];
	route_best = new int[n_cities];

	for (int i = 0; i < n_cities; i++)
	{
		cities[i] = new double[2];
		for (int j = 0; j < 2; j++)
		{
			cities[i][j] = 0.0;
		}
		tabu[i] = new int[n_cities];
		for (int j = 0; j < n_cities; j++) {
			tabu[i][j] = 0;
		}
		route_best[i] = 0;
	}

	for (int i = 0; i < n_cands; i++)
	{
		cands[i] = new int[n_cities];
		for (int j = 0; j < n_cities; j++) {
			cands[i][j] = 0;
		}
		reward[i] = 0;
	}
}

void TS::CreateCities(int city_id, double pos_x, double pos_y)
{
	cities[city_id][0] = pos_x;
	cities[city_id][1] = pos_y;
}

//private

void TS::Optimize()
{
	InitialSolution();
	int r[2] = { 0, 0 };

	for (int i = 0; i < iter_max; i++)
	{
		Neighborhood();
		
		int index_best = min_element(reward, reward + n_cities) - reward;
		if ((reward[index_best] < reward_best) || (i == 0))
		{
			int k = 0;
			for (int j = 0; j < n_cities; j++)
			{
				if (route_best[j] != cands[index_best][j])
				{
					route_best[j] = cands[index_best][j];
					r[k] = j;
					k++;
				}
			}
			tabu[r[0]][r[1]] = tabu_len;
			tabu[r[1]][r[0]] = tabu_len;
			reward_best = reward[index_best];
		}

		//cout << reward_best << " ";

		UpdateTabuList();
	}
}

void TS::InitialSolution()
{
	for (int i = 0; i < n_cities; i++)
	{
		bool flag;
		while (true)
		{
			flag = true;
			route_best[i] = rand() % n_cities;
			for (int j = 0; j < i; j++)
			{
				if (route_best[i] == route_best[j])
				{
					flag = false;
					break;
				}
			}
			if (flag == true)
				break;
		}
	}
}

void TS::Neighborhood()
{
	int r1, r2;
	for (int i = 0; i < n_cands; i++)
	{
		for (int j = 0; j < n_cities; j++)
			cands[i][j] = route_best[j];
		r1 = rand() % n_cities;
		r2 = rand() % n_cities;
		cands[i][r1] = route_best[r2];
		cands[i][r2] = route_best[r1];
		if (tabu[r1][r2] > 0)
			reward[i] = INFINITY;
		else
			reward[i] = Reward(cands[i]);
	}
}

double TS::Distance(int city_i, int city_j)
{
	double dis = sqrt(pow(cities[city_i][0] - cities[city_j][0], 2) +
		pow(cities[city_i][1] - cities[city_j][1], 2));
	return dis;
}

double TS::Reward(int route[])
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

void TS::UpdateTabuList()
{
	for (int i = 0; i < n_cities; i++)
	{
		for (int j = 0; j < n_cities; j++)
		{
			if (tabu[i][j] > 0)
				tabu[i][j]--;
		}
	}
}
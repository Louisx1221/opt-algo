//@file       : MCTS.h
//@autor      : github.com/louisx1221
//@date       : 2021/08/24

#pragma once

class MCTS
{
public:
	MCTS(double (*func_)(double*, double*), int n_cities_, int iter_max_ = 1e5, double cp_ = 2);
	virtual ~MCTS();

	void CreateCities(int city, double pos_x, double pos_y);
	void Optimize();

	double reward_best;
	int* route_best;

private:
	void Init();
	void Selection(int* route);
	void Probabilities(int city, int* route);
	double Distance(int city_i, int city_j);
	double Reward(int* route);
	void BackPropagation(double q, int* route);

	double (*func)(double*, double*);

	int n_cities, iter_max;
	double cp, q_max, q_min;

	double** q_tab, **cities;
	int** n_tab;
	double* prob;
};
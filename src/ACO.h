//@file       : ACO.h
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#pragma once

class ACO
{
public:
	ACO(double (*func_)(double*, double*), int n_cities_, int n_dim_ = 2, int n_ants_ = 50, int iter_max_ = 100,
		double alpha_ = 1.0, double beta_ = 2.0, double q_ = 1.0, double rho_ = 0.1, double tau_max_ = 1.0);
	virtual ~ACO();

	void CreateCities(int city, double x[]);
	void Optimize();

	double tau_max, reward_best;
	int* route_best;

private:
	void Init();
	void ChooseCities(int ant);
	void Probabilities(int city_i, int route[]);
	double HeuristicValue(int city_i, int city_j);
	double Distance(int city_i, int city_j);
	double Reward(int route[]);
	void UpdatePheromone();

	double (*func)(double*, double*);

	int n_cities, n_dim, n_ants, iter_max;
	double alpha, beta, q, rho;

	double** tau, ** delta_tau, **cities;
	int** tabu;
	double* prob, * reward;
};
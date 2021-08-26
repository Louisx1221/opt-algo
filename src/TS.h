//@file       : TS.h
//@autor      : github.com/louisx1221
//@date       : 2021/08/21

#pragma once

class TS
{
public:
	TS(double (*func_)(double*, double*), int n_cities_, int n_dim_= 2, int n_cands_ = 50, int iter_max_ = 100);
	virtual ~TS();

	void CreateCities(int city, double x[]);
	void Optimize();

	double reward_best;
	int* route_best;

private:
	void Init();
	void InitialSolution();
	void Neighborhood();
	double Distance(int city_i, int city_j);
	double Reward(int route[]);
	void UpdateTabuList();

	double (*func)(double*, double*);

	int n_cities, n_dim, n_cands, iter_max, tabu_len;

	double**cities;
	int** tabu, ** cands;
	double* reward;
};
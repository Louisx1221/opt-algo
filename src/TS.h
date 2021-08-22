//@file       : TS.h
//@autor      : github.com/louisx1221
//@date       : 2021/08/21

#pragma once

class TS
{
public:
	TS(double (*func_)(double*, double*), int n_cands_, int n_cities_, int iter_max_ = 100);
	virtual ~TS();

	void CreateCities(int city, double pos_x, double pos_y);
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

	int n_cands, n_cities, iter_max, tabu_len;

	double**cities;
	int** tabu, ** cands;
	double* reward;
};
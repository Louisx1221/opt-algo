//@file       : SA.h
//@autor      : github.com/louisx1221
//@date       : 2021/10/19

#pragma once

class SA
{
public:
	SA(double (*func_)(double*), int n_dim_, double* lb_, double* ub_,
		double T_max_ = 100.0, double T_min_ = 1e-7, double c_cool_ = 0.8, int L_ = 300, int stay_max_ = 100);
	virtual ~SA();

	void Optimize();

	double* x_best;
	double y_best;


private:
	void Init();
	void NewSol(double x0[], double x[]);
	void CoolDown();

	double (*func)(double*);

	int n_dim, L, stay_max;
	double T, T_max, T_min, c_cool;
	double* lb, * ub;
};

class SA_TSP
{
public:
	SA_TSP(double (*func_)(double*, double*), int n_cities_, int n_dim_ = 2,
		double T_max_ = 100.0, double T_min_ = 1e-7, double c_cool_ = 0.8, int L_ = 300, int stay_max_ = 100);
	virtual ~SA_TSP();

	void SetCities(int city, double x[]);
	void Optimize();

	int* route_best;
	double reward_best;


private:
	void Init();
	void InitSol();
	void NewSol(int x0[], int x[]);
	void Mutation(int x0[], int x[], int op);
	double Distance(int city_i, int city_j);
	double Fitness(int route[]);
	void CoolDown();

	double (*func)(double*, double*);

	int n_cities, n_dim, L, stay_max;
	double T, T_max, T_min, c_cool;
	double** cities;
};
//@file       : GA.h
//@autor      : github.com/louisx1221
//@date       : 2021/08/18

#pragma once

class GA
{
public:
	GA(double (*func_)(double*), int n_dim, double* lb_, double* ub_, int chrom_len_ = 10,
		int pop_ = 40, int iter_max_ = 100, double pc_ = 0.8, double pm_ = 0.2);
	virtual ~GA();

	void Optimize();

	double* best_x;
	double best_y;

private:
	void Init();
	void Clone(int* chrom_, int* chrom);
	void Crossover(int* chrom1_, int* chrom2_, int* chrom1, int* chrom2);
	void Mutation(int* chrom_, int* chrom);
	double* Decoding(int* chrom);
	void Selection();

	double (*func)(double*);

	int n_dim, chrom_len, pop, iter_max;
	double pc, pm;
	double* lb, * ub, * bit_len, * fit_val;
	int** chroms;
};

class GA_TSP
{
public:
	GA_TSP(double (*func_)(double*, double*), int chrom_len_, int n_dim_ = 2,
		int pop_ = 40, int iter_max_ = 100, double pc_ = 0.8, double pm_ = 0.2);
	virtual ~GA_TSP();

	void SetCities(int city, double x[]);
	void Optimize();

	double reward_best;
	int* route_best;

private:
	void Init();
	void InitPop();
	void Clone(int* chrom_, int* chrom);
	void Crossover(int* chrom1_, int* chrom2_, int* chrom1, int* chrom2, int op = 0);
	void Mutation(int* chrom_, int* chrom, int op = 0);
	double Distance(int city_i, int city_j);
	double Fitness(int route[]);
	void Selection();

	double (*func)(double*, double*);

	int n_dim, chrom_len, pop, iter_max;
	double pc, pm;

	double* fit_val;
	double** cities;
	int** chroms;
};
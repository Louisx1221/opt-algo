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
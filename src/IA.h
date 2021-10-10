//@file       : IA.h
//@autor      : github.com/louisx1221
//@date       : 2021/08/19

#pragma once

class IA
{
public:
	IA(double (*func_)(double*), int n_dim, double* lb_, double* ub_, int chrom_len_ = 10,
		int pop_ = 40, int iter_max_ = 100, double pc_ = 0.8, double pm_ = 0.2,
		double ps_ = 0.5, double ws_ = 0.05);
	virtual ~IA();

	void Optimize();

	double* x_best;
	double y_best;

private:
	void Init();
	void Clone(int* chrom_, int* chrom);
	void Crossover(int* chrom1_, int* chrom2_, int* chrom1, int* chrom2);
	void Mutation(int* chrom_, int* chrom);
	void Decoding(int* chrom, double* x);
	void Selection();
	void Encourage();

	double (*func)(double*);

	int n_dim, chrom_len, pop, iter_max;
	double pc, pm, ps, ws;
	double* lb, * ub, * bit_len, * fit_val, * aff, * sim, * enc;
	int** chroms;
};
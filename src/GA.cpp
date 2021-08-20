//@file       : GA.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/18

#include "GA.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>

GA::GA(double (*func_)(double*), int n_dim_, double* lb_, double* ub_, int chrom_len_,
	int pop_, int iter_max_, double pc_, double pm_)
{
	func = func_;
	n_dim = n_dim_;
	chrom_len = chrom_len_;
	pop = pop_;
	iter_max = iter_max_;
	pc = pc_;
	pm = pm_;

	lb = new double[n_dim];
	ub = new double[n_dim];
	bit_len = new double[n_dim];
	for (int i = 0; i < n_dim; i++)
	{
		lb[i] = lb_[i];
		ub[i] = ub_[i];
		bit_len[i] = (ub[i] - lb[i]) / (pow(2, chrom_len) - 1);
	}

	srand((int)time(0));

	Init();
}

GA::~GA()
{
	for (int i = 0; i < pop; i++)
	{
		delete[] chroms[i];
	}
}

void GA::Init()
{
	chroms = new int* [pop * 4];
	fit_val = new double[pop * 4];

	for (int i = 0; i < pop * 4; i++)
	{
		chroms[i] = new int[chrom_len * n_dim];
		if (i < pop)
		{
			for (int j = 0; j < chrom_len * n_dim; j++)
				chroms[i][j] = rand() % 2;
		}

		fit_val[i] = 0.0;
	}
}

void GA::Optimize()
{
	int r;
	double* x;
	for (int i = 0; i < iter_max; i++)
	{
		for (int j = 0; j < pop; j++)
		{
			for (int k = 1; k < 4; k++)
				Clone(chroms[j], chroms[pop * k + j]);
		}

		for (int j = 0; j < pop; j++)
		{
			if (rand() / double(RAND_MAX) < pc)
			{
				r = rand() % pop;
				Crossover(chroms[j], chroms[r], chroms[pop + j], chroms[2 * pop + r]);
			}
			if (rand() / double(RAND_MAX) < pm)
				Mutation(chroms[j], chroms[3 * pop + j]);
		}

		for (int j = 0; j < pop * 4; j++)
		{
			x = Decoding(chroms[j]);
			fit_val[j] = func(x);
		}

		Selection();
	}

	best_x = Decoding(chroms[0]);
	best_y = fit_val[0];
}

void GA::Clone(int* chrom_, int* chrom)
{
	for (int i = 0; i < chrom_len * n_dim; i++)
		chrom[i] = chrom_[i];
}

void GA::Crossover(int* chrom1_, int* chrom2_, int* chrom1, int* chrom2)
{
	int r = rand() % (chrom_len * n_dim);

	for (int i = 0; i < r; i++)
	{
		chrom1[i] = chrom1_[i];
		chrom2[i] = chrom2_[i];
	}
	for (int i = r; i < chrom_len * n_dim; i++)
	{
		chrom1[i] = chrom2_[i];
		chrom2[i] = chrom1_[i];
	}
}

void GA::Mutation(int* chrom_, int* chrom)
{
	int r = rand() % (chrom_len * n_dim);
	
	for (int i = 0; i < chrom_len * n_dim; i++)
		chrom[i] = chrom_[i];

	chrom[r] = (chrom_[r] + 1) % 2;
}

double* GA::Decoding(int* chrom)
{
	double* x = new double[n_dim];
	for (int i = 0; i < n_dim; i++)
	{
		x[i] = 0.0;
		for (int j = 0; j < chrom_len; j++)
		{
			x[i] += chrom[chrom_len * i + j] * pow(2, j);
		}
		x[i] *= bit_len[i];
		x[i] += lb[i];
	}
	return x;
}

void GA::Selection()
{
	int temp1;
	double temp2;
	for (int i = 0; i < pop; i++)
	{
		for (int j = pop * 4 - 1; j > i; j--)
		{
			if (fit_val[j] > fit_val[j - 1])
			{
				for (int k = 0; k < chrom_len * n_dim; k++)
				{
					temp1 = chroms[j][k];
					chroms[j][k] = chroms[j - 1][k];
					chroms[j - 1][k] = temp1;
				}
				temp2 = fit_val[j];
				fit_val[j] = fit_val[j - 1];
				fit_val[j - 1] = temp2;
			}
		}
	}
}

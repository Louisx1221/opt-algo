//@file       : IA.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/19

#include "IA.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

IA::IA(double (*func_)(double*), int n_dim_, double* lb_, double* ub_, int chrom_len_,
	int pop_, int iter_max_, double pc_, double pm_, double ps_, double ws_)
{
	func = func_;
	n_dim = n_dim_;
	chrom_len = chrom_len_;
	pop = pop_;
	iter_max = iter_max_;
	pc = pc_;
	pm = pm_;
	ps = ps_;
	ws = ws_;

	lb = new double[n_dim];
	ub = new double[n_dim];
	bit_len = new double[n_dim];
	for (int i = 0; i < n_dim; i++)
	{
		lb[i] = lb_[i];
		ub[i] = ub_[i];
		bit_len[i] = (ub[i] - lb[i]) / pow(2, chrom_len);
	}

	srand((int)time(0));

	Init();
}

IA::~IA()
{
	for (int i = 0; i < pop; i++)
	{
		delete[] chroms[i];
	}
	delete[] chroms;
	delete[] fit_val;
	delete[] aff;
	delete[] sim;
	delete[] enc;
	delete[] x_best;
	delete[] lb;
	delete[] ub;
	delete[] bit_len;
}

void IA::Init()
{
	chroms = new int* [pop * 4];
	fit_val = new double[pop * 4];
	aff = new double[pop * 4];
	sim = new double[pop * 4];
	enc = new double[pop * 4];
	x_best = new double[n_dim];
	y_best = INFINITY;

	for (int i = 0; i < pop * 4; i++)
	{
		chroms[i] = new int[chrom_len * n_dim];
		if (i < pop)
		{
			for (int j = 0; j < chrom_len * n_dim; j++)
				chroms[i][j] = rand() % 2;
		}

		fit_val[i] = 0.0;
		aff[i] = 0.0;
		sim[i] = 0.0;
		enc[i] = 0.0;
	}
}

void IA::Optimize()
{
	int r;
	double* x = new double[n_dim];
	for (int i = 0; i < n_dim; i++)
		x[i] = 0.0;

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
			Decoding(chroms[j], x);
			fit_val[j] = func(x);
		}

		Encourage();
		Selection();
	}

	Decoding(chroms[0], x_best);
	y_best = fit_val[0];

	delete[] x;
}

void IA::Clone(int* chrom_, int* chrom)
{
	for (int i = 0; i < chrom_len * n_dim; i++)
		chrom[i] = chrom_[i];
}

void IA::Crossover(int* chrom1_, int* chrom2_, int* chrom1, int* chrom2)
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

void IA::Mutation(int* chrom_, int* chrom)
{
	int r = rand() % (chrom_len * n_dim);
	
	for (int i = 0; i < chrom_len * n_dim; i++)
		chrom[i] = chrom_[i];

	chrom[r] = (chrom_[r] + 1) % 2;
}

void IA::Decoding(int* chrom, double* x)
{
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
}

void IA::Selection()
{
	int temp1;
	double temp2;
	for (int i = 0; i < pop; i++)
	{
		for (int j = pop * 4 - 1; j > i; j--)
		{
			if (enc[j] < enc[j - 1])
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
				temp2 = enc[j];
				enc[j] = enc[j - 1];
				enc[j - 1] = temp2;
			}
		}
	}
}

void IA::Encourage()
{
	int sim_sum = 0, aff_sum = 0;
	int index_max = max_element(fit_val, fit_val + pop * 4) - fit_val;
	int index_min = min_element(fit_val, fit_val + pop * 4) - fit_val;
	double val_min = fit_val[index_min];
	double range = fit_val[index_max] - fit_val[index_min];

	for (int i = 0; i < pop * 4; i++)
	{
		//Min-Max Normalization
		aff[i] = (fit_val[i] - val_min) / range;
		aff_sum += aff[i];

		//Hamming Distance
		sim[i] = 0;
		for (int j = 0; j < pop * 4; j++)
		{
			for (int k = 0; k < chrom_len * n_dim; k++)
				sim[i] += chroms[i][k] ^ chroms[j][k];
		}
		if ((double)sim[i] < ps * chrom_len * n_dim)
			sim[i] += 1;
		sim_sum += sim[i];
	}

	for (int i = 0; i < pop * 4; i++)
	{
		enc[i] = aff[i] / aff_sum - ws * sim[i] / sim_sum;
	}
}
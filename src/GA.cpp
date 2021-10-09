//@file       : GA.cpp
//@autor      : github.com/louisx1221
//@date       : 2021/08/18

#include "GA.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

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
	delete[] chroms;
	delete[] fit_val;
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

GA_TSP::GA_TSP(double (*func_)(double*, double*), int chrom_len_, int n_dim_,
	int pop_, int iter_max_, double pc_, double pm_, int opc_, int opm_)
{
	func = func_;// 收益函数(适应值, 如：距离)
	chrom_len = chrom_len_;//序列长(如：城市数)
	n_dim = n_dim_;
	pop = pop_;
	iter_max = iter_max_;
	pc = pc_;
	pm = pm_;
	opc = opc_;
	opm = opm_;

	srand((int)time(0));

	Init();
}

GA_TSP::~GA_TSP()
{
	for (int i = 0; i < pop * 4; i++)
	{
		delete[] chroms[i];
	}
	delete[] chroms;
	delete[] fit_val;
	delete[] route_best;

	for (int i = 0; i < chrom_len; i++)
	{
		delete[] cities[i];
	}
	delete[] cities;
}

void GA_TSP::Init()
{
	cities = new double* [chrom_len];
	chroms = new int* [pop * 4];
	fit_val = new double[pop * 4];
	route_best = new int[chrom_len];

	for (int i = 0; i < chrom_len; i++)
	{
		cities[i] = new double[n_dim];
		for (int j = 0; j < n_dim; j++)
		{
			cities[i][j] = 0.0;
		}
		route_best[i] = 0;
	}
	for (int i = 0; i < pop * 4; i++)
	{
		chroms[i] = new int[chrom_len];
		for (int j = 0; j < chrom_len; j++)
			chroms[i][j] = 0;

		fit_val[i] = 0.0;
	}
}

void GA_TSP::SetCities(int city, double x[])
{
	//设置城市状态(如：位置)
	for (int i = 0; i < n_dim; i++)
		cities[city][i] = x[i];
}

void GA_TSP::InitPop()
{
	//生成初始种群
	//①一个城市序列
	vector<int> temp_cities(chrom_len);
	for (int i = 0; i < chrom_len; i++)
		temp_cities[i] = i;

	//②打乱后生成初始种群
	for (int i = 0; i < pop * 4; i++)
	{
		random_shuffle(temp_cities.begin(), temp_cities.end());
		for (int j = 0; j < chrom_len; j++)
		{
			chroms[i][j] = temp_cities[j];
		}
	}

	//显示
	//cout << "init_population:" << endl;
	//for (int i = 0; i < pop; i++)
	//{
	//	for (int j = 0; j < chrom_len; j++)
	//	{
	//		cout << chroms[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;
}

void GA_TSP::Optimize()
{
	//初始种群
	InitPop();
	//迭代优化
	int r;
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
				Crossover(chroms[j], chroms[r], chroms[pop + j], chroms[2 * pop + r], opc);
			}
			if (rand() / double(RAND_MAX) < pm)
				Mutation(chroms[j], chroms[3 * pop + j], opm);
		}

		for (int j = 0; j < pop * 4; j++)
			fit_val[j] = Fitness(chroms[j]);

		Selection();
	}

	for (int i = 0; i < chrom_len; i++)
		route_best[i] = chroms[0][i];
	reward_best = fit_val[0];
}

void GA_TSP::Clone(int* chrom_, int* chrom)
{
	//克隆
	for (int i = 0; i < chrom_len; i++)
		chrom[i] = chrom_[i];
}

void GA_TSP::Crossover(int* chrom1_, int* chrom2_, int* chrom1, int* chrom2, int op)
{
	//交叉
	switch (op)
	{
	case 1:
	{
		//顺序交叉(Order Crossover, OX)
		/*example:
		chrom1_[10] = { 0,1,2,3,4,5,6,7,8,9 }, chrom2_[10] = { 0,5,4,6,9,2,1,7,8,3 };
		p1 = 3, p2 = 6;
		chrom1[10]  = { 0,3,4,6,9,2,1,5,7,8 }, chrom2[10]  = { 0,2,9,3,4,5,6,1,7,8 };*/

		int temp = 0;
		//随机交叉点
		int p1 = rand() % (chrom_len - 1);
		int p2 = rand() % (chrom_len - 1);
		if (p1 > p2)
		{
			temp = p1;
			p1 = p2;
			p2 = temp;
		}
		//初始化子代基因
		for (int i = p1; i <= p2; i++)
		{
			chrom1[i] = chrom2_[i];
			chrom2[i] = chrom1_[i];
		}
		//交叉操作
		int pos = 0;
		bool flag = true;
		for (int i = 0; i < chrom_len; i++)
		{
			if (pos == p1)
				pos += (p2 - p1 + 1);
			flag = true;
			for (int j = p1; j <= p2; j++)
			{
				if (chrom1_[i] == chrom2_[j])
				{
					flag = false;
					continue;
				}
			}
			if (flag)
			{
				chrom1[pos] = chrom1_[i];
				pos++;
			}
		}
		pos = 0;
		for (int i = 0; i < chrom_len; i++)
		{
			if (pos == p1)
				pos += (p2 - p1 + 1);
			flag = true;
			for (int j = p1; j <= p2; j++)
			{
				if (chrom2_[i] == chrom1_[j])
				{
					flag = false;
					continue;
				}
			}
			if (flag)
			{
				chrom2[pos] = chrom2_[i];
				pos++;
			}
		}
		break;
	}
	default:
	{
		//部分匹配交叉(Partial-Mapped Crossover, PMX)
		/*example:
		chrom1_[10] = { 0,1,2,3,4,5,6,7,8,9 }, chrom2_[10] = { 0,5,4,6,9,2,1,7,8,3 };
		p1 = 3, p2 = 6;
		chrom1[10]  = { 0,3,5,6,9,2,1,7,8,4 }, chrom2[10]  = { 0,2,9,3,4,5,6,7,8,1 };*/

		int temp = 0;
		//初始化子代基因
		for (int i = 0; i < chrom_len; i++)
		{
			chrom1[i] = chrom1_[i];
			chrom2[i] = chrom2_[i];
		}
		//随机交叉点
		int p1 = rand() % (chrom_len - 1);
		int p2 = rand() % (chrom_len - 1);
		if (p1 > p2)
		{
			temp = p1;
			p1 = p2;
			p2 = temp;
		}
		//交叉操作
		int val1 = 0, val2 = 0, pos1 = 0, pos2 = 0;
		for (int i = p1; i <= p2; i++)
		{
			val1 = chrom1_[i];
			val2 = chrom2_[i];
			for (int j = 0; j < chrom_len; j++)
			{
				if (chrom1[j] == val2)
				{
					pos1 = j;
					break;
				}
			}
			for (int j = 0; j < chrom_len; j++)
			{
				if (chrom2[j] == val1)
				{
					pos2 = j;
					break;
				}
			}
			temp = chrom1[i];
			chrom1[i] = chrom1[pos1];
			chrom1[pos1] = temp;
			temp = chrom2[i];
			chrom2[i] = chrom2[pos2];
			chrom2[pos2] = temp;
		}
	}
	}
}

void GA_TSP::Mutation(int* chrom_, int* chrom, int op)
{
	//变异
	switch (op)
	{
	case 1:
	{
		//逆转(Reverse)
		/*example:
		chrom_[10] = { 0,1,2,3,4,5,6,7,8,9 };
		p1 = 3, p2 = 6;
		chrom[10]  = { 0,1,2,6,5,4,3,7,8,9 }*/

		int p1 = rand() % (chrom_len - 1);
		int p2 = rand() % (chrom_len - 1);
		if (p1 > p2)
		{
			int temp = p1;
			p1 = p2;
			p2 = temp;
		}

		for (int i = 0; i < p1; i++)
			chrom[i] = chrom_[i];
		for (int i = p1; i <= p2; i++)
			chrom[i] = chrom_[p1 + p2 - i];
		for (int i = p2 + 1; i < chrom_len; i++)
			chrom[i] = chrom_[i];
		break;
	}
	case 2:
	{
		//插入(Insert)
		/*example:
		chrom_[10] = { 0,1,2,3,4,5,6,7,8,9 };
		p1 = 3, p2 = 6;
		chrom[10]  = { 0,1,2,6,3,4,5,7,8,9 }*/

		int p1 = rand() % (chrom_len - 1);
		int p2 = rand() % (chrom_len - 1);
		if (p1 > p2)
		{
			int temp = p1;
			p1 = p2;
			p2 = temp;
		}

		for (int i = 0; i < p1; i++)
			chrom[i] = chrom_[i];
		chrom[p1] = chrom_[p2];
		for (int i = p1 + 1; i <= p2; i++)
			chrom[i] = chrom_[i - 1];
		for (int i = p2 + 1; i < chrom_len; i++)
			chrom[i] = chrom_[i];
		break;
	}
	default:
	{
		//交换(Swap)
		/*example:
		chrom_[10] = { 0,1,2,3,4,5,6,7,8,9 };
		p1 = 3, p2 = 6;
		chrom[10]  = { 0,1,2,6,4,5,3,7,8,9 }*/

		for (int i = 0; i < chrom_len; i++)
			chrom[i] = chrom_[i];

		int p1 = rand() % (chrom_len - 1);
		int p2 = rand() % (chrom_len - 1);

		chrom[p1] = chrom_[p2];
		chrom[p2] = chrom_[p1];
	}
	}
}

double GA_TSP::Distance(int city_i, int city_j)
{
	//计算距离
	double dis = sqrt(pow(cities[city_i][0] - cities[city_j][0], 2) +
		pow(cities[city_i][1] - cities[city_j][1], 2));
	return dis;
}

double GA_TSP::Fitness(int route[])
{
	//计算解的适应度值(如：距离)
	double reward = 0.0;
	//for (int i = 0; i < n_cities - 1; i++)
	//	reward += Distance(route[i], route[i + 1]);
	//reward += Distance(route[n_cities - 1], route[0]);
	for (int i = 0; i < chrom_len - 1; i++)
		reward += func(cities[route[i]], cities[route[i + 1]]);
	reward += func(cities[route[chrom_len - 1]], cities[route[0]]);
	return reward;
}

void GA_TSP::Selection()
{
	//选择
	int temp1;
	double temp2;
	for (int i = 0; i < pop; i++)
	{
		for (int j = pop * 4 - 1; j > i; j--)
		{
			if (fit_val[j] < fit_val[j - 1])
			{
				for (int k = 0; k < chrom_len; k++)
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
//Simulated Annealing
#include "../Simulated Annealing/SA.h"
#define _CRT_SECURE_NO_WARNINGS 1
using namespace std;

void Simulated_Annealing::in()
{
	ifstream fin("in.txt");
	for (i = 0; i < C; i++)
		for (j = 0; j < 2; j++)
			fin >> citys_position[i][j];
	fin.close();
}
void Simulated_Annealing::SA()
{
	citys_generate();
	f1 = Fitness(Current_Solution);
	while (T > T_end)
	{
		All_solutions.push_back({});
		All_fitness.push_back({});
		for (i = 0; i < L; i++)
		{
			for (j = 0; j < C; j++)
			{
				Current_copy[j] = Current_Solution[j];
			}
			Swap(Current_copy);
			f2 = Fitness(Current_copy);
			df = f1 - f2;
			if (df < 0)
			{
				double r;
				r = (double)rand() / RAND_MAX;
				if (r < exp(df / T))
				{
					for (j = 0; j < C; j++)
					{
						Current_Solution[j] = Current_copy[j];
					}
					f1 = f2;
				}
			}
			else
			{
				for (j = 0; j < C; j++)
				{
					Current_Solution[j] = Current_copy[j];
				}
				f1 = f2;
			}
		}
		for (int j = 0; j < C; j++)
		{
			All_solutions[count].push_back(Current_Solution[j]);
		}
		All_fitness[count].push_back(f1);

		T *= q;
		count++;
	}
}
void Simulated_Annealing::citys_generate()
{
	
	for (int i = 0; i < C; i++)
	{
		temp_city.push_back(i + 1);
	}
	random_shuffle(temp_city.begin(), temp_city.end());
	for (int j = 0; j < temp_city.size(); j++)
	{
		Current_Solution[j] = temp_city[j];
	}
}
void Simulated_Annealing::Swap(int* input_solution)
{
	int point1 = rand() % C;
	int point2 = rand() % C;
	while (point1 == point2)
	{
		point2 = rand() % C;
	}
	swap(input_solution[point1], input_solution[point2]);
}
double Simulated_Annealing::Fitness(int* input_solution)
{
	double cost = 0;
	for (int j = 0; j < C - 1; j++)
	{
		cost += distance(citys_position[input_solution[j] - 1], citys_position[input_solution[j + 1] - 1]);
	}
	cost += distance(citys_position[input_solution[C - 1] - 1], citys_position[input_solution[0] - 1]);
	return cost;
}
double Simulated_Annealing::distance(double* city1, double*city2)
{
	double x1 = city1[0];
	double y1 = city1[1];
	double x2 = city2[0];
	double y2 = city2[1];
	double dist = pow((pow((x1 - x2), 2) + pow((y1 - y2), 2)), 0.5);
	return dist;
}
void Simulated_Annealing::out()
{
	ofstream fout;
	fout.open("out.txt");
	fout << "每次退火内部迭代的最终解：" << endl;
	for (int i = 0; i < All_solutions.size(); i++)
	{
		for (int j = 0; j <All_solutions[i].size(); j++)
		{
			fout << All_solutions[i][j] << "—>";
		}
		fout << All_solutions[i][0] << endl;
		fout << "适应值为：";
		for (int j = 0; j < All_fitness[i].size(); j++)
		{
			fout << All_fitness[i][j] << "  ";
		}
		fout << endl << endl;

	}
	fout.close();
}
void Simulated_Annealing::get_All_solutions()
{
	cout << "每次退火内部迭代的最终解：" << endl;
	for (int i = 0; i < All_solutions.size(); i++)
	{
		for (int j = 0; j < All_solutions[i].size(); j++)
		{
			cout << All_solutions[i][j] << "—>";
		}
		cout << All_solutions[i][0] << endl;
		cout << "适应值为：";
		for (int j = 0; j < All_fitness[i].size(); j++)
		{
			cout << All_fitness[i][j] << "  ";
		}
		cout << endl << endl;
	}
}
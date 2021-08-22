#pragma once
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include<string>
#include<iterator>
#include<iostream>
using namespace std;

class Simulated_Annealing
{
public:
	void SA_set(int T0,int T_end,double q,int L,int C);
	void SA();//SA主体功能
	void in();//输入文件
	int getCount() { return count; }//给外部调用count值
	int getCurrent_Solution(int k)//给外部调用当前解
	{
		return Current_Solution[k];
	}
	int getF1() { return f1; }//给外部调用f1值
	void out();//输出文件
	void get_All_solutions();//输出所有解


private:
	int i, j;
	int T0, T_end, L, C;
	double T,q;
	double f1, f2, df;
	int count = 0;
	void citys_generate();
	void Swap(vector<int> &input_solution);
	double Fitness(vector<int> &input_solution);
	double distance(double* city1, double* city2);
	vector<vector<double>> All_solutions;
	vector<vector<double>> All_fitness;
	vector<int> temp_city;
	vector<vector<double>> citys_position;
	vector<int> Current_copy;
	vector<int> Current_Solution;
};
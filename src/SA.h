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
	void SA();//SA���幦��
	void in();//�����ļ�
	int getCount() { return count; }//���ⲿ����countֵ
	int getCurrent_Solution(int k)//���ⲿ���õ�ǰ��
	{
		return Current_Solution[k];
	}
	int getF1() { return f1; }//���ⲿ����f1ֵ
	void out();//����ļ�
	void get_All_solutions();//������н�


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
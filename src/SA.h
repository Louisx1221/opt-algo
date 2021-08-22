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
//参数设置
#define T0 1000				//初始温度
#define T_end 20			//退出温度（环境温度）
#define q 0.99				//退火系数
#define L 5					//每个温度时的迭代次数，即链长
#define C 10				//城市数量
//输入文件："in.txt"
class Simulated_Annealing
{
public:
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
	double citys_position[C][2];
	int Current_copy[C];
	int Current_Solution[C];
	double T = T0;
	double f1, f2, df;
	int count = 0;
	void citys_generate();
	void Swap(int* input_solution);
	double Fitness(int* input_solution);
	double distance(double* city1, double* city2);
	vector<vector<double>> All_solutions;
	vector<vector<double>> All_fitness;
	vector<int> temp_city;
	
};
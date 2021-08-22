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
//��������
#define T0 1000				//��ʼ�¶�
#define T_end 20			//�˳��¶ȣ������¶ȣ�
#define q 0.99				//�˻�ϵ��
#define L 5					//ÿ���¶�ʱ�ĵ���������������
#define C 10				//��������
//�����ļ���"in.txt"
class Simulated_Annealing
{
public:
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
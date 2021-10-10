//@file       : SA.h
//@autor      : github.com/louisx1221
//@date       : 2021/10/19

#pragma once

class SA
{
public:
	SA(double (*func_)(double*), int n_dim_, double* lb_, double* ub_,
		double T_max_ = 100.0, double T_min_ = 1e-7, double c_cool_ = 0.8, int L_ = 300, int stay_max_ = 100);
	virtual ~SA();

	void Optimize();

	double* x_best;
	double y_best;


private:
	void Init();
	void NewSol(double x[], double y[]);
	void CoolDown();

	double (*func)(double*);

	int n_dim, stay_max;
	double T, T_max, T_min, L, c_cool;
	double* lb, * ub;
};
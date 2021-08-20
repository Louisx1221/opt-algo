//@file       : PSO.h
//@autor      : github.com/louisx1221
//@date       : 2021/08/17

#pragma once

class PSO
{
public:
	PSO(double (*func_)(double*), int n_dim_, double lb_[], double ub_[],
		int pop_ = 40, int iter_max_ = 100, double w_ = 0.8, double cp_ = 0.5, double cg_ = 0.5);
	virtual ~PSO();

	void Optimize();

	double *gbest_x;
	double gbest_y;

private:
	void Init();
	void UpdateVecPos();
	void Fitness();
	void UpdateBest();

	double (*func)(double*);

	double w, cp, cg, verbose;
	int pop, n_dim, iter_max, gbest;

	double** x, ** v, ** pbest_x;
	double* lb, * ub, * v_high, * y, * pbest_y;
};
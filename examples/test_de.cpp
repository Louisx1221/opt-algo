#include <iostream>
#include "DE.h"

using namespace std;
using namespace de;


double obj(dvec x){
    // Beale function
    //return pow((1.5 - x[0] + x[0]*x[1]), 2) + pow((2.25 - x[0] + x[0]*x[1]*x[1]), 2) + pow((2.625 - x[0] + x[0]*x[1]*x[1]*x[1]), 2);
	double val = 1.0;
	val = val + x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
	return val;
}

double func(dvec x)
{
	double x1 = x[0];
	double x2 = x[1];
	double x3 = x[2];
	return -x1 * x2 - (x2 - 0.05) * (x2 - 0.05) - x3 * x3;
}


int main(){

	dvec lb = { 0, -1, 0.5 };
	dvec ub = { 1, 1, 1 };
	double y = 0;

    auto evo = diff_evo(func, 3, lb, ub);
    auto sol = evo.optimize();

	y = func(sol);

    cout << "optimal solution:" << endl;
    for(int i=0; i<sol.size(); ++i){
        cout << sol[i] << endl;
    }
    cout << "optimal cost: " << y << endl;

	system("pause");
    return 0;
}
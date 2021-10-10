#include "../Simulated Annealing/SA.h"
#define _CRT_SECURE_NO_WARNINGS 1
using namespace std;

void main()
{
	srand((unsigned)time(NULL));

	time_t start, finish;
	start = clock();

	SA SA1;
	SA1.in();
	SA1.SA();

	finish = clock();
	double run_time = ((double)(finish - start)) / CLOCKS_PER_SEC;

	printf("模拟退火算法解TSP问题：\n");
	cout << "初始温度T0=" << T0 << ",降温系数q=" << q << endl;
	printf("每个温度迭代%d次,共降温%d次。\n", L, SA1.getCount());

	SA1.get_All_solutions();
	SA1.out();

	printf("最终得TSP问题最优路径为：\n");
	for (int j = 0; j < C - 1; j++)
	{
		printf("%d—>", SA1.getCurrent_Solution(j));
	}
	printf("%d—>", SA1.getCurrent_Solution(C - 1));
	printf("%d\n", SA1.getCurrent_Solution(0));

	cout << "最优路径长度为：" << SA1.getF1() << endl;
	printf("程序运行耗时%1f秒.\n", run_time);
	system("pause");

}
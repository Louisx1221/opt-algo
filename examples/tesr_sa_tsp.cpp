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

	printf("ģ���˻��㷨��TSP���⣺\n");
	cout << "��ʼ�¶�T0=" << T0 << ",����ϵ��q=" << q << endl;
	printf("ÿ���¶ȵ���%d��,������%d�Ρ�\n", L, SA1.getCount());

	SA1.get_All_solutions();
	SA1.out();

	printf("���յ�TSP��������·��Ϊ��\n");
	for (int j = 0; j < C - 1; j++)
	{
		printf("%d��>", SA1.getCurrent_Solution(j));
	}
	printf("%d��>", SA1.getCurrent_Solution(C - 1));
	printf("%d\n", SA1.getCurrent_Solution(0));

	cout << "����·������Ϊ��" << SA1.getF1() << endl;
	printf("�������к�ʱ%1f��.\n", run_time);
	system("pause");

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double g_price[20] = { 2.6584, 2.6584, 2.6543, 2.6537,2.6546,   2.6592,2.6372,2.6318,2.6061,2.5981,  2.6138,2.6269,2.6231,2.6245,2.6278,2.6347, 2.6450,2.6381,2.6458,2.6312 };

double g_price1[] = {
	2.6356,
	2.6269,
	2.6292,
	2.6378,
	2.6524,
	2.6477,
	2.6448,
	2.6516,
	2.6413,
	2.6344,
	2.6311,
	2.6297,
	2.6335,
	2.6203,
	2.6046,
	2.6127,
	2.6384,
	2.6439,
	2.6659,
	2.6613,
	2.6603,
	2.661 ,
	2.6651,
	2.6651,
	2.6673,
	2.6686,
	2.6717,
	2.6758,
	2.6723,
	2.672 ,
	2.6754,
	2.6696,
	2.6739,
	2.6737,
	2.6646,
	2.6529,
	2.6448,
	2.6687,
	2.6716,
	2.6683,
	2.6702,
	2.6803,
	2.6729,
	2.6814,
	2.6827,
	2.6736,
	2.6747,
	2.6692,
	2.6651,
	2.6648,
	2.6611,
	2.6667,
	2.6632,
	2.6628,
	2.6581,
	2.6489,
	2.6544,
	2.6696,
	2.6786,
};

double price_difff[20] = { 0 };
typedef struct stat {
	int times_in;
	int times_out;
	double price;
}stat;

stat g_stat[6] = { {0} };

void func(double cur ,double year)
{
	for (int i = 0; i < year * 12 * 30; ++i)
	{
		cur = cur + cur * 0.15 / 12.0 / 30.0;
	}
	printf("year %f sum %f\n", year, cur);
	
}

void statistic(int p, int i, double price)
{
	if (p) {
		g_stat[i].price += price; //卖出的钱
		g_stat[i].times_out++;  //卖出次数
	}
	else {
		g_stat[i].price -= price;
		g_stat[i].times_in++;
	}
}

double price_diff(double a, double b, int *p, double *argv)
{
	double r = b - a;
	double ret = 0;
	*p = true; //涨
	if (r < 0)
	{
		r = -r;
		*p = false;//跌
	}


	if (r > 0.0200)
	{
		ret = argv[5];
		statistic(*p, 5, ret);
	}
	else if (r > 0.0150 && r <= 0.0200)
	{
		ret = argv[4];
		statistic(*p, 4, ret);

	}
	else if (r > 0.0100 && r <= 0.0150)
	{
		ret = argv[3];
		statistic(*p, 3, ret);

	}
	else if (r > 0.0050 && r <= 0.0100)
	{
		ret = argv[2];
		statistic(*p, 2, ret);

	}
	else if (r > 0.0010 && r <= 0.0050)
	{
		ret = argv[1];
		statistic(*p, 1, ret);

	}
	else
	{
		ret = argv[0];
		statistic(*p, 0, ret);
	}
	return ret;
}



void training()
{
	double total = 0, total_tmp = 0;
	double count = 0;
	double ret = 0;
	double para_train[6] = { 0 };
	double para_pri[6] = { 0 };

	int p = 0;
	for (int n = 0; n < 6; n++) //针对每种投入金额进行训练，得出每一种的最高值
	{
		memset(para_train, 0, sizeof(double) * 6);

		for (double k = 0; k < 100; k++)
		{
			para_train[n] = k;

			for (int i = 1; i < sizeof(g_price1)/sizeof(double) -2 ; ++i)
			{
				ret = price_diff(g_price1[i - 1], g_price1[i], &p, para_train);
				//price_difff[i] = price[i]- price[i-1];
				if (ret) {
					if (p == true) //涨            
						count -= (ret / g_price1[i + 1]); //卖出股数
					else
						count += (ret / g_price1[i + 1]); //买入股数
				}
			}
			//最后的收益
			total = count * g_price1[sizeof(g_price1) / sizeof(double)-1] + g_stat[0].price + g_stat[1].price + g_stat[2].price + g_stat[3].price + g_stat[4].price + g_stat[5].price;

			if (total_tmp <= total)
				para_pri[n] = k;  //保持更新到最高的收益

			total_tmp = total;

			//初始化
			count = 0;
			memset(g_stat, 0, sizeof(stat) * 6);
		}

	}




	for (int i = 1; i < sizeof(g_price1) / sizeof(double) - 2; ++i)
	{
		ret = price_diff(g_price1[i - 1], g_price1[i], &p, para_pri);
		//price_difff[i] = price[i]- price[i-1];
		if (ret) {
			if (p == true) //涨            
				count -= (ret / g_price1[i + 1]); //卖出股数
			else
				count += (ret / g_price1[i + 1]); //买入股数
		}
	}
	//最后的收益
	total = count * g_price1[sizeof(g_price1) / sizeof(double)-1] + g_stat[0].price + g_stat[1].price + g_stat[2].price + g_stat[3].price + g_stat[4].price + g_stat[5].price;
}

void main()
{
	training();
}





#include <stdio.h>
double price[20] = { 2.6584, 2.6584, 2.6543, 2.6537,2.6546,   2.6592,2.6372,2.6318,2.6061,2.5981,  2.6138,2.6269,2.6231,2.6245,2.6278,2.6347,
	2.6450,2.6381,2.6458,2.6312 };
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
		g_stat[i].price += price;
		g_stat[i].times_out++;
	}
	else {
		g_stat[i].price -= price;
		g_stat[i].times_in++;
	}
}

double price_diff(double a, double b, int *p)
{
	double r = b - a;
	double ret = 0;
	*p = false;
	if (r < 0)
	{
		r = -r;
		*p = true;
	}


	if (r > 0.0200)
	{
		ret = 10;
		statistic(*p, 5, ret);
	}
	else if (r > 0.0150 && r <= 0.0200)
	{
		ret = 10;
		statistic(*p, 4, ret);

	}
	else if (r > 0.0100 && r <= 0.0150)
	{
		ret = 10;
		statistic(*p, 3, ret);

	}
	else if (r > 0.0050 && r <= 0.0100)
	{
		ret = 50;
		statistic(*p, 2, ret);

	}
	else if (r > 0.0010 && r <= 0.0050)
	{
		ret = 100;
		statistic(*p, 1, ret);

	}
	else
	{
		ret = 0;
		statistic(*p, 0, ret);
	}
	return ret;
}




int main()
{
	double base = 100 / 2.6061;
	double dif = 0;
	double ret = 0;
	int p;

	for (int i = 0; i < 18; ++i)
	{
		ret = price_diff(price[i], price[i + 1], &p);
		price_difff[i] = price[i + 1]- price[i];

		if (p == true)
			dif = (ret/ price[i + 1])* (price[i+2] - price[i + 1]);
		else
			dif = (-ret/ price[i + 1]) * (price[i + 2] - price[i + 1]);
	}

	return 0;
}
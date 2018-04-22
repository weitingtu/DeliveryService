#include "feasibleStochasticDemand.h"



FeasibleStochasticDemand::FeasibleStochasticDemand(const Demand & d, const Trips & t)
{
}


FeasibleStochasticDemand::~FeasibleStochasticDemand()
{
}

static unsigned _nChoosek(int n, int k)
{
	if (k > n) return 0;
	if (k * 2 > n) k = n - k;
	if (k == 0) return 1;

	int result = n;
	for (int i = 2; i <= k; ++i) 
	{
		result *= (n - i + 1);
		result /= i;
	}
	return result;
}

void FeasibleStochasticDemand::start()
{
}

int FeasibleStochasticDemand::_get_p1() const
{
	int x1 = 0;
	for (size_t p = 0; p < POPULATION; ++p)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t j = 0; j < DISTRICT; ++j)
				{
					for (size_t k = 0; k < TASK; ++k)
					{
						x1 += _trips.x2()[p][t][i][j][k];
					}
				}
			}
		}
	}

	int n1 = 0;
	for (size_t p = 0; p < POPULATION; ++p)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
						n1 += _trips.y2()[p][t][j][k];
				}
			}
		}
	}

	n1 += x1;

	return _nChoosek(n1, x1);
}

int FeasibleStochasticDemand::_get_p2() const
{
	int x2 = 0;
	for (size_t p = 0; p < POPULATION; ++p)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t m = 0; m < STATION; ++m)
				{
					x2 += _trips.x3()[p][t][i][m];
				}
			}
		}
	}

	int n2 = 0;
	for (size_t p = 0; p < POPULATION; ++p)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
					n2 += _trips.y3()[p][t][m];
			}
			
		}
	}

	n2 += x2;

	return _nChoosek(n2, x2);
}

int FeasibleStochasticDemand::_get_p3() const
{
	int x3 = 0;
	for (size_t p = 0; p < POPULATION; ++p)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					x3 += _trips.x4()[p][t][i][k];
				}
			}
		}
	}

	int n3 = 0;
	for (size_t p = 0; p < POPULATION; ++p)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
					n3 += _trips.y4()[p][t][k];
			}
			
		}
	}

	n3 += x3;

	return _nChoosek(n3, x3);
}


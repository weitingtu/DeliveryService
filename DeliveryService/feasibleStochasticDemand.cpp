#include "feasibleStochasticDemand.h"
#include "defines.h"

FeasibleStochasticDemand::FeasibleStochasticDemand(const Demands & d, const Trips & t) :
	_p1(0.8),
	_p2(0.1),
	_p3(0.7),
	_pf(0.5),
	_demand(d),
	_trips(t)
{
}

FeasibleStochasticDemand::~FeasibleStochasticDemand()
{
}

static int _nChoosek(int n, int k)
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
	for (size_t p = 0; p < POPULATION; ++p)
	{
		_start(p);
	}
}

void FeasibleStochasticDemand::_start(size_t p)
{
	while ( _get_p1(p) * _get_p2(p) * _get_p3(p) < _pf)
	{
		// change x2, y2, x3, y3, x4, y4 to satisfy
	}
}

double FeasibleStochasticDemand::_get_p1(size_t p) const
{
	int x1 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t j = 0; j < DISTRICT; ++j)
				{
					for (size_t k = 0; k < TASK; ++k)
					{
						x1 += _trips.trips()[p][t].x2()[s][i][j][k];
					}
				}
			}
		}
	}

	int n1 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					n1 += _trips.trips()[p][t].y2()[s][j][k];
				}
			}
		}
	}

	n1 += x1;

	return ((double)_nChoosek(n1, x1)) * std::pow(_p1, x1) * std::pow(1 - _p1, n1 - x1);
}

double FeasibleStochasticDemand::_get_p2(size_t p) const
{
	int x2 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t m = 0; m < STATION; ++m)
				{
					x2 += _trips.trips()[p][t].x3()[s][i][m];
				}
			}
		}
	}

	int n2 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				n2 += _trips.trips()[p][t].y3()[s][m];
			}
		}
	}

	n2 += x2;

	return ((double)_nChoosek(n2, x2)) * std::pow(_p2, x2) * std::pow(1 - _p2, n2 - x2);
}

double FeasibleStochasticDemand::_get_p3(size_t p) const
{
	int x3 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t i = 0; i < FLEET; ++i)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					x3 += _trips.trips()[p][t].x4()[s][i][k];
				}
			}
		}
	}

	int n3 = 0;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		for (size_t t = 0; t < DAY; ++t)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				n3 += _trips.trips()[p][t].y4()[s][k];
			}
		}
	}

	n3 += x3;

	return ((double)_nChoosek(n3, x3)) * std::pow(_p3, x3) * std::pow(1 - _p3, n3 - x3);
}


#include "feasibleStochasticDemand.h"
#include "defines.h"
#include "verify.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <boost/math/distributions/binomial.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/gamma.hpp>

FeasibleStochasticDemand::FeasibleStochasticDemand(const Demands & d, const Trips & t) :
	_p1(0.5),
	_p2(0.1),
	_p3(0.5),
	_pf(0.8),
	_demand(d),
	_trips(t),
	_min_max_c1(),
	_max_min_c1()
{
}

FeasibleStochasticDemand::~FeasibleStochasticDemand()
{
}

void FeasibleStochasticDemand::_initialize_cost_array()
{
	for (size_t k = 0; k < TASK; ++k)
	{
		std::vector<std::pair<double, size_t> > c;
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			c.push_back(std::make_pair(_demand.c1()[j][k], j));
		}
		std::sort(c.begin(), c.end());
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			_min_max_c1[j][k] = c[j].second;
		}
		std::reverse(c.begin(), c.end());
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			_max_min_c1[j][k] = c[j].second;
		}
	}
}

int FeasibleStochasticDemand::_get_sum_x2(size_t p, size_t s) const
{
	int sum_x2 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					sum_x2 += _trips.trips()[p][t].x2()[s][i][j][k];
				}
			}
		}
	}
	return sum_x2;
}

int FeasibleStochasticDemand::_get_sum_y2(size_t p, size_t s) const
{
	int sum_y2 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_y2 += _trips.trips()[p][t].y2()[s][j][k];
			}
		}
	}
	return sum_y2;
}

int FeasibleStochasticDemand::_get_sum_x3(size_t p, size_t s) const
{
	int sum_x3 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				sum_x3 += _trips.trips()[p][t].x3()[s][i][m];
			}
		}
	}
	return sum_x3;
}

int FeasibleStochasticDemand::_get_sum_y3(size_t p, size_t s) const
{
	int sum_y3 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			sum_y3 += _trips.trips()[p][t].y3()[s][m];
		}
	}
	return sum_y3;
}

int FeasibleStochasticDemand::_get_sum_x4(size_t p, size_t s) const
{
	int sum_x4 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_x4 += _trips.trips()[p][t].x4()[s][i][k];
			}
		}
	}
	return sum_x4;
}

int FeasibleStochasticDemand::_get_sum_y4(size_t p, size_t s) const
{
	int sum_y4 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			sum_y4 += _trips.trips()[p][t].y4()[s][k];
		}
	}
	return sum_y4;
}

void FeasibleStochasticDemand::start()
{
	_initialize_cost_array();
	//for (size_t p = 0; p < POPULATION; ++p)
	//for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		size_t p = 0;
		size_t s = 1;
		printf("Start feasible stochastic demand %zu %zu\n", p, s);
		_start(p, s);
	}
}

void FeasibleStochasticDemand::_start(size_t p, size_t s)
{
	if (p == s)
	{
		return;
	}
	int X1 = _get_sum_x2(p, s);
	int N1 = _get_sum_y2(p, s) + X1;
	int X2 = _get_sum_x3(p, s);
	int N2 = _get_sum_y3(p, s) + X2;
	int X3 = _get_sum_x4(p, s);
	int N3 = _get_sum_y4(p, s) + X3;
	printf("%d %d %d %d %d %d\n", X1, N1, X2, N2, X3, N3);

	double p1 = _get_p(X1, N1, _p1);
	double p2 = _get_p(X2, N2, _p2);
	double p3 = _get_p(X3, N3, _p3);
	printf("%f %f %f\n", p1, p2, p3);
}

double FeasibleStochasticDemand::_get_p(int X, int N, double p) const
{
	using boost::math::binomial_distribution;
	using boost::math::binomial;

	return cdf(complement(binomial(N, p), X));
}

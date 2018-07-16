#include "feasibleStochasticDemand.h"
#include "defines.h"
#include "verify.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <boost/math/distributions/binomial.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/gamma.hpp>

FeasibleStochasticDemand::FeasibleStochasticDemand(const Demands& d) :
	_p1(0.5),
	_p2(0.1),
	_p3(0.5),
	_pf(0.8),
	_demand(d),
	_trips(nullptr),
	_min_max_c1(),
	_max_min_c1()
{
	_initialize_cost_array();
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

int FeasibleStochasticDemand::_get_sum_x2(size_t s) const
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
					sum_x2 += (*_trips)[t].x2()[s][i][j][k];
				}
			}
		}
	}
	return sum_x2;
}

int FeasibleStochasticDemand::_get_sum_y2(size_t s) const
{
	int sum_y2 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t j = 0; j < DISTRICT; ++j)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_y2 += (*_trips)[t].y2()[s][j][k];
			}
		}
	}
	return sum_y2;
}

int FeasibleStochasticDemand::_get_sum_x3(size_t s) const
{
	int sum_x3 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t m = 0; m < STATION; ++m)
			{
				sum_x3 += (*_trips)[t].x3()[s][i][m];
			}
		}
	}
	return sum_x3;
}

int FeasibleStochasticDemand::_get_sum_y3(size_t s) const
{
	int sum_y3 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t m = 0; m < STATION; ++m)
		{
			sum_y3 += (*_trips)[t].y3()[s][m];
		}
	}
	return sum_y3;
}

int FeasibleStochasticDemand::_get_sum_x4(size_t s) const
{
	int sum_x4 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t k = 0; k < TASK; ++k)
			{
				sum_x4 += (*_trips)[t].x4()[s][i][k];
			}
		}
	}
	return sum_x4;
}

int FeasibleStochasticDemand::_get_sum_y4(size_t s) const
{
	int sum_y4 = 0;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t k = 0; k < TASK; ++k)
		{
			sum_y4 += (*_trips)[t].y4()[s][k];
		}
	}
	return sum_y4;
}

bool FeasibleStochasticDemand::start(FILE* fp, size_t count, size_t p, std::vector<Trip>& trips)
{
	_trips = &trips;
	bool r = true;
	std::vector<std::string> results;
	for (size_t s = 0; s < STOCHASTIC_DEMAND; ++s)
	{
		if (!_start(count, p, s, results))
		{
			r = false;
			break;
		}
	}

	if (r)
	{
		for (const std::string& str : results)
		{
			fprintf(fp, "%s\n", str.c_str());
		}
	}
	return r;
}

bool FeasibleStochasticDemand::_start(size_t count, size_t p, size_t s, std::vector<std::string>& results) const
{
	char buf[1024];

	int X1 = _get_sum_x2(s);
	int N1 = _get_sum_y2(s) + X1;
	int X2 = _get_sum_x3(s);
	int N2 = _get_sum_y3(s) + X2;
	int X3 = _get_sum_x4(s);
	int N3 = _get_sum_y4(s) + X3;
	double p1 = _get_p(X1, N1, _p1);
	double p2 = _get_p(X2, N2, _p2);
	double p3 = _get_p(X3, N3, _p3);
	double pf = p1 * p2 * p2;

	if (0 == N1 || 0 == N2 || 0 == N3)
	{
		snprintf(buf, 1024, "%zu, %zu, %zu, %d, %d, %f, %d, %d, %f, %d, %d, %f, %f",
			count, p, s, N1, X1, p1, N2, X2, p2, N3, X3, p3, pf);
		results.push_back(buf);
		return true;
	}

	if (p1 < 0.8)
	{
		int X = X1 - 1;
		for (; X >= 0; --X)
		{
			p1 = _get_p(X, N1, _p1);
			if (p1 >= 0.8)
			{
				break;
			}
		}
		if (p1 < 0.8)
		{
			return false;
		}
		X1 = X;
	}

	pf = p1 * p2 * p2;

	if (pf >= _pf)
	{
		//printf("%zu, %zu, %zu, %d, %d, %f, %d, %d, %f, %d, %d, %f, %f\n",
			//count, p, s, N1, X1, p1, N2, X2, p2, N3, X3, p3, pf);
		snprintf(buf, 1024, "%zu, %zu, %zu, %d, %d, %f, %d, %d, %f, %d, %d, %f, %f",
			count, p, s, N1, X1, p1, N2, X2, p2, N3, X3, p3, pf);
		results.push_back(buf);
		return true;
	}

	int X = X1 - 1;
	for (; X >= 0; --X)
	{
		p1 = _get_p(X, N1, _p1);
		pf = p1 * p2 * p2;
		if (pf >= _pf)
		{
			break;
		}
	}

	// failed to match
	if (pf < _pf)
	{
		//printf("failed to match: %zu, %zu, %zu, %d, %d, %f, %d, %d, %f, %d, %d, %f, %f\n",
			//count, p, s, N1, X1, p1, N2, X2, p2, N3, X3, p3, pf);
		return false;
	}

	//printf("match %d -> %d: %zu, %zu, %zu, %d, %d, %f, %d, %d, %f, %d, %d, %f, %f\n",
		//X1, X,
		//count, p, s, N1, X1, p1, N2, X2, p2, N3, X3, p3, pf);
	X1 = X;

	snprintf(buf, 1024, "%zu, %zu, %zu, %d, %d, %f, %d, %d, %f, %d, %d, %f, %f",
		count, p, s, N1, X1, p1, N2, X2, p2, N3, X3, p3, pf);
	results.push_back(buf);

	int diff = X1 - X;
	for (size_t t = 0; t < DAY; ++t)
	{
		for (size_t i = 0; i < FLEET; ++i)
		{
			for (size_t j = 0; j < DISTRICT; ++j)
			{
				for (size_t k = 0; k < TASK; ++k)
				{
					if (0 == diff)
					{
						break;
					}
					int x1 = (*_trips)[t].x2()[s][i][j][k];
					int d = x1 >= diff ? diff : x1;
					(*_trips)[t].x2()[s][i][j][k] -= d;
					(*_trips)[t].y2()[s][j][k] += d;
					diff -= d;
				}
			}
		}
	}

	return true;
}

double FeasibleStochasticDemand::_get_p(int X, int N, double p) const
{
	using boost::math::binomial_distribution;
	using boost::math::binomial;

	return cdf(complement(binomial(N, p), X));
}
